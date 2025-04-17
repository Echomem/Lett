#include <iostream>
#include "common.h"
#include "lexer.h"

namespace Lett {
    #define TKTP_MEMBER(m, s) {LexerState::m, TokenType::m},
    const std::unordered_map<LexerState, TokenType>& LexicalAnalyzer::_final_state_tktp_map = {
        {LexerState::ZERO,  TokenType::DEC_INTEGER},
        LETT_TKTP_BASIC
        LETT_TKTP_OPERATOR
        LETT_TKTP_SEPERATOR
    };
    #undef INSERT_STATE_TO_TOKEN_TYPE_MAP

    LexerState LexicalAnalyzer::_get_final_state(TokenType type) {
        // 根据Token类型返回对应的LexerState
        for (const auto& pair : LexicalAnalyzer::_final_state_tktp_map) {
            if (pair.second == type) {
                return pair.first;
            }
        }
        return LexerState::ERROR;
    }

    /* 
     *LexcialAnalyzer单例模式的实现    
     */
    LexicalAnalyzer* LexicalAnalyzer::_instance = nullptr;
    std::mutex LexicalAnalyzer::_mutex;

    LexicalAnalyzer& LexicalAnalyzer::getInstance(Reader *rd) {
        std::lock_guard<std::mutex> lock(_mutex);
        if (_instance == nullptr) {
            _instance = new LexicalAnalyzer();
            _instance->_set_reader(rd);
        } else {
            _instance->_set_reader(rd); // 设置新的Reader对象
        }
        return *_instance;
    }

    LexicalAnalyzer::LexicalAnalyzer() 
        : _reader(nullptr), _state(LexerState::READY), _lexer_used_chars(LEXER_USED_CHARS) 
    {
        // 初始化词法分析器
        _init_state_table(); // 初始化状态转移表
        // 以下手工修改初始的状态转移表
        _install_state_transition();
    }

    void LexicalAnalyzer::_set_reader(Reader *rd) {
        if (rd==nullptr) {
            throw InvalidArgument("rd", "Reader pointer is null.");
        }
        _reader = rd;
        _state = LexerState::READY; // 重置状态为就绪状态
        _tokens.clear();
    }

    /* 
     *状态转移表相关的实现
     */
    // 设置状态转换表中的转换关系
    // 当在statrtState状态时，将输入符合charList的字符，全部转换为endState
    void LexicalAnalyzer::_setup_state_transform(LexerState startState, LexerState endState, const char *charList){
        size_t i = static_cast<size_t>(startState);
        for (size_t j=0; j<_lexer_used_chars.length(); j++) {
            for (char ch:std::string(charList)) {
                if (ch == _lexer_used_chars[j]) {
                    _state_table[i][j] = endState;
                }
            }
        }
    }

    // 设置默认状态转移
    // 当在initState时，将其它未设置的状态全部设置为defaultState
    void LexicalAnalyzer::_setup_default_state_transform(LexerState initState, LexerState defaultState) {
        size_t i = static_cast<size_t>(initState);
        for (size_t j=0; j<LEXER_CHARSET_SIZE; j++) {
            if (_state_table[i][j] != defaultState) {
                _state_table[i][j] = defaultState;
            }
        }
    }

     void LexicalAnalyzer::_init_state_table() {
        size_t lexer_state_size = static_cast<size_t>(LexerState::ERROR) + 1;
        for(size_t i=0; i<lexer_state_size; ++i) {
            for(size_t j=0; j<LEXER_CHARSET_SIZE; ++j) {
                _state_table[i][j] = LexerState::ERROR; // 初始化为错误状态
            }
        }
    }

    void LexicalAnalyzer::_install_state_transition() {
        // Setup Ready
        _setup_state_transform(LexerState::READY, LexerState::READY, " \t\n");
        _setup_state_transform(LexerState::READY, LexerState::ZERO, "0");
        _setup_state_transform(LexerState::READY, LexerState::DEC_INTEGER, "123456789");
        _setup_state_transform(LexerState::READY, LexerState::_CHAR_S, "'");
        _setup_state_transform(LexerState::READY, LexerState::_STRING, "\"");
        _setup_state_transform(LexerState::READY, LexerState::IDENTIFIER, IDENT_START);
        // Ready -> Symbol...

        // setup Ident
        _setup_default_state_transform(LexerState::IDENTIFIER, LexerState::READY);
        _setup_state_transform(LexerState::IDENTIFIER, LexerState::IDENTIFIER, IDENT_CHARS);
        
        // setup String
        _setup_default_state_transform(LexerState::_STRING, LexerState::_STRING);
        _setup_state_transform(LexerState::_STRING, LexerState::ERROR, "\n");
        _setup_state_transform(LexerState::_STRING, LexerState::_ESC_STRING, "\\");
        _setup_state_transform(LexerState::_STRING, LexerState::STRING, "\"");
        _setup_default_state_transform(LexerState::_ESC_STRING, LexerState::ERROR);
        _setup_state_transform(LexerState::_ESC_STRING, LexerState::_STRING, "abfnrtv\"\\");
        _setup_default_state_transform(LexerState::STRING, LexerState::READY);

        // setup Char
        _setup_default_state_transform(LexerState::_CHAR_S, LexerState::_CHAR);
        _setup_state_transform(LexerState::_CHAR_S, LexerState::_ESC_CHAR, "\\");
        _setup_state_transform(LexerState::_CHAR_S, LexerState::ERROR, "\n\t");
        _setup_default_state_transform(LexerState::_CHAR, LexerState::ERROR);
        _setup_state_transform(LexerState::_CHAR, LexerState::CHAR, "'");
        _setup_default_state_transform(LexerState::CHAR, LexerState::READY);
        _setup_default_state_transform(LexerState::_ESC_CHAR, LexerState::ERROR);
        _setup_state_transform(LexerState::_ESC_CHAR, LexerState::_CHAR, "abfnrtv'\\");

        // TODO: setup Number
        _setup_state_transform(LexerState::ZERO, LexerState::DEC_INTEGER, DEC_CHARS);
        _setup_state_transform(LexerState::ZERO, LexerState::_HEX_, "xX");
        _setup_state_transform(LexerState::ZERO, LexerState::_OCT_, "oO");
        _setup_state_transform(LexerState::ZERO, LexerState::_BIN_, "bB");
        _setup_state_transform(LexerState::ZERO, LexerState::FLOAT, ".");
        _setup_state_transform(LexerState::ZERO, LexerState::READY, NUMBER_SEPERATOR);
        _setup_state_transform(LexerState::_HEX_, LexerState::HEX_INTEGER, HEX_CHARS);
        _setup_state_transform(LexerState::_OCT_, LexerState::OCT_INTEGER, OCT_CHARS);
        _setup_state_transform(LexerState::_BIN_, LexerState::BIN_INTEGER, BIN_CHARS);
        _setup_state_transform(LexerState::HEX_INTEGER, LexerState::HEX_INTEGER, HEX_CHARS);
        _setup_state_transform(LexerState::HEX_INTEGER, LexerState::READY, NUMBER_SEPERATOR);
        _setup_state_transform(LexerState::OCT_INTEGER, LexerState::OCT_INTEGER, OCT_CHARS);
        _setup_state_transform(LexerState::OCT_INTEGER, LexerState::READY, NUMBER_SEPERATOR);
        _setup_state_transform(LexerState::BIN_INTEGER, LexerState::BIN_INTEGER, BIN_CHARS);
        _setup_state_transform(LexerState::BIN_INTEGER, LexerState::READY, NUMBER_SEPERATOR);
        _setup_state_transform(LexerState::DEC_INTEGER, LexerState::DEC_INTEGER, DEC_CHARS);
        _setup_state_transform(LexerState::DEC_INTEGER, LexerState::FLOAT, ".");
        _setup_state_transform(LexerState::DEC_INTEGER, LexerState::READY, NUMBER_SEPERATOR);
        _setup_state_transform(LexerState::FLOAT, LexerState::FLOAT, DEC_CHARS);
        _setup_state_transform(LexerState::FLOAT, LexerState::READY, NUMBER_SEPERATOR);

        // TODO: setup Symbols
        for (const auto& pair : Token::getTokenTable()) {
            if (pair.second.size() == 0) {
                // 处理单字符的符号
                _setup_state_transform(LexerState::READY, LexicalAnalyzer::_get_final_state(pair.first.type), pair.first.value.c_str());
                _setup_default_state_transform(LexicalAnalyzer::_get_final_state(pair.first.type), LexerState::READY);
            } else {
                // 处理双字符的符号
                _setup_default_state_transform(LexicalAnalyzer::_get_final_state(pair.first.type), LexerState::READY);
                _setup_state_transform(LexerState::READY, LexicalAnalyzer::_get_final_state(pair.first.type), pair.first.value.c_str());
                for (const auto& symbol : pair.second) {
                    _setup_state_transform(LexicalAnalyzer::_get_final_state(pair.first.type), LexicalAnalyzer::_get_final_state(symbol.type), symbol.value.substr(1,2).c_str());
                    _setup_default_state_transform(LexicalAnalyzer::_get_final_state(symbol.type), LexerState::READY);
                }
            }
        }
        // 处理单行注释和多行注释
        _setup_state_transform(LexerState::OP_DIV, LexerState::SINGLINE_COMMENT, "/");
        _setup_state_transform(LexerState::OP_DIV, LexerState::_MUILTLINE_COMMENT, "*");

        _setup_default_state_transform(LexerState::SINGLINE_COMMENT, LexerState::SINGLINE_COMMENT);
        _setup_state_transform(LexerState::SINGLINE_COMMENT, LexerState::READY, "\n");

        _setup_default_state_transform(LexerState::_MUILTLINE_COMMENT, LexerState::_MUILTLINE_COMMENT);
        _setup_state_transform(LexerState::_MUILTLINE_COMMENT, LexerState::_MUILTLINE_COMMENT_E, "*");
        _setup_default_state_transform(LexerState::_MUILTLINE_COMMENT_E, LexerState::_MUILTLINE_COMMENT);
        _setup_state_transform(LexerState::_MUILTLINE_COMMENT_E, LexerState::MUILTLINE_COMMENT, "/");
        _setup_default_state_transform(LexerState::MUILTLINE_COMMENT, LexerState::READY);
    }

    LexerState LexicalAnalyzer::_get_next_state(char ch) {
        // 根据当前状态state,查找_state_table表确定下一个状态。
        // 未找到，返回LexerState::ERROR
        size_t i = static_cast<size_t>(_state);
        size_t len = _lexer_used_chars.length();
        if(static_cast<unsigned short>(ch) > 128) {
            // Unicode字符虚拟为可见字符集后的第一项
            return _state_table[i][UNICODE_CHAR_INDEX];
        } else {
            // 在可见字符集中查找
            for (size_t j=0; j<len; j++) {
                if(_lexer_used_chars[j] == ch) {
                    return _state_table[i][j];
                }
            }
        }
        return LexerState::ERROR;
    }

    /*
     * 词法分析主要实现函数
     */
    TokenType LexicalAnalyzer::_get_token_type() {
        // 根据状态返回Token类型
        TokenType token_type = TokenType::UNKNOWN;
        auto it = LexicalAnalyzer::_final_state_tktp_map.find(_state);
        if (it != LexicalAnalyzer::_final_state_tktp_map.end()) {
            token_type = it->second;
        }
        return token_type; 
    }

    void LexicalAnalyzer::_handle_error(std::string &value, std::size_t line, std::size_t column) {
        // 错误处理，在错误状态下尝试继续读取直到读取到下一个分隔符为止
        char ch;
        if (_state==LexerState::_ESC_STRING || _state==LexerState::_STRING) {
            while(_reader->read(ch)) {
                // 读取直到读取到String结束符
                if (ch=='"') {
                    break;
                }
                value += ch;
            }
        } else if (_state==LexerState::_CHAR || _state==LexerState::_CHAR_S || _state==LexerState::_ESC_CHAR) {
            while(_reader->read(ch)) {
                // 读取直到读取到Char结束符
                if (ch=='\'') {
                    break;
                }
                value += ch;
            }
        } else {
            while(_reader->read(ch)) {
                if (ch==' ' || ch=='\t' || ch=='\n') {
                    break;
                }
                value += ch;
            }
        }
        _tokens.push_back(Token(TokenType::UNKNOWN,value, line, column));
        _state = LexerState::READY;
    }

    void LexicalAnalyzer::analyze() {
        // 读取源代码流，分析词法单元
        char ch;
        std::string value;
        size_t line = 1, column = 1; // 行号和列号
        while(true) {
            if (_state == LexerState::READY) {
                // 当前在就绪状态
                if (_reader->read(ch)) {
                    LexerState next_state = _get_next_state(ch);
                    if (next_state == LexerState::READY) {
                        // Ready -> Ready
                        continue;
                    }
                    // Ready -> Error | Other
                    value.clear();
                    value += ch;
                    line = _reader->line();
                    column = _reader->column();
                    _state = next_state;
                } else {
                    break; // 读取到文件结束符，退出循环
                }
            } else if (_state == LexerState::ERROR) {
                // 当前在错误状态，处理错误
                _handle_error(value, line, column);
            } else {
                // 当前在其他状态
                char next_ch;
                if (_reader->peek(next_ch)) {
                    LexerState next_state = _get_next_state(next_ch);
                    if (next_state == LexerState::READY) {
                        // 如果是单行或多行注释状态，则丢弃不处理
                        if (!(_state == LexerState::SINGLINE_COMMENT || _state == LexerState::MUILTLINE_COMMENT)) {
                            TokenType type = _get_token_type();
                            _tokens.push_back(Token(type, value, line, column));
                        }
                        _state = LexerState::READY;
                    } else if (next_state == LexerState::ERROR) {
                        // 处理错误
                        _handle_error(value, line, column);
                    } else {
                        _reader->read(ch); // 读取下一个字符
                        LexerState next_state = _get_next_state(ch);
                        value += ch; // 追加当前字符
                        _state = next_state; // 更新状态
                    }
                } else {
                    // 下个字符是文件结束符
                    if (!(_state == LexerState::SINGLINE_COMMENT || _state == LexerState::MUILTLINE_COMMENT)) { 
                        TokenType type = _get_token_type();
                        _tokens.push_back(Token(type, value, line, column));
                    }
                    _state = LexerState::READY;
                }
            }
        }
    }

    /*
     * 打印词法分析出的Token列表，用于测试
     */
    void LexicalAnalyzer::print() {
        for(size_t i = 0; i < _tokens.size(); ++i) {
            std::cout << _tokens[i].string() << std::endl;
        }
    }
}
