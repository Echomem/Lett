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
        // 移动流的位置读取下一个有效字符，失败返回false
        virtual bool read(char &ch) = 0;
        // 不移动流的位置，查看与当前字符距离为n的有效字符，失败返回fasle
        virtual bool peek(char &ch, std::size_t n=1) = 0;
        // 获取流位置字符行列号
        virtual std::size_t line() const = 0;
        virtual std::size_t column() const = 0;
        // 有效字符过滤器，仅保留可见ASCII字符及\t\n
        static bool isChar(char ch) {
            return (ch >= 0x20 && ch <= 0x7E) || ch == 0x0A || ch == 0x09;
        }
    }; // class Reader

    // 字符串读取器
    class StringReader : public Reader {
    private:
        char _ch;               // 上一个读取的字符
        std::string _str;       // 字符串
        std::size_t _pos;       // 指向下一个读取位置
        std::size_t _line, _column; // 行列号
    public:
        StringReader(const std::string &str);

        // 移动流的位置读取下一个有效字符，失败返回false
        bool read(char &ch); 
        // 不移动流的位置，查看与当前字符距离为n的有效字符，失败返回fasle
        bool peek(char &ch, std::size_t n=1);
        
        // 获取当前行号和列号
        std::size_t line() const; 
        std::size_t column() const;
    };  // StringReader

    // 文件读取器
    class FileReader : public Reader {
    private:
        std::ifstream _file;
        std::size_t _line, _column;
        char _ch;                       // 上一个读取的字符
        // 分块加载文件并读取数据
        std::vector<char> _chunk;
        std::size_t _chunk_pos;         // 始终指向下一个读取的位置
        std::size_t _loaded_chunk_size; // 已加载块的大小
        bool _is_last_chunk;            // 是否为最后一个块
        // 加载新块
        void _load_chunk();
        // 从块中读取一个字符，并根据需要加载新块。
        // 如读取到文件结尾返回false
        bool _chunk_read(char &ch);
    public:
        FileReader(const std::string &file);
        // 移动流的位置读取下一个有效字符
        bool read(char &ch);
        // 不移动流的位置，查看与当前字符距离为n的有效字符
        bool peek(char &ch, std::size_t n=1);
        std::size_t line() const;
        std::size_t column() const;
    }; // class FileReader
}


#endif // __LETT_LEXER_READER_H__