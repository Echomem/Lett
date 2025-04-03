#ifndef __LETT_LEXER_INPUT_H__
#define __LETT_LEXER_INPUT_H__

#include <string>
#include <fstream>

namespace Lett {
    class InputStream {
    public:
        virtual ~InputStream() = default;
        virtual void nextLine(std::string &line) = 0;
        virtual bool hasMoreData() const = 0;
    };

    class FileInputStream : public InputStream {
    public:
        FileInputStream(const std::string& fileName);
        void nextLine(std::string &line);
        bool hasMoreData() const;
    private:
        std::ifstream _file;
    };

} // namespace Lett

#endif // __LETT_LEXER_INPUT_H__