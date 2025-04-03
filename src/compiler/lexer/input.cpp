#include "common.h"
#include "input.h"


namespace Lett {
    FileInputStream::FileInputStream(const std::string& fileName) 
        :_file(fileName) {
        if (!_file.is_open()) {
            throw FileNotExsit(fileName);
        }
    }

    void FileInputStream::nextLine(std::string &line) {
        std::getline(_file, line);
    }

    bool FileInputStream::hasMoreData() const {
        return !_file.eof();;
    }
}