#ifndef __LETT_LEXER_TOKEN_H__
#define __LETT_LEXER_TOKEN_H__
#include <string>

namespace Lett {
    enum TokenType {
        KEYWORDS, 
        // 运算符
        PLUS,   // +
        MINUS,  // -
        MUL,    // *
        DIV,    // /
        MOD,    // %
        EQUAL,  // =
        LEFT_PARENT,    // (
        RIGHT_PARENT,   // )
        LEFT_BRACE,     // {
        RIGHT_BRACE,    // }
        COMMA,          // ,
        COLON,          // :
        SEM,            // ;
    };

    class Token {
    public:
        Token(TokenType type, std::string value);
        const char *c_str() const;
    private:
        TokenType _type;
        std::string _value;
    };
}

#endif // __LETT_LEXER_TOKEN_H__