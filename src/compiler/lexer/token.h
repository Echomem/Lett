#ifndef __LETT_LEXER_TOKEN_H__
#define __LETT_LEXER_TOKEN_H__
#include <string>
#include <mutex>
#include <unordered_set>
#include <unordered_map>
#include "reader.h"

namespace Lett {
    // Token类型枚举，定义了所有可能的词法单元类型
    enum class TokenType {
        // Basic types
        IDENTIFIER, 
        BOOL,               // true/false
        KEYWORD,            // keyword
        STRING,             // "string"
        CHAR,               // 'c'
        DEC_INTEGER,        // 10
        HEX_INTEGER,        // 0xFF
        OCT_INTEGER,        // 0o76
        BIN_INTEGER,        // 0b1101
        FLOAT,              // 3.14
        // Operators
        OP_ADD,             // +
        OP_SUB,             // -
        OP_MUL,             // *
        OP_DIV,             // /
        OP_MOD,             // %
        OP_INC,             // ++
        OP_DEC,             // --
        OP_BIT_AND,         // &
        OP_BIT_OR,          // |
        OP_BIT_NOT,         // ^
        OP_BIT_XOR,         // ~
        OP_BIT_SHIFT_LEFT,  // <<
        OP_BIT_SHIFT_RIGHT, // >>
        OP_ASSIGN,          // =
        OP_ADD_ASSIGN,      // +=
        OP_SUB_ASSIGN,      // -=
        OP_MUL_ASSIGN,      // *=
        OP_DIV_ASSIGN,      // /=
        OP_MOD_ASSIGN,      // %=
        OP_BIT_AND_ASSIGN,  // &=
        OP_BIT_OR_ASSIGN,   // |=
        OP_EQUAL,           // ==
        OP_NOT_EQUAL,       // !=
        OP_GREAT,           // >
        OP_LESS,            // <
        OP_GREAT_EQUAL,     // >=
        OP_LESS_EQUAL,      // <=
        OP_AND,             // &&
        OP_OR,              // ||
        OP_NOT,             // !
        // Seperators
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
        UNKNOWN
    };

    // Symbols, include operators and seperators
    class Symbol {
    private:
        TokenType _type;
        std::string _value;
    public:
        Symbol(TokenType type, const std::string &value);
        Symbol(const Symbol& symbol); 
        Symbol& operator=(const Symbol& symbol);
        // 重载 < 运算符作为成员函数
        bool operator<(const Symbol& other) const;

        TokenType type() const { return _type; }
        std::string value() const { return _value; }
    };

    class SymbolTable {
    private:
        // 构造函数私有化，防止外部实例化
        SymbolTable();
        // 拷贝构造函数私有化，防止拷贝
        SymbolTable(const SymbolTable&) = delete; 
        // 赋值运算符私有化，防止赋值
        SymbolTable& operator=(const SymbolTable&) = delete;
        // 静态互斥锁，用于线程安全
        static std::mutex _mutex;
        // 静态成员变量，保存单例实例
        static SymbolTable *_instance;

        std::vector<Symbol> _symbols; // 符号表
        // 添加符号到符号表
        void addSymbol(TokenType type, const std::string &value);
    public:
        // 获取单例实例
        static SymbolTable& getInstance();
        // 根据类型获取符号
        Symbol getSymbol(TokenType type) const;
        const std::vector<Symbol> &getSymbols() const { return _symbols; }
    };

    class Token {
    public:
        // 判断字符串是否是关键字
        static bool isKeyWord(const std::string &str);
        // 获取类型的名称 
        static const char *getTypeName(TokenType type);

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
        const static std::unordered_map<TokenType, std::string> _tokenTypeToStringMap;
        TokenType _type;
        std::string _value;
        std::size_t _line, _column; // 行列号
    };
}

#endif // __LETT_LEXER_TOKEN_H__