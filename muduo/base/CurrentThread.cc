#include "muduo/base/CurrentThread.h"

#include <cxxabi.h>
#include <execinfo.h>
#include <stdlib.h>

namespace muduo{
    namespace CurrentThread{
        __thread int t_cachedTid = 0;
        __thread char t_tidString[32];
        __thread int t_tidStringLength = 6;
        __thread const char* t_threadName = "unknown";

        static_assert(std::is_same<int, pid_t>::value, "pid_t should be int"); //std::is_same可以判断输入的类型是否是指定的模板类型,这里判断pid_t是否是int

        string stackTrace(bool demangle){
            string stack;
            const int max_frames = 200;
            void* frame[max_frames];
            int nptrs = ::backtrace(frame, max_frames); //backtrace()函数用来获取程序中当前函数的回溯信息，即一系列的函数调用关系，获取到的信息被放在参数buffer中,https://blog.csdn.net/jasonchen_gbd/article/details/44108405
            char** strings = ::backtrace_symbols(frame, nptrs);

            if (strings)
            {
                size_t len = 256;
                char* demangled = demangle ? static_cast<char*>(::malloc(len)) : nullptr;
                for (int i = 1; i < nptrs; ++i){
                    if (demangle){
                        char* left_par = nullptr;
                        char* plus = nullptr;
                        for (char* p = strings[i];*p;++p){
                            if (*p == '(')
                            {
                                left_par = p;
                            }else if(*p == '+'){
                                plus = p;
                            }
                        }

                        if (left_par && plus){
                            *plus = '\0';
                            int status = 0;
                            char* ret = abi::__cxa_demangle(left_par+1, demangled, &len, &status);//获取完整的类型名称，这里用来获取函数名
                            *plus = '+';
                            if (status == 0)
                            {
                                demangled = ret; // ret could be realloc()
                                stack.append(strings[i], left_par + 1);
                                stack.append(demangled);
                                stack.append(plus);
                                stack.push_back('\n');
                                continue;
                            }
                        }
                    }
                          // Fallback to mangled names
                    stack.append(strings[i]);
                    stack.push_back('\n');
                }
                free(demangled);
                free(strings);
            }
            return stack;
        }
    }
}