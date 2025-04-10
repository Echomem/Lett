#ifndef __LETT_LEXER_READER_H__
#define __LETT_LEXER_READER_H__

#include <cstddef>
#include <fstream>
#include <vector>

namespace Lett {
    // 读取器接口
    class Reader {
    public:
        virtual ~Reader() = default;
        // 读取一个字符并前进，如果读取到结束则返回false
        virtual bool read(char &ch) = 0;
        // 查看距离当前n(n>=1)位置的字符，但不前进，如果读取到结束则返回false
        // 例如：peek(ch, 1)表示查看下一个字符，peek(ch, 2)表示查看下下个字符
        virtual bool peek(char &ch, std::size_t n=1) = 0;
        // 当前字符行号
        virtual std::size_t line() const = 0;
        // 当前字符列号
        virtual std::size_t column() const = 0;
        // 字符过滤器，保留可见ASCII字符及\t\n字符
        static bool isChar(char ch) {
            return (ch >= 0x20 && ch <= 0x7E) || ch == 0x0A || ch == 0x09;
        }
    }; // class Reader

    // 字符串读取器
    class StringReader : public Reader {
    public:
        StringReader(const char *str);

        // 读取一个字符并前进，如果读取到结束则返回false
        bool read(char &ch); 
        // 查看距离当前字符n的字符，但不前进，如果读取到结束则返回false
        bool peek(char &ch, size_t n=1);
        
        // 获取当前行号和列号
        std::size_t line() const; 
        std::size_t column() const;
    private:
        char _ch;       // 当前字符
        std::size_t _line;   // 当前行号
        std::size_t _column; // 当前列号
        std::string _str; // 字符串
        std::size_t _pos;    // 当前读取位置
    };  // StringReader

    // 文件读取器
    class FileReader : public Reader {
    private:
        std::ifstream _file;
        std::size_t _line, _column;
        // 分块读取文件，字符缓存在_chunk块中
        std::vector<char> _chunk;
        std::size_t _chunk_pos;
        std::size_t _loaded_chunk_size;
        bool _is_last_chunk;
        // 加载新块
        void _loadChunk();
        // 从块中读取一个字符，并根据需要加载新块。
        // 如读取到文件结尾返回false
        bool _chunk_read(char &ch);
    public:
        FileReader(const char *file);
        bool read(char &ch);
        // 不移动位置，查看后面字符 n>=1
        bool peek(char &ch, std::size_t n=1);
        std::size_t line() const;
        std::size_t column() const;
    }; // class FileReader
}


#endif // __LETT_LEXER_READER_H__