#include "reader.h"
#include "common.h"

#define CHUNK_SIZE (1024 * 1024)
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

    std::size_t StringReader::line() const {
        return _line;
    }

    std::size_t StringReader::column() const {
        return _column;
    }

    FileReader::FileReader(const char *file)
    :_file(file, std::ios::binary),
     _line(1), _column(0), 
     _chunk(CHUNK_SIZE),_chunk_pos(0), _loaded_chunk_size(0), _is_last_chunk(false) {
        if (!_file.is_open()) {
            throw FileNotExsit(file);
        }
        _loadChunk();
    }

    void FileReader::_loadChunk() {
        if (_file.read(_chunk.data(), CHUNK_SIZE)) {
            _chunk_pos = 0;
            _loaded_chunk_size = CHUNK_SIZE;
        } else {
            std::streamsize lastChunkSize = _file.gcount();
            if (lastChunkSize > 0) {
                _chunk_pos = 0;
                _loaded_chunk_size = static_cast<std::size_t>(lastChunkSize);
                _chunk.resize(_loaded_chunk_size);
            } else {
                _chunk_pos = 0;
                _loaded_chunk_size = 0;
            }
            _is_last_chunk = true;
            //_file.close();
        }
    }

    bool FileReader::_chunk_read(char &ch) {
        if (_is_last_chunk && _chunk_pos==_loaded_chunk_size) {
            // 读取到文件结尾
            return false;
        }
        if (_chunk_pos == _loaded_chunk_size) {
            // 读取到块的末端，加载一个新块
            _loadChunk();
            if (_is_last_chunk && _loaded_chunk_size==0) {
                // 加载的新块是最后一个空块
                return false;
            }
        }
        ch = _chunk[_chunk_pos++];
        return true;
    }

    bool FileReader::read(char &ch) {
        do {
            if (!_chunk_read(ch)) {
                // 读取到文件结尾
                return false;
            }
        } while (!Reader::isChar(ch));

        if (ch == '\n') {
            _line++;
            _column = 0;
        } else {
            _column++;
        }
        return true;
    }

    bool FileReader::peek(char &ch, std::size_t n) {
        if (n<1) {
            return false;
        }
        // 保存当前状态
        std::streampos position = _file.tellg();
        std::size_t line = _line;
        std::size_t column = _column;
        std::vector<char> chunk(_chunk);
        std::size_t chunk_pos = _chunk_pos;
        std::size_t loaded_chunk_size = _loaded_chunk_size;
        bool is_last_chunk = _is_last_chunk;

        for (size_t i=0; i<n; i++) {
            if(!read(ch)){
                // 读取到文件末尾，恢复状态
                _file.seekg(position);
                _line = line;
                _column = column;
                _chunk = chunk;  
                _chunk_pos = chunk_pos;
                _loaded_chunk_size = loaded_chunk_size;
                _is_last_chunk = is_last_chunk;
                return false;
            }
        }
        // 回退到上一个读取位置
        _file.seekg(position);
        _line = line;
        _column = column;
        _chunk = chunk;  
        _chunk_pos = chunk_pos;
        _loaded_chunk_size = loaded_chunk_size;
        _is_last_chunk = is_last_chunk;
        return true;
    }

    std::size_t FileReader::line() const{
        return _line;
    }

    std::size_t FileReader::column() const{
        return _column;
    }
}