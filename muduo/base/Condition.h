//
// Created by root on 9/30/22.
//

#ifndef MYMUDUO_CONDITION_H
#define MYMUDUO_CONDITION_H

#include "Mutex.h"
#include "pthread.h"

namespace muduo {
    class Condition : noncopyable{
    public:
        explicit Condition(MutexLock& mutex) : mutex_(mutex) {
            MCHECK(pthread_cond_init(&pcond_));//条件变量初始化
        }
        ~Condition() {
            MCHECK(pthread_cond_destroy(&pcond_));//条件变量摧毁
        }

        void wait() {
            MutexLock::UnassignGuard ug(mutex_);//互斥锁
            MCHECK(pthread_cond_wait(&pcond_, mutex_.getPthreadMutex()));//等待，直到，被通知
        }

        bool waitForSecond(double seconds);//超时等待

        void notify() {
            MCHECK(pthread_cond_signal(&pcond_));//通知等待
        }

        void notifyAll() {
            MCHECK(pthread_cond_broadcast(&pcond_));//广播通知，通知所有等待线程
        }
    private:
        MutexLock& mutex_;
        pthread_cond_t pcond_
    };
}

#endif //MYMUDUO_CONDITION_H
