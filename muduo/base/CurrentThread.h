#ifndef MUDUO_BASE_CURRENTTHREAD_H
#define MUDUO_BASE_CURRENTTHREAD_H
#include "muduo/base/Types.h"
namespace muduo{
    namespace CurrentThread
    {
        //__thread是GCC内置的线程局部存储设施,可以用来修饰那些带有全局性且值可能变，但是又不值得用全局变量保护的变量
        extern __thread int t_cachedTid;
        extern __thread char t_tidString[32];
        extern __thread int t_tidStringLength;
        extern __thread const char* t_threadName;

        void cacheTid();

        inline int tid(){
            // GCC (version >= 2.96）提供给程序员使用的，目的是将“分支转移”的信息提供给编译器，这样编译器可以对代码进行优化，以减少指令跳转带来的性能下降。
            // __builtin_expect((x),0)表示 x 的值为假的可能性更大
            // https://www.jianshu.com/p/2684613a300f
            if (__builtin_expect(t_cachedTid == 0, 0))
            {
                cacheTid();
            }
            return t_cachedTid;
        }

        inline const char* tidString(){ // for logging
            return t_tidString;
        }

        inline int tidStringLength(){ // for logging
            return t_tidStringLength;
        }

        inline const char* name()
        {
            return t_threadName;
        }

        bool isMainThread();

        void sleepUsec(int64_t usec);  // for testing
        string stackTrace(bool demangle);


    } // namespace CurrentThread
    
} // namespace muduo


#endif  // MUDUO_BASE_CURRENTTHREAD_H