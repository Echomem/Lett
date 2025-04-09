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
        {TokenType::INTEGER, "INTEGER"},
        {TokenType::HEX_INTEGER, "HEX_INTEGER"},
        {TokenType::OCTAL_INTEGER, "OCTAL_INTEGER"},
        {TokenType::BINARY_INTEGER, "BINARY_INTEGER"},
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

    const static std::unordered_map<char, TokenType> singleCharTokenType = {
        {'+',  TokenType::OP_ADD             },
        {'-',  TokenType::OP_SUB             },
        {'*',  TokenType::OP_MUL             },
        {'/',  TokenType::OP_DIV             },
        {'%',  TokenType::OP_MOD             }, 
        {'&',  TokenType::OP_BIT_AND         }, 
        {'|',  TokenType::OP_BIT_OR          }, 
        {'^',  TokenType::OP_BIT_NOT         }, 
        {'~',  TokenType::OP_BIT_XOR         }, 
        {'=',  TokenType::OP_ASSIGN          },
        {'>',  TokenType::OP_GREAT           },
        {'<',  TokenType::OP_LESS            },
        {'(',  TokenType::LEFT_PARENT        },
        {')',  TokenType::RIGHT_PARENT       },
        {'[',  TokenType::LEFT_BRACKET       },
        {']',  TokenType::RIGHT_BRACKET      },
        {'{',  TokenType::LEFT_BRACE         },
        {'}',  TokenType::RIGHT_BRACE        },
        {'.',  TokenType::DOT                },
        {',',  TokenType::COMMA              },
        {':',  TokenType::COLON              },
        {';',  TokenType::SEMI_COLON         },
    };

    const static std::unordered_map<std::string, TokenType> doubleCharTokenType = {
        {"++",  TokenType::OP_INC             },
        {"--",  TokenType::OP_DEC             },
        {"<<",  TokenType::OP_BIT_SHIFT_LEFT  },
        {">>",  TokenType::OP_BIT_SHIFT_RIGHT },
        {"+=",  TokenType::OP_ADD_ASSIGN      },
        {"-=",  TokenType::OP_SUB_ASSIGN      },
        {"*=",  TokenType::OP_MUL_ASSIGN      },
        {"/=",  TokenType::OP_DIV_ASSIGN      },
        {"&=",  TokenType::OP_BIT_AND_ASSIGN  },
        {"|=",  TokenType::OP_BIT_OR_ASSIGN   },
        {"==",  TokenType::OP_EQUAL           },
        {"!=",  TokenType::OP_NOT_EQUAL       },
        {">=",  TokenType::OP_GREAT_EQUAL     },
        {"<=",  TokenType::OP_LESS_EQUAL      },
        {"&&",  TokenType::OP_AND             },
        {"||",  TokenType::OP_OR              },
        {"::",  TokenType::DOUBLE_COLON       },
    };

    bool Token::isKeyWord(const std::string &str) {
        // TODO: 使用哈希表来查找关键字
        return false;
    }

    Token::Token(const char ch, size_t line, size_t column) {
        auto it = singleCharTokenType.find(ch);
        if (it != singleCharTokenType.end()) {
            _type = it->second;
        } else {
            _type = TokenType::UNKNOWN;
        }
        _value = std::string(1, ch);
        _line = line;
        _column = column;
    }

    Token::Token(const char ch, const char nextch, size_t line, size_t column) {
        std::string doubleChar(1, ch);
        doubleChar += nextch;
        auto it = doubleCharTokenType.find(doubleChar);
        if (it != doubleCharTokenType.end()) {
            _type = it->second;
        } else {
            _type = TokenType::UNKNOWN;
        }
        _value = doubleChar;
        _line = line;
        _column = column;
    }

    Token::Token(TokenType type, std::string value, size_t line, size_t column)
        :_type(type), _value(value), _line(line), _column(column) {
            // constructor.
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