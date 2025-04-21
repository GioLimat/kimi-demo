//
// Created by home on 21/04/25.
//

#ifndef PARSER_MACRO_H
#define PARSER_MACRO_H

#include "parser.h"
#include "lexer_tokens.h"

class ParserMacro : public Parser {
public:
    std::unique_ptr<StatementNode> parseMacro();
    explicit ParserMacro(const std::vector<LexerToken>& tokens): Parser(tokens) {};

};

#endif //PARSER_MACRO_H
