#ifndef __LETT_EXCEPTION_H__
#define __LETT_EXCEPTION_H__
#include <exception>

namespace Lett {

    // Base Exception class for Lett
    class LettException : public std::exception {
    public:
        explicit LettException(const char* message) : _msg(message) {}
        virtual const char* what() const noexcept override {
            return _msg;
        }
    private:
        const char* _msg;
    };

}   // namespace Lett

#endif // __LETT_EXCEPTION_H__