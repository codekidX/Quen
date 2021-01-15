#include <list>
#include "lexer.h"

#ifndef PARSER_H
#define PARSER_H

class Node {
    public:
        std::list<Node> child;
};

class ASTNode
{

private:
    Node root;
public:
    Node child;

};

class Parser
{

private:
    std::list<TokenDef> lexed_tokens;
    ASTNode ast;

public:
    Parser(std::list<TokenDef>);
    ASTNode get_ast();
    void parse();
};

inline Parser::Parser(std::list<TokenDef> tokens)
{
    this->lexed_tokens = tokens;
}

inline ASTNode Parser::get_ast() {
    return this->ast;
}

#endif