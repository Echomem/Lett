#include <sstream>
#include <unordered_map>
#include "token.h"

namespace Lett {

    const std::unordered_map<TokenType, std::string> tokenTypeToStringMap = {
        {TokenType::IDENTIFIER, "IDENTIFIER"},
        {TokenType::BOOL, "BOOL"},
        {TokenType::KEYWORD, "KEYWORD"},
        {TokenType::STRING, "STRING"},
        {TokenType::CHAR, "CHAR"},
        {TokenType::DEC_INTEGER, "DEC_INTEGER"},
        {TokenType::HEX_INTEGER, "HEX_INTEGER"},
        {TokenType::OCT_INTEGER, "OCT_INTEGER"},
        {TokenType::BIN_INTEGER, "BIN_INTEGER"},
        {TokenType::FLOAT, "FLOAT"},
        {TokenType::OP_ADD, "OP_ADD"},
        {TokenType::OP_SUB, "OP_SUB"},
        {TokenType::OP_MUL, "OP_MUL"},
        {TokenType::OP_DIV, "OP_DIV"},
        {TokenType::OP_MOD, "OP_MOD"},
        {TokenType::OP_INC, "OP_INC"},
        {TokenType::OP_DEC, "OP_DEC"},
        {TokenType::OP_BIT_AND, "OP_BIT_AND"},
        {TokenType::OP_BIT_OR, "OP_BIT_OR"},
        {TokenType::OP_BIT_NOT, "OP_BIT_NOT"},
        {TokenType::OP_BIT_XOR, "OP_BIT_XOR"},
        {TokenType::OP_BIT_SHIFT_LEFT, "OP_BIT_SHIFT_LEFT"},
        {TokenType::OP_BIT_SHIFT_RIGHT, "OP_BIT_SHIFT_RIGHT"},
        {TokenType::OP_ASSIGN, "OP_ASSIGN"},
        {TokenType::OP_ADD_ASSIGN, "OP_ADD_ASSIGN"},
        {TokenType::OP_SUB_ASSIGN, "OP_SUB_ASSIGN"},
        {TokenType::OP_MUL_ASSIGN, "OP_MUL_ASSIGN"},
        {TokenType::OP_DIV_ASSIGN, "OP_DIV_ASSIGN"},
        {TokenType::OP_MOD_ASSIGN, "OP_MOD_ASSIGN"},
        {TokenType::OP_BIT_AND_ASSIGN, "OP_BIT_AND_ASSIGN"},
        {TokenType::OP_BIT_OR_ASSIGN, "OP_BIT_OR_ASSIGN"},
        {TokenType::OP_EQUAL, "OP_EQUAL"},
        {TokenType::OP_NOT_EQUAL, "OP_NOT_EQUAL"},
        {TokenType::OP_GREAT, "OP_GREAT"},
        {TokenType::OP_LESS, "OP_LESS"},
        {TokenType::OP_GREAT_EQUAL, "OP_GREAT_EQUAL"},
        {TokenType::OP_LESS_EQUAL, "OP_LESS_EQUAL"},
        {TokenType::OP_AND, "OP_AND"},
        {TokenType::OP_OR, "OP_OR"},
        {TokenType::LEFT_PARENT, "LEFT_PARENT"},
        {TokenType::RIGHT_PARENT, "RIGHT_PARENT"},
        {TokenType::LEFT_BRACKET, "LEFT_BRACKET"},
        {TokenType::RIGHT_BRACKET, "RIGHT_BRACKET"},
        {TokenType::LEFT_BRACE, "LEFT_BRACE"},
        {TokenType::RIGHT_BRACE, "RIGHT_BRACE"},
        {TokenType::DOT, "DOT"},
        {TokenType::COMMA, "COMMA"},
        {TokenType::COLON, "COLON"},
        {TokenType::DOUBLE_COLON, "DOUBLE_COLON"},
        {TokenType::SEMI_COLON, "SEMI_COLON"},
        {TokenType::UNKNOWN, "UNKNOWN"}
    };

    const std::unordered_set<std::string> Token::_keyWords = {
        "import",   "var",      "fn",   "return",   "main",
        "while",    "do",       "for",  "if",       "elif",
        "else",     "switch",   "case", "default",  "break",
        "continue", "void",     "int",  "int8",     "int16",
        "int32",    "int64",    "uint", "uint8",    "uint16",
        "uint32",   "uint64",   "float","float32",  "float64",
        "char",     "string",   "bool", "class",    "public",
        "protected","private", "interface", "virtual", "super",
        "this",     "self",     "object"
    };

    bool Token::isKeyWord(const std::string &str) {
        // TODO: 使用哈希表来查找关键字
        auto it = _keyWords.find(str);
        if (it != _keyWords.end()) {
            return true;
        }
        return false;
    }

    Token::Token(TokenType type, const std::string value, size_t line, size_t column)
        :_type(type), _value(value), _line(line), _column(column) {
            // constructor.
            if (type == TokenType::IDENTIFIER) {
                if (value.compare("true")==0 || value.compare("false")==0) {
                    _type = TokenType::BOOL;
                } else if (Token::isKeyWord(value)) {
                    _type = TokenType::KEYWORD;
                } else {
                    _type = TokenType::IDENTIFIER;
                }
            }
    }

    const char *Token::c_str() const {
        std::ostringstream oss;
        std::string typeStr;
        auto it = tokenTypeToStringMap.find(_type);
        if (it != tokenTypeToStringMap.end()) {
            typeStr = it->second;
        } else {
            typeStr = "UNKNOWN";
        }
        oss << "(" << typeStr << "," << _value << "," << _line << "," << _column << ")";
        return oss.str().c_str();
    }
} // namespace Lett