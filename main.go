package main

import (
	"errors"
	"flag"
	"fmt"
	"io/ioutil"
	"regexp"
	"strings"
)

const (
	// Definition of a type in quen
	Definition Token = "def"
	// TypeOf is the operator to assign a type to a variable
	ScopeAccessor = "ScopeAccessor"
	Scope         = "Scope"
	ScopeStart    = "ScopeStart"
	ScopeEnd      = "ScopeEnd"
	LParen        = "LParen"
	RParen        = "RParen"
	LBrace        = "LBrace"
	RBrace        = "RBrace"
	Function      = "Function"
	Comment       = "Comment"
	Assignment    = "Assignment"
	Constant      = "Constant"
	Variable      = "Variable"
	Value         = "Value"
	Import        = "Import"
	Quote         = "Quote"
	Comma         = "Comma"
	Text          = "Text"
	Dot           = "Dot"
	LineBreak     = "LineBreak"
	For           = "For"
	If            = "If"
	Else          = "Else"
	ElseIf        = "ElseIf"
	BoolVal       = "Boolean Value"

	String = "String"
	Bool   = "Bool"
	Float  = "Float"
	Int    = "Int"
	Fn     = "Function"

	Addition       = "+"
	Subtraction    = "-"
	Multiplication = "*"
	Division       = "/"
	Modulo         = "%"

	And   = "and"
	Or    = "or"
	Is    = "is"
	IsNot = "isnot"
)

// Token is the token for quen that is lexed
type Token string

// TokenDef holds the definition instance of a token together
type TokenDef struct {
	T     Token
	Pos   int
	Value string
}

// Lexer analyzes the file contents of a quen program
type Lexer struct {
	position    int
	currentChar rune
	nextChar    rune
	contents    string
	tokens      []TokenDef
}

// Parser parses the lexed program into AST
type Parser struct {
	contents string
	tokens   []Token
}

func (p *Parser) parse() {

}

func regexpLettersMatch(some string) bool {
	match, _ := regexp.MatchString("\\w", some)
	matchSpace, _ := regexp.MatchString("\\s", some)
	return match || matchSpace
}

func (l *Lexer) next() int {
	l.position++
	if l.position >= len(l.contents) {
		return -1
	}

	l.currentChar = rune(l.contents[l.position])
	if l.position+1 != len(l.contents) {
		l.nextChar = rune(l.contents[l.position+1])
	}
	return l.position
}

func (l *Lexer) checkNext() int {
	pos := l.position + 1
	if pos > len(l.contents) {
		return -1
	}
	return l.position
}

func (l *Lexer) checkIfKeyword(text string) {
	target := strings.TrimSpace(text)
	if strings.Contains(target, " ") {
		for _, word := range strings.Split(text, " ") {
			l.addKeyword(word)
		}
	} else {
		l.addKeyword(target)
	}
}

func (l *Lexer) addKeyword(word string) {
	switch word {
	case "if":
		l.tokens = append(l.tokens, TokenDef{T: If, Pos: l.position, Value: word})
		break
	case "else":
		l.tokens = append(l.tokens, TokenDef{T: Else, Pos: l.position, Value: word})
		break
	case "elif":
		l.tokens = append(l.tokens, TokenDef{T: ElseIf, Pos: l.position, Value: word})
		break
	case "for":
		l.tokens = append(l.tokens, TokenDef{T: For, Pos: l.position, Value: word})
		break
	case "Str":
		l.tokens = append(l.tokens, TokenDef{T: String, Pos: l.position, Value: word})
		break
	case "Bool":
		l.tokens = append(l.tokens, TokenDef{T: Bool, Pos: l.position, Value: word})
		break
	case "Int":
		l.tokens = append(l.tokens, TokenDef{T: Int, Pos: l.position, Value: word})
		break
	case "Float":
		l.tokens = append(l.tokens, TokenDef{T: Float, Pos: l.position, Value: word})
		break
	case "Fn":
		l.tokens = append(l.tokens, TokenDef{T: Fn, Pos: l.position, Value: word})
		break
	case "true":
		l.tokens = append(l.tokens, TokenDef{T: BoolVal, Pos: l.position, Value: word})
		break
	case "false":
		l.tokens = append(l.tokens, TokenDef{T: BoolVal, Pos: l.position, Value: word})
		break
	case "and":
		l.tokens = append(l.tokens, TokenDef{T: And, Pos: l.position, Value: word})
		break
	case "or":
		l.tokens = append(l.tokens, TokenDef{T: Or, Pos: l.position, Value: word})
		break
	case "is":
		l.tokens = append(l.tokens, TokenDef{T: Is, Pos: l.position, Value: word})
		break
	case "isnot":
		l.tokens = append(l.tokens, TokenDef{T: IsNot, Pos: l.position, Value: word})
		break
	default:
		l.tokens = append(l.tokens, TokenDef{T: Text, Pos: l.position, Value: word})
	}
}

func (l *Lexer) nextUntilNewLine() {
	l.next()
	for {
		if l.currentChar != '\n' {
			l.next()
		} else {
			break
		}
	}
}

func (l *Lexer) readFromFile(path string) error {
	if !strings.HasSuffix(path, ".qn") {
		return errors.New("not a Quen script")
	}

	b, err := ioutil.ReadFile(path)
	if err != nil {
		return err
	}

	l.contents = strings.Trim(string(b), " ")
	l.currentChar = rune(l.contents[l.position])
	l.nextChar = rune(l.contents[l.position+1])
	return nil
}

func (l *Lexer) analyze() error {
	for {
		switch l.currentChar {
		case '/':
			if l.nextChar == '/' {
				l.tokens = append(l.tokens, TokenDef{T: Comment, Pos: l.position})
				l.next()
				l.next()
			} else {
				l.tokens = append(l.tokens, TokenDef{T: Division, Pos: l.position})
				l.next()
			}
			break
		case ':':
			if l.nextChar == ':' {
				l.tokens = append(l.tokens, TokenDef{T: ScopeAccessor, Pos: l.position})
				l.next()
				l.next()
			} else if l.nextChar == '=' {
				l.tokens = append(l.tokens, TokenDef{T: Constant, Pos: l.position})
				l.next()
				l.next()
			} else {
				l.next()
			}
			break
		case '.':
			l.tokens = append(l.tokens, TokenDef{T: Dot, Pos: l.position})
			l.next()
			break
		case '"':
			l.tokens = append(l.tokens, TokenDef{T: Quote, Pos: l.position})
			l.next()
			break
		case '@':
			l.tokens = append(l.tokens, TokenDef{T: Import, Pos: l.position})
			l.next()
			break
		case '{':
			l.tokens = append(l.tokens, TokenDef{T: ScopeStart, Pos: l.position})
			l.next()
			break
		case '}':
			l.tokens = append(l.tokens, TokenDef{T: ScopeEnd, Pos: l.position})
			l.next()
			break
		case '(':
			l.tokens = append(l.tokens, TokenDef{T: LParen, Pos: l.position})
			l.next()
			break
		case ')':
			l.tokens = append(l.tokens, TokenDef{T: RParen, Pos: l.position})
			l.next()
			break
		case '[':
			l.tokens = append(l.tokens, TokenDef{T: LBrace, Pos: l.position})
			l.next()
			break
		case ']':
			l.tokens = append(l.tokens, TokenDef{T: RBrace, Pos: l.position})
			l.next()
			break
		case '\n':
			l.tokens = append(l.tokens, TokenDef{T: LineBreak, Pos: l.position})
			l.next()
			break
		case ',':
			l.tokens = append(l.tokens, TokenDef{T: Comma, Pos: l.position})
			l.next()
			break
		case '=':
			l.tokens = append(l.tokens, TokenDef{T: Assignment, Pos: l.position})
			l.next()
			break
		default:
			text := ""
			for {
				spcl, _ := regexp.MatchString("[$-/:-?{-~!\"^_`\\[\\]]", string(l.currentChar))
				if spcl || l.currentChar == '\n' {
					l.checkIfKeyword(text)
					break
				}

				if !regexpLettersMatch(string(l.currentChar)) {
					l.checkIfKeyword(text)
					break
				} else {
					text += string(l.currentChar)
					l.next()
					pos := l.checkNext()
					if pos == -1 {
						l.checkIfKeyword(text)
						break
					}
				}

			}
			break
		}

		pos := l.checkNext()
		if pos == -1 {
			break
		}
	}

	return nil
}

func main() {
	flag.Parse()
	lexer := &Lexer{}
	err := lexer.readFromFile(flag.Arg(0))
	if err != nil {
		panic(err)
	}

	err = lexer.analyze()
	if err != nil {
		panic(err)
	}

	fmt.Printf("%+v\n", lexer.tokens)
}
