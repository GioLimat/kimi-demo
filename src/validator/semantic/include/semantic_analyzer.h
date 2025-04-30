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
        std::vector<std::string> parameters;
        std::vector<std::string> parametersTypes;
    };

    using Scope = std::stack<std::unordered_map<std::string, std::variant<FunctionInfo, VariableInfo>>>;
private:

   Scope scopes;

    void enterScope();
    void exitScope();
    void declareVariable(const std::string& name, const std::string& type, bool isConst);
    [[nodiscard]] VariableInfo lookupVariable(const std::string& name) const;
    [[nodiscard]] FunctionInfo lookupFunction(const std::string& name) const;
    void declareFunction(const std::string& name, const std::vector<std::string>& parameters,
                         const std::vector<std::string>& parametersTypes);


    void visitVarDeclaration(VarDeclarationNode* var) override;
    void visitAssignmentExpr(AssignmentExprNode* expr) override;
    void visitFunctionDeclaration(FunctionDeclarationNode *node) override;
    void visitBinaryExpr(BinaryExprNode *node) override;
    void visitExpressionStatement(ExpressionStatementNode *node) override;

};

#endif //SEMANTIC_ANALYZER_H
