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
    };
    struct FunctionInfo {
        std::string name;
        std::vector<FunctionDeclarationNode::Param> parameters;
    };
    using Scope = std::stack<std::unordered_map<std::string, std::variant<VariableInfo, FunctionInfo>>>;
private:

    Scope scopes;

    void enterScope();
    void exitScope();
    void declareVariable(const std::string& name, const std::string& type, bool isConst);
    void declareFunction(const std::string& name,
        const std::vector<FunctionDeclarationNode::Param>& parameters);

    template <typename  T>
    T lookup(const std::string& name, const std::string& message) const {
        auto tempScopes = scopes;

        while (!tempScopes.empty()) {
            if (const auto& currentScope = tempScopes.top(); currentScope.contains(name)) {
                return std::get<T>(currentScope.at(name));
            }
            tempScopes.pop();
        }

        throw std::runtime_error(message);
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

};

#endif //SEMANTIC_ANALYZER_H
