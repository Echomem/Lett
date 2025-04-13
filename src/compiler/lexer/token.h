#ifndef __LETT_LEXER_TOKEN_H__
#define __LETT_LEXER_TOKEN_H__
#include <string>
#include <vector>
#include <unordered_set>
#include <map>

#define LETT_TKTP_BASIC \
        TKTP_MEMBER(IDENTIFIER, "")     \
        TKTP_MEMBER(BOOL, "")           \
        TKTP_MEMBER(KEYWORD, "")        \
        TKTP_MEMBER(STRING, "")         \
        TKTP_MEMBER(CHAR, "")           \
        TKTP_MEMBER(DEC_INTEGER, "")    \
        TKTP_MEMBER(HEX_INTEGER, "")    \
        TKTP_MEMBER(OCT_INTEGER, "")    \
        TKTP_MEMBER(BIN_INTEGER, "")    \
        TKTP_MEMBER(FLOAT, "")

#define LETT_TKTP_OPERATOR \
        TKTP_MEMBER(OP_ADD, "+")                \
        TKTP_MEMBER(OP_SUB, "-")                \
        TKTP_MEMBER(OP_MUL, "*")                \
        TKTP_MEMBER(OP_DIV, "/")                \
        TKTP_MEMBER(OP_MOD, "%")                \
        TKTP_MEMBER(OP_INC, "++")               \
        TKTP_MEMBER(OP_DEC, "--")               \
        TKTP_MEMBER(OP_BIT_AND, "&")            \
        TKTP_MEMBER(OP_BIT_OR, "|")             \
        TKTP_MEMBER(OP_BIT_NOT, "^")            \
        TKTP_MEMBER(OP_BIT_XOR, "~")            \
        TKTP_MEMBER(OP_BIT_SHIFT_LEFT, "<<")    \
        TKTP_MEMBER(OP_BIT_SHIFT_RIGHT, ">>")   \
        TKTP_MEMBER(OP_ASSIGN, "=")             \
        TKTP_MEMBER(OP_ADD_ASSIGN, "+=")        \
        TKTP_MEMBER(OP_SUB_ASSIGN, "-=")        \
        TKTP_MEMBER(OP_MUL_ASSIGN, "*=")        \
        TKTP_MEMBER(OP_DIV_ASSIGN, "/=")        \
        TKTP_MEMBER(OP_MOD_ASSIGN, "%=")        \
        TKTP_MEMBER(OP_BIT_AND_ASSIGN, "&=")    \
        TKTP_MEMBER(OP_BIT_OR_ASSIGN, "|=")     \
        TKTP_MEMBER(OP_EQUAL, "==")             \
        TKTP_MEMBER(OP_NOT_EQUAL, "!=")         \
        TKTP_MEMBER(OP_GREAT, ">")              \
        TKTP_MEMBER(OP_LESS, "<")               \
        TKTP_MEMBER(OP_GREAT_EQUAL, ">=")       \
        TKTP_MEMBER(OP_LESS_EQUAL, "<=")        \
        TKTP_MEMBER(OP_AND, "&&")               \
        TKTP_MEMBER(OP_OR, "||")                \
        TKTP_MEMBER(OP_NOT, "!")

#define LETT_TKTP_SEPERATOR \
        TKTP_MEMBER(LEFT_PARENT, "(")   \
        TKTP_MEMBER(RIGHT_PARENT, ")")  \
        TKTP_MEMBER(LEFT_BRACKET, "[")  \
        TKTP_MEMBER(RIGHT_BRACKET, "]") \
        TKTP_MEMBER(LEFT_BRACE, "{")    \
        TKTP_MEMBER(RIGHT_BRACE, "}")   \
        TKTP_MEMBER(DOT, ".")           \
        TKTP_MEMBER(COMMA, ",")         \
        TKTP_MEMBER(COLON, ":")         \
        TKTP_MEMBER(DOUBLE_COLON, "::") \
        TKTP_MEMBER(SEMI_COLON, ";")

#define LETT_KEYWORDS \
        KEYWORD_MEMBER(import) \
        KEYWORD_MEMBER(var) \
        KEYWORD_MEMBER(fn) \
        KEYWORD_MEMBER(main) \
        KEYWORD_MEMBER(return) \
        KEYWORD_MEMBER(while) \
        KEYWORD_MEMBER(do) \
        KEYWORD_MEMBER(for) \
        KEYWORD_MEMBER(if) \
        KEYWORD_MEMBER(elif) \
        KEYWORD_MEMBER(else) \
        KEYWORD_MEMBER(switch) \
        KEYWORD_MEMBER(case) \
        KEYWORD_MEMBER(default) \
        KEYWORD_MEMBER(break) \
        KEYWORD_MEMBER(continue) \
        KEYWORD_MEMBER(void) \
        KEYWORD_MEMBER(int) \
        KEYWORD_MEMBER(int8) \
        KEYWORD_MEMBER(int16) \
        KEYWORD_MEMBER(int32) \
        KEYWORD_MEMBER(int64) \
        KEYWORD_MEMBER(uint) \
        KEYWORD_MEMBER(uint8) \
        KEYWORD_MEMBER(uint16) \
        KEYWORD_MEMBER(uint32) \
        KEYWORD_MEMBER(uint64) \
        KEYWORD_MEMBER(float) \
        KEYWORD_MEMBER(float32) \
        KEYWORD_MEMBER(float64) \
        KEYWORD_MEMBER(char) \
        KEYWORD_MEMBER(string) \
        KEYWORD_MEMBER(bool) \
        KEYWORD_MEMBER(class) \
        KEYWORD_MEMBER(public) \
        KEYWORD_MEMBER(protected) \
        KEYWORD_MEMBER(private) \
        KEYWORD_MEMBER(interface) \
        KEYWORD_MEMBER(virtual) \
        KEYWORD_MEMBER(super) \
        KEYWORD_MEMBER(this) \
        KEYWORD_MEMBER(self) \
        KEYWORD_MEMBER(object)

#define LETT_BOOLEAN \
        BOOLEAN_MEMBER(true) \
        BOOLEAN_MEMBER(false)

namespace Lett {
    
    #define TKTP_MEMBER(m, s) m,
    enum class TokenType {
        LETT_TKTP_BASIC
        LETT_TKTP_OPERATOR
        LETT_TKTP_SEPERATOR
        UNKNOWN
    };
    #undef TKTP_MEMBER

    typedef struct TokenTypeItem {
        TokenType type;
        std::string value;
        TokenTypeItem(TokenType t, const std::string &v):type(t), value(v) {}
        TokenTypeItem(const TokenTypeItem &other):type(other.type), value(other.value){}
        TokenTypeItem& operator=(const TokenTypeItem& other);
        bool operator<(const TokenTypeItem& other) const;
    } TokenTypeItem;

    typedef std::vector<TokenTypeItem> TokenTypeItems;
    typedef std::map<TokenTypeItem, TokenTypeItems> TokenTypeTable;

    class Token {
    private:
        static const std::unordered_set<std::string> &_keywords;
        static const std::unordered_set<std::string> &_boolean;
        static const TokenTypeItems &_operators;
        static const TokenTypeItems &_seperators;
        static TokenTypeTable _table;
        static bool _is_keyword(std::string &value);
        static bool _is_boolean(std::string &value);
        
        TokenType _type;
        std::string _value;
        std::size_t _line;
        std::size_t _column;
    public:
        // type := IDENTIFIER: 
        // 构造函数会根据`value`值查表自动转换KEYWORD或BOOL类型
        Token(
            TokenType type, 
            const std::string &value,
            std::size_t line,
            std::size_t column
        );

        TokenType type() const { return _type; }
        const char *value() const { return _value.c_str(); }
        std::size_t line() const { return _line; }
        std::size_t column() const { return _column; }

        std::string string() const;

        static const char *getTypeName(TokenType type);
        static const TokenTypeTable& getTokenTable();
    };

}   // namespace Lett.

#endif // __LETT_LEXER_TOKEN_H__