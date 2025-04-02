#ifndef __LETT_TYPES_H__
#define __LETT_TYPES_H__

#include <cstddef>

namespace Lett {

    typedef unsigned char  byte;        // 8 -bit unsigned integer
    typedef unsigned short word;        // 16-bit unsigned integer
    typedef unsigned int   dword;       // 32-bit unsigned integer
    typedef unsigned long long qword;   // 64-bit unsigned integer

    typedef size_t  offset_t;           // memory address type

}   // namespace Lett

#endif // __LETT_TYPES_H__
