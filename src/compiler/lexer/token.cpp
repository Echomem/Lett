#include <sstream>
#include "token.h"

namespace Lett {

    Token::Token(TokenType type, std::string value)
        :_type(type), _value(value) {
            // constructor.
    }

    const char *Token::c_str() const {
        std::ostringstream oss;
        oss << "(type:" << _type << " value:" << _value;
        return oss.str().c_str();
    }
} // namespace Lett