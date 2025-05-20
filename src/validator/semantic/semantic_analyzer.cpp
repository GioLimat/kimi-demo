//
// Created by home on 23/04/25.
//
#include "semantic_analyzer.h"

#include <complex>
#include <iostream>

#include "sizes.h"
#include "type_analyzer.h"


void SemanticAnalyzer::analyze(const AST& ast) {
    enterScope();
    for (auto& node : ast.children) {
        node->accept(*this);
    }
    exitScope();
}

void SemanticAnalyzer::visitBinaryExpr(BinaryExprNode *node) {
    node->left->accept(*this);
    node->right->accept(*this);

    std::string leftType  = TypeInfer::analyzeExpression(node->left.get(),  &scopes);
    std::string rightType = TypeInfer::analyzeExpression(node->right.get(), &scopes);
    const std::string &op = node->op;

    const static std::set<std::string> bitwiseOps = {"&", "|", "^", "<<", ">>"};
    if (bitwiseOps.contains(op)) {
        auto itL = typePrecedence.find(leftType);
        auto itR = typePrecedence.find(rightType);
        if (itL == typePrecedence.end() || itR == typePrecedence.end()
            || leftType.front() != 'i' || rightType.front() != 'i')
        {
            throw std::runtime_error("Bitwise operators require integer types, got: "
                                     + leftType + " and " + rightType);
        }
    }

    node->type = TypeInfer::analyzeExpression(node, &scopes);
}


void SemanticAnalyzer::visitIdentifier(IdentifierExprNode *node) {
    TypeInfer::analyzeExpression(node, &scopes);
}


void SemanticAnalyzer::visitVarDeclaration(VarDeclarationNode* var) {
    std::string inferredType;

    if (var->initializer) {
        var->initializer->accept(*this);
        inferredType = TypeInfer::analyzeExpression(var->initializer.get(), &scopes);

        if (var->declaredType.empty()) {
            if (inferredType.rfind('i', 0) == 0) {
                if (inferredType == "i64") inferredType = "i64";
                else inferredType = "i32";
            }
            else if (inferredType.rfind('f', 0) == 0) {
                inferredType = "f64";
            }
        }
        else {
            auto itInf = typePrecedence.find(inferredType);
            auto itDec = typePrecedence.find(var->declaredType);

            if (itInf == typePrecedence.end() || itDec == typePrecedence.end()) {
                throw std::runtime_error(
                    "Unknown types in declaration: inferred=" + inferredType +
                    ", declared=" + var->declaredType
                );
            }
            if (itInf->second > itDec->second) {
                throw std::runtime_error(
                    "Type mismatch in variable declaration: expected a type at least as wide as "
                    + var->declaredType + ", got " + inferredType
                );
            }
            inferredType = var->declaredType;
        }
    }
    else if (var->isConst) {
        throw std::runtime_error("Const variable " + var->name + " must be initialized");
    }
    else if (var->declaredType.empty()) {
        throw std::runtime_error("Variable " + var->name + " must have a type or be initialized, to type infer");
    }
    if (scopes.size() == 1) {
        var->isGlobal = true;
    }
    else {
        var->isGlobal = false;
    }

    const std::string finalType = var->declaredType.empty() ? inferredType : var->declaredType;
    declareVariable(var->name, finalType, var->isConst, var->isGlobal);
    var->type = finalType;
}

void SemanticAnalyzer::visitAssignmentExpr(AssignmentExprNode* expr) {
    std::string storeType = TypeInfer::analyzeExpression(expr->value.get(), &scopes);

    auto [varType, isConst, isGlobal] =
        lookup<VariableInfo>(expr->name, "Variable " + expr->name + " not declared in this scope");
    if (isConst) {
        throw std::runtime_error("Cannot assign to const variable " + expr->name);
    }

    auto itStore = typePrecedence.find(storeType);
    auto itVar   = typePrecedence.find(varType);
    if (itStore == typePrecedence.end() || itVar == typePrecedence.end()) {
        throw std::runtime_error(
            "Unknown types in assignment: value=" + storeType + ", variable=" + varType
        );
    }
    if (itStore->second > itVar->second) {
        throw std::runtime_error(
            "Type mismatch in assignment to '" + expr->name +
            "': variable is " + varType + ", but the value is " + storeType
        );
    }

    expr->type = varType;
}



void SemanticAnalyzer::visitFunctionDeclaration(FunctionDeclarationNode *node) {
    currentFns.push_back(node->name);
    declareFunction(node->name, node->parameters, node->returnType);
    enterScope();
    for (const auto& param : node->parameters) {
        declareVariable(param.name, param.type, false, false);
    }
    for (const auto& child : node->body->statements) {
        child->accept(*this);
    }
    auto fn = lookup<FunctionInfo>(node->name, "Function not declared");
    if (fn.returnType.empty()) {
        fn.returnType = "void";
    }
    exitScope();
    currentFns.pop_back();
}


void SemanticAnalyzer::visitIfStatement(IfStatementNode *node) {
    enterScope();
    node->condition->accept(*this);
    for (const auto& child : node->thenBranch->statements) {
        child->accept(*this);
    }
    exitScope();
    if (node->elseBranch) {
        node->elseBranch->accept(*this);
    }

}

void SemanticAnalyzer::visitWhileStatement(WhileStatementNode *node) {
    enterScope();
    node->condition->accept(*this);
    for (const auto& child : node->body->statements) {
        child->accept(*this);
    }
    exitScope();
}


void SemanticAnalyzer::visitDoWhileStatement(DoWhileStatementNode *node) {
    enterScope();
    for (const auto& child : node->whileStatement->body->statements) {
        child->accept(*this);
    }
    node->whileStatement->condition->accept(*this);
    exitScope();
}

void SemanticAnalyzer::visitPrintln(PrintlnStatementNode *node) {
    const std::set<std::string> comparisonOps = {"==", "!=", "<", ">", "<=", ">="};
    node->expression->accept(*this);
    auto preType = TypeInfer::analyzeExpression(node->expression.get(), &scopes);
    if (auto bin = dynamic_cast<BinaryExprNode*>(node->expression.get())) {
        if (comparisonOps.contains(bin->op)) {
            node->type = "bool";
        }
        else node->type = preType;
    }
    else node->type = preType;
}

void SemanticAnalyzer::visitReturnStatement(ReturnStatementNode *node) {
    if (node->returnValue) {
        node->returnValue->accept(*this);
        std::string inferred = TypeInfer::analyzeExpression(node->returnValue.get(), &scopes);
        auto fnInfo = lookup<FunctionInfo>(currentFns.back(), "Function " + currentFns.back() + " not declared in this scope");
        if (fnInfo.returnType != "infer") {
            auto itInf = typePrecedence.find(inferred);
            auto itDec = typePrecedence.find(fnInfo.returnType);
            if (itInf == typePrecedence.end() || itDec == typePrecedence.end() || itInf->second > itDec->second) {
                throw std::runtime_error(
                    "Return type mismatch in function " + currentFns.back() +
                    ": expected " + fnInfo.returnType + ", got " + inferred
                );
            }
            node->returnType = fnInfo.returnType;
        } else {
            updateScope(currentFns.back(),
                        FunctionInfo(fnInfo.name, fnInfo.parameters, inferred, inferred));
            node->returnType = inferred;
        }
    } else {
        auto fnInfo = lookup<FunctionInfo>(currentFns.back(), "Function " + currentFns.back() + " not declared in this scope");
        fnInfo.returnType = "void";
        node->returnType = "void";
    }
}



void SemanticAnalyzer::visitCallFunction(CallFunctionNode *node) {;
    auto functionInfo = lookup<FunctionInfo>(node->name, "Function " + node->name + " not declared in this scope");
    if (functionInfo.name != node->name) {
        throw std::runtime_error("Function " + node->name + " not declared in this scope");
    }


    if (functionInfo.parameters.size() != node->arguments.size()) {
        throw std::runtime_error("Function " + node->name + " expects " +
                                 std::to_string(functionInfo.parameters.size()) +
                                 " arguments, but got " + std::to_string(node->arguments.size()));
    }

    for (size_t i = 0; i < functionInfo.parameters.size(); i++) {
        node->arguments[i]->accept(*this);
    }
    node->returnType = functionInfo.returnType;
}


void SemanticAnalyzer::visitBlockStatement(BlockStatementNode *node) {
    enterScope();
    for (const auto& child : node->statements) {
        child->accept(*this);
    }
    exitScope();
}


void SemanticAnalyzer::visitGenericExpressionNode(GenericExpressionNode *node) {
    node->node->accept(*this);
}


void SemanticAnalyzer::visitUnaryExpr(UnaryExprNode *node) {
    node->operand->accept(*this);
    if (node->op == "++" || node->op == "--" || node->op == "~") {
        std::string type = TypeInfer::analyzeExpression(node->operand.get(), &scopes);
        if (type[0] != 'i') throw std::runtime_error("Unexpected type in unary operator " + node->op);
    }
    node->type = TypeInfer::analyzeExpression(node, &scopes);
}

void SemanticAnalyzer::visitPostFixExpr(PostFixExprNode *node) {
    node->operand->accept(*this);
    const auto type = TypeInfer::analyzeExpression(node->operand.get(), &scopes);
    if (node->op == "++" || node->op == "--") {
        if (type[0] != 'i') throw std::runtime_error("Unexpected type in unary operator " + node->op);
    }
    node->type = type;
}



void SemanticAnalyzer::declareFunction(const std::string &name, const std::vector<FunctionDeclarationNode::Param> &parameters, const std::string &returnType) {
    if (scopes.top().contains(name)) {
        if (std::holds_alternative<FunctionInfo>(scopes.top().at(name))) {
            throw std::runtime_error("Function " + name + " already declared in this scope");
        }
    }
    scopes.top()[name] = FunctionInfo{name, parameters, returnType};
}



void SemanticAnalyzer::declareVariable(const std::string& name, const std::string& type, const bool isConst, const bool isGlobal) {
    if (scopes.top().contains(name)) {
        throw std::runtime_error("Variable " + name + " already declared in this scope");
    }
    scopes.top()[name] = VariableInfo{type, isConst, isGlobal};
}


SemanticAnalyzer::FunctionInfo SemanticAnalyzer::lookupFunction(const std::string &name,
    std::vector<FunctionDeclarationNode::Param> parameters) const {
    auto tempScopes = scopes;

    while (!tempScopes.empty()) {
        if (const auto& currentScope = tempScopes.top(); currentScope.contains(name)) {
            auto fn = std::get<FunctionInfo>(currentScope.at(name));
            return fn;
        }
        tempScopes.pop();
    }

    throw std::runtime_error("Function " + name + " not declared in this scope");
}


void SemanticAnalyzer::enterScope() {
    scopes.emplace();
}

void SemanticAnalyzer::exitScope() {
    scopes.pop();
}


void SemanticAnalyzer::printAllScopes() {
    Scope scope = scopes;

    for (int i = 0; i < scopes.size(); ++i) {
        std::cout << "Scope " << i << ":" << std::endl;
        for (const auto& [name, info] : scope.top()) {
            if (std::holds_alternative<VariableInfo>(info)) {
                const auto& varInfo = std::get<VariableInfo>(info);
                std::cout << "  Variable: " << name << ", Type: " << varInfo.type << ", IsConst: " << varInfo.isConst << std::endl;
            }
            else if (std::holds_alternative<FunctionInfo>(info)) {
                const auto& funcInfo = std::get<FunctionInfo>(info);
                std::cout << "  Function: " << funcInfo.name << ", ReturnType: " << funcInfo.returnType << std::endl;
            }
        }
        scope.pop();
    }
}
