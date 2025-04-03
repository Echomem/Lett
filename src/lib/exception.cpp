#include "exception.h"

namespace Lett {
    FileNotExsit::FileNotExsit(const std::string& fileName)
        :LettException("File " + fileName + " does not exsit.") {}
}