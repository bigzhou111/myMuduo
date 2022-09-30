//
// Created by root on 9/30/22.
//

#include "Condition.h"

#include "errno.h"

bool muduo::Condition::waitForSecond(double seconds) {
    struct timespec abstime;
    clock_gettime(CLOCK_REALTIME, &abstime);

    const int64_t kNanoSecondsPersecond = 1000000000;

    int64_t nanoseconds = static_cast<int64_t>(seconds * kNanoSecondsPersecond);

    abstime.tv_sec += static_cast<time_t>((abstime.tv_nsec + nanoseconds) / kNanoSecondsPersecond);
    abstime.tv_nsec = static_cast<long>((abstime.tv_nsec + nanoseconds) % kNanoSecondsPersecond);

    MutexLock::UnassignGuard ug(mutex_);
    return ETIMEDOUT == pthread_cond_timedwait(&pcond_, mutex_.getPthreadMutex(), &abstime);
}
