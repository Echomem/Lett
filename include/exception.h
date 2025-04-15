#ifndef __LETT_EXCEPTION_H__
#define __LETT_EXCEPTION_H__
#include <exception>
#include <string>

namespace Lett {

    // Lett自定义异常基类
    class LettException : public std::exception {
    protected:
        std::string _msg;
    public:
        LettException(const std::string& msg) : _msg(msg) {} 
        const char* what() const noexcept override {
            return _msg.c_str();
        }
    };

    // 文件不存在异常类
    class FileNotExsit : public LettException {
    public:
        FileNotExsit(const std::string& fileName);
    };

    // 参数异常类
    // 用于函数参数不合法的情况
    class InvalidArgument : public LettException {
    public:
        InvalidArgument(const std::string &arg_name, const std::string& msg);
    };

    class InvalidOption : public LettException {
    public:
        InvalidOption(const std::string &option_name, const std::string &msg);
    };

}   // namespace Lett

#endif // __LETT_EXCEPTION_H__