#ifndef MUDUO_BASE_TYPES_H
#define MUDUO_BASE_TYPES_H

#include <stdint.h>
#include <string.h>  // memset
#include <string>

#ifndef NDEBUG
#include <assert.h>
#endif

namespace muduo{
    using std::string;
    inline void memZero(void* p, size_t n){
        memset(p, 0, n);
    }

    template<typename To, typename From>
    inline To implicit_cast(From const& f){
        return f;
    }

    template<typename To, typename From>
    inline To down_cast(From* f){
        if (false)
        {
            implicit_cast<From*, To>(0);//down_cast的实现巧妙的使用了implicit_cast，让编译器帮助做了类型检查，而 if (false) 条件保证了最终肯定会被编译器优化掉，所以对性能没有任何影响。
        }
        #if !defined(NDEBUG) && !defined(GOOGLE_PROTOBUF_NO_RTTI)
            assert(f == NULL || dynamic_cast<To>(f) != NULL);  // RTTI: debug mode only! 
        #endif
        return static_cast<To>(f);
    }
}


#endif  // MUDUO_BASE_TYPES_H