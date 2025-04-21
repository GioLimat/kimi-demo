//
// Created by home on 19/04/25.
//

#ifndef BYGEN_EXPRESSION_H
#define BYGEN_EXPRESSION_H

#include "ast.h"
#include "bygen.h"

class BygenExpression {
public:
     static std::vector<std::string> generateExpression(const ExpressionNode *expression);
private:
     static std::vector<std::string> binExpression(const BinaryExprNode* expression);
     static std::string number(const NumberNode *number);
     static std::string identifier(const IdentifierExprNode *identifier);
     static std::vector<std::string> callFunction(const std::shared_ptr<ASTNode>& function);
     static std::vector<std::string> unaryExpression(const std::shared_ptr<ASTNode>& expression);
     static std::string getOpCode(const std::string& op, const std::string& type);
};

#endif //BYGEN_EXPRESSION_H
