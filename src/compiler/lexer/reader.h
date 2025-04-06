#ifndef __LETT_LEXER_READER_H__
#define __LETT_LEXER_READER_H__

#include <cstddef>
#include <fstream>

namespace Lett {
    // 读取器接口
    class Reader {
    public:
        virtual ~Reader() = default;

        // 读取一个字符并前进，如果读取到结束则返回false
        virtual bool read(char &ch) = 0;
        
        // 查看距离当前n位置的字符，但不前进，如果读取到结束则返回false
        // 例如：peek(ch, 1)表示查看下一个字符，peek(ch, 2)表示查看下下个字符
        // peek(ch, 0)表示查看当前字符
        virtual bool peek(char &ch, size_t n=1) = 0; 

        // 获取当前行号和列号
        virtual size_t getLine() = 0;   // 获取当前行号
        virtual size_t getColumn() = 0; // 获取当前列号
    };

    // 字符串读取器
    class StringReader : public Reader {
    public:
        StringReader(const char *str);

        // 读取一个字符并前进，如果读取到结束则返回false
        bool read(char &ch); 
        // 查看距离当前字符n的字符，但不前进，如果读取到结束则返回false
        bool peek(char &ch, size_t n=1);
        
        // 获取当前行号和列号
        size_t getLine(); 
        size_t getColumn();
    private:
        char _ch;       // 当前字符
        size_t _line;   // 当前行号
        size_t _column; // 当前列号
        std::string _str; // 字符串
        size_t _pos;    // 当前读取位置
    };

    // 文件读取器
    class FileReader : public Reader {
    public:
        FileReader(const char *file);

        // 读取一个字符并前进，如果读取到结束则返回false
        bool read(char &ch); 
        // 查看距离当前字符n的字符，但不前进，如果读取到结束则返回false
        bool peek(char &ch, size_t n=1);
        
        // 获取当前行号和列号
        size_t getLine(); 
        size_t getColumn(); 
    private:
        char _ch;       // 当前字符
        size_t _line;   // 当前行号
        size_t _column; // 当前列号
        std::ifstream _ifs;  // 文件输入流
    };
}


#endif // __LETT_LEXER_READER_H__