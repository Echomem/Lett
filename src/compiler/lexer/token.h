#ifndef __LETT_LEXER_TOKEN_H__
#define __LETT_LEXER_TOKEN_H__
#include <string>
#include <map>
#include <unordered_set>
#include "reader.h"

namespace Lett {
    // Token类型枚举，定义了所有可能的词法单元类型
    enum class TokenType {
        IDENTIFIER,         // 标识符
        BOOL,               // 布尔字面量
        KEYWORD,            // 关键字
        STRING,             // 字符串字面量
        CHAR,               // 字符字面量
        DEC_INTEGER,        // 十进制整数字面量
        HEX_INTEGER,        // 十六进制数字面量
        OCT_INTEGER,      // 八进制数字面量
        BIN_INTEGER,     // 二进制数字面量
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
        OP_MOD_ASSIGN,      // %=
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
        // 判断字符串是否是关键字
        static bool isKeyWord(const std::string &str); 

        // 构造Token
        Token(
                TokenType type,     // 当Token类型为IDENTIFIER时，自动根据value的值来转换为KEYWORD或BOOL
                const std::string value, 
                std::size_t line, 
                std::size_t column
        );

        // 获取Token类型
        TokenType type() const { return _type; }
    
        // 获取Token值
        std::string value() const { return _value; } 

        // 获取行或列号
        std::size_t line() const { return _line; }
        std::size_t column() const { return _column; }

        // 转换为字符串
        const char *c_str() const; 
        std::string toString() const { return std::string(c_str()); }
    private:
        const static std::unordered_set<std::string> _keyWords; // 关键字集合
        TokenType _type;
        std::string _value;
        std::size_t _line, _column; // 行列号
    };
}

#endif // __LETT_LEXER_TOKEN_H__