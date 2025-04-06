#ifndef __LETT_LEXER_TOKEN_H__
#define __LETT_LEXER_TOKEN_H__
#include <string>
#include <map>
#include "reader.h"

namespace Lett {
    // Token类型枚举，定义了所有可能的词法单元类型
    enum class TokenType {
        IDENTIFIER,         // 标识符，如变量名、函数名等
        BOOL,               // 布尔字面量，如 true, false
        KEYWORD,            // 关键字，如 fn, class, return 等
        STRING,             // 字符串字面量，如 "hello"
        CHAR,               // 字符字面量，如 'c'
        INTEGER,            // 十进制整数字面量，如 42
        HEX_INTEGER,        // 十六进制数字面量，如 0x2A
        OCTAL_INTEGER,      // 八进制数字面量，如 0o52
        BINARY_INTEGER,     // 二进制数字面量，如 0b101010
        FLOAT,              // 浮点数字面量，如 3.14
        /* 算术运算符       */
        OP_ADD,             // +
        OP_SUB,             // -
        OP_MUL,             // *
        OP_DIV,             // /
        OP_MOD,             // %
        OP_INC,             // ++
        OP_DEC,             // --
        /* 比特运算符       */
        OP_BIT_AND,         // &
        OP_BIT_OR,          // |
        OP_BIT_NOT,         // ^
        OP_BIT_XOR,         // ~
        OP_BIT_SHIFT_LEFT,  // <<
        OP_BIT_SHIFT_RIGHT, // >>
        /* 赋值运算符        */
        OP_ASSIGN,          // =
        OP_ADD_ASSIGN,      // +=
        OP_SUB_ASSIGN,      // -=
        OP_MUL_ASSIGN,      // *=
        OP_DIV_ASSIGN,      // /=
        OP_BIT_AND_ASSIGN,  // &=
        OP_BIT_OR_ASSIGN,   // |=
        /* 比较运算符        */
        OP_EQUAL,           // ==
        OP_NOT_EQUAL,       // !=
        OP_GREAT,           // >
        OP_LESS,            // <
        OP_GREAT_EQUAL,     // >=
        OP_LESS_EQUAL,      // <=
        /* 逻辑运算符        */
        OP_AND,             // &&
        OP_OR,              // ||
        /* 以下是分割符         */
        LEFT_PARENT,        // (
        RIGHT_PARENT,       // )
        LEFT_BRACKET,       // [
        RIGHT_BRACKET,      // ]
        LEFT_BRACE,         // {
        RIGHT_BRACE,        // }
        DOT,                // .
        COMMA,              // ,
        COLON,              // :
        DOUBLE_COLON,       // ::
        SEMI_COLON,         // ;
        UNKNOWN             // 未知类型
    };

    class Token {
    public:
        // 尝试从单字符构造Token,如果失败则返回UNKNOWN类型
        Token(const char ch, size_t line, size_t column); 
        // 尝试从单字符构造Token,如果失败则返回UNKNOWN类型
        Token(const char ch, const char nextch, size_t line, size_t column);
        // 从外界接受的Token类型和字符串构造Token
        Token(TokenType type, std::string value, size_t line, size_t column);

        TokenType type() const { return _type; } // 获取Token类型
        std::string value() const { return _value; } // 获取Token值
        const char *c_str() const; // 转换为字符串
        std::string toString() const { return std::string(c_str()); } // 转换为字符串
    private:
        TokenType _type;
        std::string _value;
        size_t _line;   // 行号
        size_t _column; // 列号
    };
}

#endif // __LETT_LEXER_TOKEN_H__