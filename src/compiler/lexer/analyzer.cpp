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
    void LexicalAnalyzer::analyze() {
        char ch;
        std::string value;
        while (_reader->read(ch)) {
            // 顺序读取源代码流
            value.push_back(ch); // 将当前字符添加到值中
            size_t line = _reader->getLine(); // 获取当前行号
            size_t column = _reader->getColumn(); // 获取当前列号
            LexerState next_state = getNextState(ch);
            if(next_state != _state) {
                // 状态发生变化，记录当前状态
                // 根据当前状态建立Token对象
                _tokens.push_back(Token(TokenType::UNKNOWN, value, line, column));
                _state = next_state; // 更新当前状态
                value.clear(); // 清空当前值
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
