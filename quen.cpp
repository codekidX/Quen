#include <fstream>
#include <iostream>
#include "quen.h"
#include <list>
#include <regex>
#include "util.h"

using namespace std;

Lexer::Lexer(const char *file_name)
{
    this->file_name = file_name;
    this->position = 0;
}

void Lexer::read_from_file()
{
    string str;
    ifstream in_file(file_name);
    while (getline(in_file, str))
    {
        contents += str;
        contents.push_back('\n');
    }
    in_file.close();

    current_char = contents[position];
    next_char = contents[position + 1];
}

void Lexer::analyze()
{
    bool keepTokenizing = true;
    while (keepTokenizing)
    {
        switch (current_char)
        {
        case '/':
        {
            if (next_char == '/')
            {
                struct TokenDef comment_def;
                comment_def.t = Comment;
                comment_def.start = position;
                comment_def.token_str = get_str_token(Comment);
                tokens.push_back(comment_def);

                this->next();
                this->next();
            }
            else
            {
                struct TokenDef div_def;
                div_def.t = Division;
                div_def.start = position;
                div_def.token_str = get_str_token(Division);
                tokens.push_back(div_def);

                this->next();
            }
            break;
        }
        case ':':
        {
            if (next_char == ':')
            {
                struct TokenDef colon_def;
                colon_def.t = ScopeAccessor;
                colon_def.start = position;
                colon_def.token_str = get_str_token(ScopeAccessor);

                tokens.push_back(colon_def);

                this->next();
                this->next();
            }
            else if (next_char == '=')
            {
                struct TokenDef const_def;
                const_def.t = Constant;
                const_def.start = position;
                const_def.token_str = get_str_token(Constant);
                tokens.push_back(const_def);

                this->next();
                this->next();
            }
            else
            {
                this->next();
            }
            break;
        }
        case '.':
        {
            struct TokenDef dot_def;
            dot_def.t = Dot;
            dot_def.start = position;
            dot_def.token_str = get_str_token(Dot);
            tokens.push_back(dot_def);

            this->next();
            break;
        }
        case '"':
        {
            struct TokenDef quote_def;
            quote_def.t = Quote;
            quote_def.start = position;
            quote_def.token_str = get_str_token(Quote);
            tokens.push_back(quote_def);

            this->next();
            break;
        }
        case '@':
        {
            struct TokenDef at_def;
            at_def.t = Import;
            at_def.start = position;
            at_def.token_str = get_str_token(Import);
            tokens.push_back(at_def);

            this->next();
            break;
        }
        case '{':
        {
            struct TokenDef scope_start_def;
            scope_start_def.t = ScopeStart;
            scope_start_def.start = position;
            scope_start_def.token_str = get_str_token(ScopeStart);
            tokens.push_back(scope_start_def);

            this->next();
            break;
        }
        case '}':
        {
            struct TokenDef scope_end_def;
            scope_end_def.t = ScopeEnd;
            scope_end_def.start = position;
            scope_end_def.token_str = get_str_token(ScopeEnd);
            tokens.push_back(scope_end_def);

            this->next();
            break;
        }
        case '(':
        {
            struct TokenDef l_paren_def;
            l_paren_def.t = LParen;
            l_paren_def.start = position;
            l_paren_def.token_str = get_str_token(LParen);
            tokens.push_back(l_paren_def);

            this->next();
            break;
        }
        case ')':
        {
            struct TokenDef r_paren_def;
            r_paren_def.t = RParen;
            r_paren_def.start = position;
            r_paren_def.token_str = get_str_token(RParen);
            tokens.push_back(r_paren_def);

            this->next();
            break;
        }
        case '[':
        {
            struct TokenDef l_brace_def;
            l_brace_def.t = LBrace;
            l_brace_def.start = position;
            l_brace_def.token_str = get_str_token(LBrace);
            tokens.push_back(l_brace_def);

            this->next();
            break;
        }
        case ']':
        {
            struct TokenDef r_brace_def;
            r_brace_def.t = RBrace;
            r_brace_def.start = position;
            r_brace_def.token_str = get_str_token(RBrace);
            tokens.push_back(r_brace_def);

            this->next();
            break;
        }
        case '\n':
        {
            struct TokenDef linebreak_def;
            linebreak_def.t = LineBreak;
            linebreak_def.start = position;
            linebreak_def.token_str = get_str_token(LineBreak);
            tokens.push_back(linebreak_def);

            this->next();
            break;
        }
        case ',':
        {
            struct TokenDef comma_def;
            comma_def.t = Comma;
            comma_def.start = position;
            comma_def.token_str = get_str_token(Comma);
            tokens.push_back(comma_def);

            this->next();
            break;
        }
        case '=':
        {
            struct TokenDef assign_def;
            assign_def.t = Assignment;
            assign_def.start = position;
            assign_def.token_str = get_str_token(Assignment);
            tokens.push_back(assign_def);

            this->next();
            break;
        }
        default:
        {
            string text;

            bool flag = true;
            while (flag)
            {
                regex spcl("[$-/:-?{-~!\"^_`\\[\\]]");
                string curr_str = string(1, current_char);
                bool matched = regex_match(curr_str.begin(), curr_str.end(), spcl);
                if (matched)
                {
                    this->checkIfKeyword(text);
                    flag = false;
                }
                else
                {
                    text += current_char;
                    this->next();
                    int pos = this->check_next();
                    if (pos == -1)
                    {
                        this->checkIfKeyword(text);
                        flag = false;
                    }
                }
            }
        }
        }

        int pos = this->check_next();
        if (pos == -1)
        {
            keepTokenizing = false;
        }
    }
}

int Lexer::next()
{
    position++;
    if (position >= contents.length())
    {
        return -1;
    }

    current_char = contents[position];
    if (position + 1 != contents.length())
    {
        next_char = contents[position + 1];
    }

    return position;
}

int Lexer::check_next()
{
    int pos = position + 1;
    if (pos >= contents.length())
    {
        return -1;
    }

    return pos;
}

void Lexer::checkIfKeyword(string text)
{
    string trimmed = trim(text, "Cannot trim this string");

    if (trimmed.find_first_of(' ') != string::npos)
    {
        string word = "";
        for (auto letter : word)
        {
            if (letter == ' ')
            {
                this->addKeyword(word);
                word = "";
            }
            else
            {
                word += letter;
            }
        }
    }
    else
    {
        this->addKeyword(trimmed);
    }
}

void Lexer::addKeyword(string word)
{
    if (word == "if")
    {
        struct TokenDef if_def;
        if_def.t = If;
        if_def.start = position;
        if_def.token_str = get_str_token(If);
        tokens.push_back(if_def);
    }
    else if (word == "else")
    {
        struct TokenDef else_def;
        else_def.t = Else;
        else_def.start = position;
        else_def.token_str = get_str_token(Else);
        tokens.push_back(else_def);
    }
    else if (word == "elif")
    {
        struct TokenDef elif_def;
        elif_def.t = ElseIf;
        elif_def.start = position;
        elif_def.token_str = get_str_token(ElseIf);
        tokens.push_back(elif_def);
    }
    else if (word == "for")
    {
        struct TokenDef for_def;
        for_def.t = For;
        for_def.start = position;
        for_def.token_str = get_str_token(For);
        tokens.push_back(for_def);
    }
    else if (word == "Str")
    {
        struct TokenDef str_def;
        str_def.t = String;
        str_def.start = position;
        str_def.token_str = get_str_token(String);
        tokens.push_back(str_def);
    }
    else if (word == "Bool")
    {
        struct TokenDef bool_def;
        bool_def.t = Bool;
        bool_def.start = position;
        bool_def.token_str = get_str_token(Bool);
        tokens.push_back(bool_def);
    }
    else if (word == "Int")
    {
        struct TokenDef int_def;
        int_def.t = Int;
        int_def.start = position;
        int_def.token_str = get_str_token(Int);
        tokens.push_back(int_def);
    }
    else if (word == "Float")
    {
        struct TokenDef float_def;
        float_def.t = Float;
        float_def.start = position;
        float_def.token_str = get_str_token(Float);
        tokens.push_back(float_def);
    }
    else if (word == "Fn")
    {
        struct TokenDef fn_def;
        fn_def.t = Fn;
        fn_def.start = position;
        fn_def.token_str = get_str_token(Fn);
        tokens.push_back(fn_def);
    }
    else if (word == "true")
    {
        struct TokenDef true_def;
        true_def.t = BoolVal;
        true_def.start = position;
        true_def.token_str = get_str_token(BoolVal);
        tokens.push_back(true_def);
    }
    else if (word == "false")
    {
        struct TokenDef false_def;
        false_def.t = BoolVal;
        false_def.start = position;
        false_def.token_str = get_str_token(BoolVal);
        tokens.push_back(false_def);
    }
    else if (word == "and")
    {
        struct TokenDef and_def;
        and_def.t = And;
        and_def.start = position;
        and_def.token_str = get_str_token(And);
        tokens.push_back(and_def);
    }
    else if (word == "or")
    {
        struct TokenDef or_def;
        or_def.t = Or;
        or_def.start = position;
        or_def.token_str = get_str_token(Or);
        tokens.push_back(or_def);
    }
    else if (word == "is")
    {
        struct TokenDef is_def;
        is_def.t = BoolVal;
        is_def.start = position;
        is_def.token_str = get_str_token(BoolVal);
        tokens.push_back(is_def);
    }
    else if (word == "isnot")
    {
        struct TokenDef isnot_def;
        isnot_def.t = IsNot;
        isnot_def.start = position;
        isnot_def.token_str = get_str_token(IsNot);
        tokens.push_back(isnot_def);
    }
    else
    {
        struct TokenDef text_def;
        text_def.t = Text;
        text_def.start = position;
        text_def.token_str = get_str_token(Text);
        tokens.push_back(text_def);
    }
}

int main()
{
    Lexer lexer("main.qn");
    lexer.read_from_file();
    lexer.analyze();

    for (auto def : lexer.get_tokens())
    {
        cout << " - " << def.token_str << endl;
    }
    return 0;
}