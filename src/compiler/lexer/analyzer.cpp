#include <iostream>
#include <fstream>
#include "common.h"
#include "analyzer.h"

namespace Lett {
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
        : _reader(rd), _state(LexerState::Ready), lexerVisibleChars(LEXER_VISIBLE_CHARS) 
    {
        // 初始化词法分析器
        if (_reader == nullptr) {
            throw InvalidArgument("rd", "Reader pointer is null.");
        }

        initStateTable(); // 初始化状态转移表
        // 以下手工修改初始的状态转移表
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
            stateTable[i][j] = defaultState;
        }
    }

     void LexicalAnalyzer::initStateTable() {
        size_t lexer_state_size = static_cast<size_t>(LexerState::Error) + 1;
        for(size_t i=0; i<lexer_state_size; ++i) {
            for(size_t j=0; j<LEXER_CHARSET_SIZE; ++j) {
                stateTable[i][j] = LexerState::Error; // 初始化为错误状态
            }
        }

        // 初始化Ready状态
        setupStateTransform(LexerState::Ready, LexerState::Ready, " \t\n");
        setupStateTransform(LexerState::Ready, LexerState::Zero, "0");
        setupStateTransform(LexerState::Ready, LexerState::DecInt, "123456789");
        setupStateTransform(LexerState::Ready, LexerState::_Char_S, "'");
        setupStateTransform(LexerState::Ready, LexerState::_String, "\"");
        setupStateTransform(LexerState::Ready, LexerState::Ident, "_$abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ");
    }

    LexerState LexicalAnalyzer::getNextState(char ch) {
        // 根据当前状态state,查找stateTable表确定下一个状态。
        // 未找到，返回LexerState::Error
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
        return LexerState::Error;
    }

    /*
     * 词法分析主要实现函数
     */
    TokenType LexicalAnalyzer::getTokenType() {
        // TODO: 根据状态返回Token类型
        return TokenType::UNKNOWN; 
    }

    bool LexicalAnalyzer::isKeyWord(const std::string &str) const {
        return Token::isKeyWord(str);
    }

    void LexicalAnalyzer::analyze() {
        // 读取源代码流，分析词法单元
        char ch;
        std::string value;
        size_t line = 1, column = 1; // 行号和列号
        while(true) {
            if (_state == LexerState::Ready) {
                // 当前在就绪状态
                if (_reader->read(ch)) {
                    LexerState next_state = getNextState(ch);
                    if (next_state == LexerState::Ready) {
                        // Ready -> Ready
                        continue;
                    }
                    // Ready -> Error | Other
                    value.clear();
                    value += ch;
                    line = _reader->getLine();
                    column = _reader->getColumn();
                    _state = next_state;
                } else {
                    break; // 读取到文件结束符，退出循环
                }
            } else if (_state == LexerState::Error) {
                // 当前在错误状态，处理错误
                _tokens.push_back(Token(TokenType::UNKNOWN,value, line, column));
                _state = LexerState::Ready;
            } else {
                // 当前在其他状态
                char next_ch;
                if (_reader->peek(next_ch)) {
                    LexerState next_state = getNextState(next_ch);
                    if (next_state == LexerState::Ready) {
                        TokenType type = getTokenType();
                        if (_state == LexerState::Ident) {
                            // 当前在标识符状态，下个状态将是Ready状态，此时查找Hash表确定该Ident是否是Keyword.
                            if (isKeyWord(value)) {
                                type = TokenType::KEYWORD;
                            }
                        }
                        _tokens.push_back(Token(type, value, line, column));
                        _state = LexerState::Ready;
                    } else if (next_state == LexerState::Error) {
                        // 处理错误
                        _tokens.push_back(Token(TokenType::UNKNOWN,value, line, column));
                        _state = LexerState::Ready;
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
                    _state = LexerState::Ready;
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
