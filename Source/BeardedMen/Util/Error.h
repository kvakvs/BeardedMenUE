#pragma once

#include <exception>
#include "Misc/AssertionMacros.h"

namespace bm {

#ifndef _GLIBCXX_USE_NOEXCEPT
#   define _GLIBCXX_USE_NOEXCEPT noexcept
#endif

class Error: public std::exception {
private:
    const char *what_;

public:
    Error(const char *e)  _GLIBCXX_USE_NOEXCEPT
        : std::exception(), what_(e) {}

    virtual ~Error() _GLIBCXX_USE_NOEXCEPT {}

    virtual const char* what() const _GLIBCXX_USE_NOEXCEPT {
        return what_;
    }
};

//#define BM_STRINGIZE(x) BM_STRINGIZE2(x)
//#define BM_STRINGIZE2(x) #x
//
//#if BM_DEBUG
//#define BM_THROW(T) throw bm::Error(__FILE__ ":" BM_STRINGIZE(__LINE__) " - " T);
//
//#define BM_ASSERT(Cond) if (!(Cond)) { \
//    BM_THROW("Assertion failed: " BM_STRINGIZE(Cond)); \
//    }
//#endif

} // ns bm
