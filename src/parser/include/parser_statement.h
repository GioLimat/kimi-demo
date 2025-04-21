//
// Created by home on 14/04/25.
//

#ifndef PARSER_STATEMENT_H
#define PARSER_STATEMENT_H

#include "parser.h"

class ParserStatement : public Parser {
public:
    explicit ParserStatement(const std::vector<LexerToken>& tokens) : Parser(tokens) {}

    std::unique_ptr<StatementNode> parseStatement();
    std::unique_ptr<StatementNode> parsePrintln();
    std::unique_ptr<StatementNode> parseReturnStatement();
    std::unique_ptr<StatementNode> parseIfStatement();
    std::unique_ptr<StatementNode> parseWhileStatement();
    std::unique_ptr<StatementNode> parseDoWhileStatement();
    std::vector<std::unique_ptr<StatementNode>> parseBlock(int end);
    std::unique_ptr<StatementNode> generateWhileStatement();
};
#endif //PARSER_STATEMENT_H
