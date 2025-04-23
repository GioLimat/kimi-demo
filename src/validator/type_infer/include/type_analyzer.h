//
// Created by home on 23/04/25.
//

#ifndef TYPE_ANALYZER_H
#define TYPE_ANALYZER_H

#include "ast.h"

class TypeInfer {
public:
    static std::string analyzeExpression(ExpressionNode* expr);
};

#endif //TYPE_ANALYZER_H
