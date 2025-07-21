//
// Created by home on 14/04/25.
//

#ifndef PARSE_DECLARATION_H
#define PARSE_DECLARATION_H

#include "parser.h"
#include "parser_statement.h"
#include "parser_expression.h"

class ParserDeclaration : public Parser {
private:
    void extractType(std::string &type, std::string assumedType, std::vector<std::string> sizes);

public:
    explicit ParserDeclaration(const std::vector<LexerToken>& tokens) : Parser(tokens) {}
    std::unique_ptr<StatementNode> parseDeclaration();
    std::unique_ptr<StatementNode> parseVarDeclaration();
    std::unique_ptr<StatementNode> parseFunctionDeclaration();



private:
    std::string getType();
};

#endif //PARSE_DECLARATION_H
