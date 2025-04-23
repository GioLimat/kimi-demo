//
// Created by home on 19/04/25.
//

#ifndef IR_GEN_EXPRESSION_H
#define IR_GEN_EXPRESSION_H

#include "ast.h"
#include "ir_gen.h"

class IRGenExpression {
public:
     static std::vector<std::string> generateExpression(const ExpressionNode *expression);
private:
     static std::vector<std::string> binExpression(const BinaryExprNode* expression);
     static std::string number(const NumberNode *number);
     static std::string identifier(const IdentifierExprNode *identifier);
     static std::vector<std::string> callFunction(const std::shared_ptr<ASTNode>& function);
     static std::vector<std::string> unaryExpression(const std::shared_ptr<ASTNode>& expression);
};

#endif //IR_GEN_EXPRESSION_H
