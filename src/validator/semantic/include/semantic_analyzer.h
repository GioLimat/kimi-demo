//
// Created by home on 23/04/25.
//

#ifndef SEMANTIC_ANALYZER_H
#define SEMANTIC_ANALYZER_H

#include "ast.h"
#include <stack>
#include <visitor.h>
#include <unordered_map>
#include <variant>

class SemanticAnalyzer : public DefaultASTVisitor {
public:
    void analyze(const AST& ast);
    struct VariableInfo {
        std::string type;
        bool isConst;
        bool isGlobal;
    };
    struct FunctionInfo {
        std::string name;
        std::vector<FunctionDeclarationNode::Param> parameters;
        std::string returnType;
        std::string inferType;
    };
    using Scope = std::stack<std::unordered_map<std::string, std::variant<VariableInfo, FunctionInfo>>>;
private:
    std::string currentFn;
    Scope scopes;

    void printAllScopes();
    void enterScope();
    void exitScope();
    void declareVariable(const std::string& name, const std::string& type, bool isConst, bool isGlobal);
    void declareFunction(const std::string& name,
        const std::vector<FunctionDeclarationNode::Param>& parameters, const std::string& returnType);

    template <typename  T>
    T lookup(const std::string& name, const std::string& message) const {
        auto tempScopes = scopes;

        while (!tempScopes.empty()) {
            if (const auto& currentScope = tempScopes.top(); currentScope.contains(name)) {
                auto payload = currentScope.at(name);
                return std::get<T>(payload);
            }
            tempScopes.pop();
        }

        throw std::runtime_error(message);
    }


    template <typename T>
    void updateScope(const std::string& name, const T& value) {
        std::stack<std::unordered_map<std::string, std::variant<VariableInfo, FunctionInfo>>> temp;
        bool updated = false;


        while (!scopes.empty()) {
            auto& currentScope = scopes.top();
            if (currentScope.contains(name)) {
                currentScope[name] = value;
                updated = true;
                break;
            }
            temp.push(std::move(currentScope));
            scopes.pop();
        }

        if (!updated) {
            while (!temp.empty()) {
                scopes.push(std::move(temp.top()));
                temp.pop();
            }
            throw std::runtime_error("Name '" + name + "' not declared in any scope");
        }

        while (!temp.empty()) {
            scopes.push(std::move(temp.top()));
            temp.pop();
        }
    }

    [[nodiscard]] FunctionInfo lookupFunction(const std::string& name, std::vector<FunctionDeclarationNode::Param> parameters) const;


    void visitIdentifier(IdentifierExprNode *node) override;
    void visitVarDeclaration(VarDeclarationNode* var) override;
    void visitAssignmentExpr(AssignmentExprNode* expr) override;
    void visitBinaryExpr(BinaryExprNode *node) override;
    void visitFunctionDeclaration(FunctionDeclarationNode *node) override;
    void visitIfStatement(IfStatementNode *node) override;
    void visitWhileStatement(WhileStatementNode *node) override;
    void visitDoWhileStatement(DoWhileStatementNode *node) override;
    void visitPrintln(PrintlnStatementNode *node) override;
    void visitCallFunction(CallFunctionNode *node) override;
    void visitReturnStatement(ReturnStatementNode *node) override;
    void visitBlockStatement(BlockStatementNode *node) override;
    void visitGenericExpressionNode(GenericExpressionNode *node) override;

};

#endif //SEMANTIC_ANALYZER_H
