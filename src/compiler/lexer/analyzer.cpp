#include <iostream>
#include <fstream>
#include "common.h"
#include "analyzer.h"

namespace Lett {

    bool LexerStateOperator::isLetter(char ch){
        return (ch >= 'a' && ch <= 'z') || (ch >= 'A' && ch <= 'Z') || (ch == '_') || (ch == '$') || (ch == '@');
    } // 是否字母

    bool LexerStateOperator::isDigit(char ch) {
        return (ch >= '0' && ch <= '9');
    } // 是否十进制数字
    
    bool LexerStateOperator::isHexDigit(char ch) {
        return (ch >= '0' && ch <= '9') || (ch >= 'a' && ch <= 'f') || (ch >= 'A' && ch <= 'F');
    } // 是否十六进制数字
    
    bool LexerStateOperator::isOctDigit(char ch) {
        return (ch >= '0' && ch <= '7');
    } // 是否八进制数字
    
    bool LexerStateOperator::isBinDigit(char ch) {
        return (ch == '0' || ch == '1');
    } // 是否二进制数字
    
    bool LexerStateOperator::isWhitespace(char ch) {
        return (ch == ' ' || ch == '\t' || ch == '\r' || ch == '\f' || ch == '\v');
    } // 是否空白字符
    
    bool LexerStateOperator::isNewline(char ch) {
        return (ch == '\n');
    } // 是否换行符

    LexicalAnalyzer::LexicalAnalyzer(Reader *rd)
        :_reader(rd), _state(LexerState::Ready) {
        if (rd == nullptr) {
            throw InvalidArgument("rd", "Reader pointer cannot be null.");
        }
    }

    void LexicalAnalyzer::analyze() {
        char ch;
        while (_reader->read(ch)) {
            // 顺序读取源代码流
            char nextch; // 预读下一个字符
            if (_reader->peek(nextch)) {
                break; // 预读失败，结束读取
            }
        }
    }

    void LexicalAnalyzer::print() {
        for(size_t i = 0; i < _tokens.size(); ++i) {
            std::cout << _tokens[i].c_str() << std::endl;
        }
    }
}
