#include <string>
#include <list>

#ifndef LEXER_H
#define LEXER_H

enum Token
{
    Definition,
    ScopeAccessor,
    Scope,
    ScopeStart,
    ScopeEnd,
    LParen,
    RParen,
    LBrace,
    RBrace,
    Function,
    Comment,
    Assignment,
    Constant,
    Variable,
    Value,
    Import,
    Quote,
    Comma,
    Text,
    Dot,
    LineBreak,
    For,
    If,
    Else,
    ElseIf,
    BoolVal,

    String,
    Bool,
    Float,
    Int,
    Fn,

    Addition,
    Subtraction,
    Multiplication,
    Division,
    Modulo,

    And,
    Or,
    Is,
    IsNot
};


struct TokenDef
{
    Token t;
    std::string token_str;
    int start;
    int end;
    std::string value;
};

class Lexer
{

private:
    const char *file_name;
    std::string contents;
    char current_char;
    char next_char;
    int position;
    std::list<TokenDef> tokens;

public:
    Lexer(const char *file_name);
    void read_from_file();
    void analyze();
    int next();
    int check_next();
    std::list<TokenDef> get_tokens();
    void checkIfKeyword(std::string text);
    void addKeyword(std::string word);
};


inline std::string get_str_token(Token t)
{
    switch (t)
    {
    case Definition:
        return "Definition";
    case ScopeAccessor:
        return "ScopeAccessor";
    case Scope:
        return "Scope";
    case ScopeStart:
        return "ScopeStart";
    case ScopeEnd:
        return "ScopeEnd";
    case LParen:
        return "LParen";
    case RParen:
        return "RParen";
    case LBrace:
        return "LBrace";
    case RBrace:
        return "RBrace";
    case Function:
        return "Function";
    case Comment:
        return "Comment";
    case Assignment:
        return "Assignment";
    case Constant:
        return "Constant";
    case Variable:
        return "Variable";
    case Value:
        return "Value";
    case Import:
        return "Import";
    case Quote:
        return "Quote";
    case Comma:
        return "Comma";
    case Text:
        return "Text";
    case Dot:
        return "Dot";
    case LineBreak:
        return "LineBreak";
    case For:
        return "For";
    case If:
        return "If";
    case Else:
        return "Else";
    case ElseIf:
        return "ElseIf";
    case BoolVal:
        return "BoolVal";
        return "case";
    case String:
        return "String";
    case Bool:
        return "Bool";
    case Float:
        return "Float";
    case Int:
        return "Int";
    case Fn:
        return "Fn";
        return "case";
    case Addition:
        return "Addition";
    case Subtraction:
        return "Subtraction";
    case Multiplication:
        return "Multiplication";
    case Division:
        return "Division";
    case Modulo:
        return "Modulo";
        return "case";
    case And:
        return "And";
    case Or:
        return "Or";
    case Is:
        return "Is";
    case IsNot:
        return "IsNot";
    default:
        break;
    }
}

inline std::list<TokenDef> Lexer::get_tokens()
{
    return tokens;
}

#endif