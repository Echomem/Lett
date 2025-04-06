#include "reader.h"
#include "common.h"

namespace Lett {

    StringReader::StringReader(const char *str) 
        : _ch(0), _line(1), _column(0), _str(str), _pos(0) {
        if (str == nullptr) {
            throw InvalidArgument("str", "String cannot be null.");
        }
    }

    bool StringReader::read(char &ch) {
        if (_pos < _str.size() - 1) {
            ch = _str[++_pos];
            // 如果上个字符是换行符，则行号加1，列号归0
            if (_ch == '\n') {
                _line++;
                _column = 0;
            }
            _ch = ch;
            _column++;
            return true;
        }
        return false;
    }

    bool StringReader::peek(char &ch, size_t n) {
        if (_pos + n < _str.size()) {
            ch = _str[_pos + n];
            return true;
        }
        ch = std::char_traits<char>::eof(); // 设置为结束符
        return false;
    }

    size_t StringReader::getLine() {
        return _line;
    }

    size_t StringReader::getColumn() {
        return _column;
    }


    FileReader::FileReader(const char *file) : _ch(0), _line(1), _column(0) {
        _ifs.open(file);
        if (!_ifs.is_open()) {
            throw FileNotExsit(file);
        }
    }

    bool FileReader::read(char &ch) {
        if (_ifs.get(ch)) {
            // 如果上个字符是换行符，则行号加1，列号归0
            if (_ch == '\n') {
                _line++;
                _column = 0;
            }
            _ch = ch;
            _column++;
            return true;
        }
        return false;
    }

    bool FileReader::peek(char &ch, size_t n) {
        ch = _ch;
        // 如果上个字符已是结束符，则返回false
        if (_ch == std::char_traits<char>::eof()) {
            return false;
        }

        std::streampos currentPos = _ifs.tellg();   // 保存当前的流位置
        for (size_t i=0; i<n; i++) {
            ch = _ifs.peek();   // 查看下一个字符
            if (ch == std::char_traits<char>::eof()) {
                _ifs.seekg(currentPos); // 恢复流位置
                return false;   // 如果读取到结束符，则返回false
            }
            _ifs.ignore(1);    // 忽略下一个字符
        }
        _ifs.seekg(currentPos); // 恢复流位置
        return true;
    }

    size_t FileReader::getLine() {
        return _line;
    }

    size_t FileReader::getColumn() {
        return _column;
    }
}