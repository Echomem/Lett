#include <sstream>
#include "token.h"

namespace Lett {

    TokenTypeItem& TokenTypeItem::operator=(const TokenTypeItem& other) {
        if (this != &other) {
            type = other.type;
            value = other.value;
        }
        return *this;
    }

    bool TokenTypeItem::operator<(const TokenTypeItem& other) const {
        if (type != other.type) {
            return static_cast<int>(type) < static_cast<int>(other.type);
        }
        return false;
    }
    
    #define KEYWORD_MEMBER(k) #k,
    const std::unordered_set<std::string> &Token::_keywords = {
        LETT_KEYWORDS
    };
    #undef KEYWORD_MEMBER

    #define BOOLEAN_MEMBER(b) #b,
    const std::unordered_set<std::string> &Token::_boolean = {
        LETT_BOOLEAN
    };
    #undef BOOLEAN_MEMBER

    #define TKTP_MEMBER(m, s) \
            TokenTypeItem{TokenType::m, std::string(s)},
    const TokenTypeItems &Token::_operators = {
            LETT_TKTP_OPERATOR
    };

    const TokenTypeItems &Token::_seperators = {
            LETT_TKTP_SEPERATOR
    };
    #undef TKTP_MEMBER

    #define TKTP_MEMBER(m, s) \
            case TokenType::m: return #m;
    const char *Token::getTypeName(TokenType type) {
        switch (type) {
            LETT_TKTP_BASIC
            LETT_TKTP_OPERATOR
            LETT_TKTP_SEPERATOR
            default:
                return "UNKOWN";
        }
    }
    #undef TKTP_MEMBER

    TokenTypeTable Token::_table;

    const TokenTypeTable &Token::getTokenTable() {
        if (Token::_table.empty()) {
            // 初始化_table.
            TokenTypeItems symbol_items;
            for (const TokenTypeItem &it: Token::_operators) {
                // symbol_items连接Token::_operators
                symbol_items.emplace_back(it);
            }

            for (const TokenTypeItem &it: Token::_seperators) {
                // symbol_items连接Token::_seperators
                symbol_items.emplace_back(it);
            }

            for (const TokenTypeItem &it: symbol_items) {
                if (it.value.length()==1) {
                    // 单字符操作符
                    Token::_table.emplace(it, TokenTypeItems());
                }
            }

            for (const TokenTypeItem &dit: symbol_items) {
                if (dit.value.length()==2) {
                    // 双字符符号
                    for (auto &pair :Token::_table) {
                        if (dit.value.substr(0,1)==pair.first.value) {
                            // 双字符符号的第一个字符与表中第一项匹配
                            pair.second.emplace_back(dit);
                        }
                    }
                }
            }
        }
        return _table;
    }

    bool Token::_is_keyword(std::string &value) {
        auto it = Token::_keywords.find(value);
        if (it != Token::_keywords.end()) {
            return true;
        }
        return false;
    }

    bool Token::_is_boolean(std::string &value) {
        auto it = Token::_boolean.find(value);
        if (it != Token::_boolean.end()) {
            return true;
        }
        return false;
    }

    Token::Token(TokenType type, const std::string &value, std::size_t line, std::size_t column)
        :_type(type), _value(value), _line(line), _column(column) {
        // IDENTIFIER类型的Token自动查表，确定是否为KEYWORD或者BOOL
        if (type == TokenType::IDENTIFIER) {
            if (Token::_is_boolean(_value)) {
                _type = TokenType::BOOL;
            } else if (Token::_is_keyword(_value)) {
                _type = TokenType::KEYWORD;
            } else {
                _type = TokenType::IDENTIFIER;
            }
        }
    }

    std::string Token::string() const {
        std::ostringstream oss;
        oss << "[" << Token::getTypeName(_type) << ", " << _value << ", " << _line << ":" << _column << "]";
        return oss.str();
    }
}   // namespace Lett
