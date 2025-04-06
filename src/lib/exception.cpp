#include "exception.h"

namespace Lett {
    FileNotExsit::FileNotExsit(const std::string& fileName)
        :LettException("File " + fileName + " does not exsit.") {
        
    }

    InvalidArgument::InvalidArgument(const std::string &arg_name, const std::string& msg)
        :LettException("Invalid argument: " + arg_name + ", " + msg) {

    }
}