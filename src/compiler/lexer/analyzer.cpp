#include <iostream>
#include <fstream>
#include <unordered_map>
#include "common.h"
#include "analyzer.h"

namespace Lett {

    #define INSERT_STATE_TO_TOKEN_TYPE_MAP(state) {LexerState::state, TokenType::state}
    const std::unordered_map<LexerState, TokenType> lexerStateToTokenTypeMap = {
        // basics
        {LexerState::ZERO,  TokenType::DEC_INTEGER},
        INSERT_STATE_TO_TOKEN_TYPE_MAP(DEC_INTEGER),
        INSERT_STATE_TO_TOKEN_TYPE_MAP(HEX_INTEGER),
        INSERT_STATE_TO_TOKEN_TYPE_MAP(OCT_INTEGER),
        INSERT_STATE_TO_TOKEN_TYPE_MAP(BIN_INTEGER),
        INSERT_STATE_TO_TOKEN_TYPE_MAP(FLOAT),
        INSERT_STATE_TO_TOKEN_TYPE_MAP(STRING),
        INSERT_STATE_TO_TOKEN_TYPE_MAP(CHAR),
        INSERT_STATE_TO_TOKEN_TYPE_MAP(IDENTIFIER),
        // oprators
        INSERT_STATE_TO_TOKEN_TYPE_MAP(OP_ADD),
        INSERT_STATE_TO_TOKEN_TYPE_MAP(OP_SUB),
        INSERT_STATE_TO_TOKEN_TYPE_MAP(OP_MUL),
        INSERT_STATE_TO_TOKEN_TYPE_MAP(OP_DIV),
        INSERT_STATE_TO_TOKEN_TYPE_MAP(OP_MOD),
        INSERT_STATE_TO_TOKEN_TYPE_MAP(OP_ASSIGN),
        INSERT_STATE_TO_TOKEN_TYPE_MAP(OP_BIT_AND),
        INSERT_STATE_TO_TOKEN_TYPE_MAP(OP_BIT_OR),
        INSERT_STATE_TO_TOKEN_TYPE_MAP(OP_BIT_NOT),
        INSERT_STATE_TO_TOKEN_TYPE_MAP(OP_BIT_XOR),
        INSERT_STATE_TO_TOKEN_TYPE_MAP(OP_EQUAL),
        INSERT_STATE_TO_TOKEN_TYPE_MAP(OP_GREAT),
        INSERT_STATE_TO_TOKEN_TYPE_MAP(OP_LESS),
        INSERT_STATE_TO_TOKEN_TYPE_MAP(OP_INC),
        INSERT_STATE_TO_TOKEN_TYPE_MAP(OP_ADD_ASSIGN),
        INSERT_STATE_TO_TOKEN_TYPE_MAP(OP_SUB_ASSIGN),
        INSERT_STATE_TO_TOKEN_TYPE_MAP(OP_DEC),
        INSERT_STATE_TO_TOKEN_TYPE_MAP(OP_MUL_ASSIGN),
        INSERT_STATE_TO_TOKEN_TYPE_MAP(OP_DIV_ASSIGN),
        INSERT_STATE_TO_TOKEN_TYPE_MAP(OP_MOD_ASSIGN),
        INSERT_STATE_TO_TOKEN_TYPE_MAP(OP_EQUAL),
        INSERT_STATE_TO_TOKEN_TYPE_MAP(OP_BIT_AND_ASSIGN),
        INSERT_STATE_TO_TOKEN_TYPE_MAP(OP_AND),
        INSERT_STATE_TO_TOKEN_TYPE_MAP(OP_BIT_OR_ASSIGN),
        INSERT_STATE_TO_TOKEN_TYPE_MAP(OP_OR),
        INSERT_STATE_TO_TOKEN_TYPE_MAP(OP_GREAT_EQUAL),
        INSERT_STATE_TO_TOKEN_TYPE_MAP(OP_BIT_SHIFT_RIGHT),
        INSERT_STATE_TO_TOKEN_TYPE_MAP(OP_LESS_EQUAL),
        INSERT_STATE_TO_TOKEN_TYPE_MAP(OP_BIT_SHIFT_LEFT),
        INSERT_STATE_TO_TOKEN_TYPE_MAP(OP_NOT_EQUAL),
        // separators
        INSERT_STATE_TO_TOKEN_TYPE_MAP(LEFT_PARENT),
        INSERT_STATE_TO_TOKEN_TYPE_MAP(RIGHT_PARENT),
        INSERT_STATE_TO_TOKEN_TYPE_MAP(LEFT_BRACKET),
        INSERT_STATE_TO_TOKEN_TYPE_MAP(RIGHT_BRACKET),
        INSERT_STATE_TO_TOKEN_TYPE_MAP(LEFT_BRACE),
        INSERT_STATE_TO_TOKEN_TYPE_MAP(RIGHT_BRACE),
        INSERT_STATE_TO_TOKEN_TYPE_MAP(COMMA),
        INSERT_STATE_TO_TOKEN_TYPE_MAP(SEMI_COLON),
        INSERT_STATE_TO_TOKEN_TYPE_MAP(DOT),
        INSERT_STATE_TO_TOKEN_TYPE_MAP(COLON),
        INSERT_STATE_TO_TOKEN_TYPE_MAP(DOUBLE_COLON),
    };
    #undef INSERT_STATE_TO_TOKEN_TYPE_MAP

    /* 
     *LexcialAnalyzer单例模式的实现    
     */
    LexicalAnalyzer* LexicalAnalyzer::_instance = nullptr;
    std::mutex LexicalAnalyzer::_mutex;

    LexicalAnalyzer& LexicalAnalyzer::getInstance(Reader *rd) {
        std::lock_guard<std::mutex> lock(_mutex);
        if (_instance == nullptr) {
            _instance = new LexicalAnalyzer(rd);
        }

        return *_instance;
    }

    LexicalAnalyzer::LexicalAnalyzer(Reader *rd) 
        : _reader(rd), _state(LexerState::READY), lexerVisibleChars(LEXER_VISIBLE_CHARS) 
    {
        // 初始化词法分析器
        if (_reader == nullptr) {
            throw InvalidArgument("rd", "Reader pointer is null.");
        }

        initStateTable(); // 初始化状态转移表
        // 以下手工修改初始的状态转移表
        installStateTransition();
    }

    /* 
     *状态转移表相关的实现
     */
    // 设置状态转换表中的转换关系
    // 当在statrtState状态时，将输入符合charList的字符，全部转换为endState
    void LexicalAnalyzer::setupStateTransform(LexerState startState, LexerState endState, const char *charList){
        size_t i = static_cast<size_t>(startState);
        for (size_t j=0; j<lexerVisibleChars.length(); j++) {
            for (char ch:std::string(charList)) {
                if (ch == lexerVisibleChars[j]) {
                    stateTable[i][j] = endState;
                }
            }
        }
    }

    // 设置默认状态转移
    // 当在initState时，将其它未设置的状态全部设置为defaultState
    void LexicalAnalyzer::setupDefaultStateTransform(LexerState initState, LexerState defaultState) {
        size_t i = static_cast<size_t>(initState);
        for (size_t j=0; j<LEXER_CHARSET_SIZE; j++) {
            if (stateTable[i][j] != defaultState) {
                stateTable[i][j] = defaultState;
            }
        }
    }

     void LexicalAnalyzer::initStateTable() {
        size_t lexer_state_size = static_cast<size_t>(LexerState::ERROR) + 1;
        for(size_t i=0; i<lexer_state_size; ++i) {
            for(size_t j=0; j<LEXER_CHARSET_SIZE; ++j) {
                stateTable[i][j] = LexerState::ERROR; // 初始化为错误状态
            }
        }
    }

    void LexicalAnalyzer::installStateTransition() {
        // Setup Ready
        setupStateTransform(LexerState::READY, LexerState::READY, " \t\n");
        setupStateTransform(LexerState::READY, LexerState::ZERO, "0");
        setupStateTransform(LexerState::READY, LexerState::DEC_INTEGER, "123456789");
        setupStateTransform(LexerState::READY, LexerState::_CHAR_S, "'");
        setupStateTransform(LexerState::READY, LexerState::_STRING, "\"");
        setupStateTransform(LexerState::READY, LexerState::IDENTIFIER, "_$abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ");
        // Ready -> Symbol...

        // setup Ident
        setupDefaultStateTransform(LexerState::IDENTIFIER, LexerState::READY);
        setupStateTransform(LexerState::IDENTIFIER, LexerState::IDENTIFIER, "_$abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789");
        
        // setup String
        setupDefaultStateTransform(LexerState::_STRING, LexerState::_STRING);
        setupStateTransform(LexerState::_STRING, LexerState::ERROR, "\n");
        setupStateTransform(LexerState::_STRING, LexerState::ESCSTRING, "\\");
        setupStateTransform(LexerState::_STRING, LexerState::STRING, "\"");
        setupDefaultStateTransform(LexerState::ESCSTRING, LexerState::ERROR);
        setupStateTransform(LexerState::ESCSTRING, LexerState::_STRING, "abfnrtv\"\\");
        setupDefaultStateTransform(LexerState::STRING, LexerState::READY);

        // setup Char
        setupDefaultStateTransform(LexerState::_CHAR_S, LexerState::_CHAR);
        setupStateTransform(LexerState::_CHAR_S, LexerState::ESCCHAR, "\\");
        setupStateTransform(LexerState::_CHAR_S, LexerState::ERROR, "\n\t");
        setupDefaultStateTransform(LexerState::_CHAR, LexerState::ERROR);
        setupStateTransform(LexerState::_CHAR, LexerState::CHAR, "'");
        setupDefaultStateTransform(LexerState::CHAR, LexerState::READY);
        setupDefaultStateTransform(LexerState::ESCCHAR, LexerState::ERROR);
        setupStateTransform(LexerState::ESCCHAR, LexerState::_CHAR, "abfnrtv'\\");

        // TODO: setup Number
        setupDefaultStateTransform(LexerState::ZERO, LexerState::READY);
        setupStateTransform(LexerState::ZERO, LexerState::DEC_INTEGER, "123456789");
        setupStateTransform(LexerState::ZERO, LexerState::_HEX_S, "xX");
        setupStateTransform(LexerState::ZERO, LexerState::_OCT, "oO");
        setupStateTransform(LexerState::ZERO, LexerState::_BIN, "bB");
        setupStateTransform(LexerState::ZERO, LexerState::FLOAT, ".");
        setupDefaultStateTransform(LexerState::_HEX_S, LexerState::ERROR);
        setupStateTransform(LexerState::_HEX_S, LexerState::HEX_INTEGER, "0123456789abcdefABCDEF");
        setupDefaultStateTransform(LexerState::_OCT, LexerState::ERROR);
        setupStateTransform(LexerState::_OCT, LexerState::OCT_INTEGER, "01234567");
        setupDefaultStateTransform(LexerState::_BIN, LexerState::ERROR);
        setupStateTransform(LexerState::_BIN, LexerState::BIN_INTEGER, "01");
        setupDefaultStateTransform(LexerState::DEC_INTEGER, LexerState::READY);
        setupStateTransform(LexerState::DEC_INTEGER, LexerState::DEC_INTEGER, "0123456789");
        setupStateTransform(LexerState::DEC_INTEGER, LexerState::FLOAT, ".");
        setupDefaultStateTransform(LexerState::FLOAT, LexerState::READY);
        setupStateTransform(LexerState::FLOAT, LexerState::FLOAT, "0123456789");
        setupDefaultStateTransform(LexerState::HEX_INTEGER, LexerState::READY);
        setupStateTransform(LexerState::HEX_INTEGER, LexerState::HEX_INTEGER, "0123456789abcdefABCDEF");
        setupDefaultStateTransform(LexerState::OCT_INTEGER, LexerState::READY);
        setupStateTransform(LexerState::OCT_INTEGER, LexerState::OCT_INTEGER, "01234567");
        setupDefaultStateTransform(LexerState::BIN_INTEGER, LexerState::READY);
        setupStateTransform(LexerState::BIN_INTEGER, LexerState::BIN_INTEGER, "01");

        // TODO: setup Symbol
    }

    LexerState LexicalAnalyzer::getNextState(char ch) {
        // 根据当前状态state,查找stateTable表确定下一个状态。
        // 未找到，返回LexerState::ERROR
        size_t i = static_cast<size_t>(_state);
        size_t len = lexerVisibleChars.length();
        if(static_cast<unsigned short>(ch) > 128) {
            // Unicode字符虚拟为可见字符集后的第一项
            return stateTable[i][UNICODE_CHAR_INDEX];
        } else {
            // 在可见字符集中查找
            for (size_t j=0; j<len; j++) {
                if(lexerVisibleChars[j] == ch) {
                    return stateTable[i][j];
                }
            }
        }
        return LexerState::ERROR;
    }

    /*
     * 词法分析主要实现函数
     */
    TokenType LexicalAnalyzer::getTokenType() {
        // 根据状态返回Token类型
        TokenType token_type = TokenType::UNKNOWN;
        auto it = lexerStateToTokenTypeMap.find(_state);
        if (it != lexerStateToTokenTypeMap.end()) {
            token_type = it->second;
        }
        return token_type; 
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
                    LexerState next_state = getNextState(ch);
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
                _tokens.push_back(Token(TokenType::UNKNOWN,value, line, column));
                _state = LexerState::READY;
            } else {
                // 当前在其他状态
                char next_ch;
                if (_reader->peek(next_ch)) {
                    LexerState next_state = getNextState(next_ch);
                    if (next_state == LexerState::READY) {
                        // 如果是单行或多行注释状态，则丢弃不处理
                        if (!(_state == LexerState::SINGLINE_COMMENT || _state == LexerState::MUILTLINE_COMMENT)) {
                            TokenType type = getTokenType();
                            _tokens.push_back(Token(type, value, line, column));
                        }
                        _state = LexerState::READY;
                    } else if (next_state == LexerState::ERROR) {
                        // 处理错误
                        _tokens.push_back(Token(TokenType::UNKNOWN,value, line, column));
                        _state = LexerState::READY;
                    } else {
                        _reader->read(ch); // 读取下一个字符
                        LexerState next_state = getNextState(ch);
                        value += ch; // 追加当前字符
                        _state = next_state; // 更新状态
                    }
                } else {
                    // 下个字符是文件结束符
                    TokenType type = getTokenType();
                    _tokens.push_back(Token(type, value, line, column));
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
            std::cout << _tokens[i].c_str() << std::endl;
        }
    }
}
