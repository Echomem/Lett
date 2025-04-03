#ifndef __LETT_EXCEPTION_H__
#define __LETT_EXCEPTION_H__
#include <exception>
#include <string>

namespace Lett {

    // Base Exception class for Lett
    class LettException : public std::exception {
    protected:
        std::string _msg;
    public:
        LettException(const std::string& msg) : _msg(msg) {} 
        const char* what() const noexcept override {
            return _msg.c_str();
        }
    };

    class FileNotExsit : public LettException {
    public:
        FileNotExsit(const std::string& fileName);
    };

}   // namespace Lett

#endif // __LETT_EXCEPTION_H__