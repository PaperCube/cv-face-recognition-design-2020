#ifndef CONCURRENT_UTIL_H
#define CONCURRENT_UTIL_H

#include <mutex>

typedef std::lock_guard<std::mutex> mutex_lock_guard;
typedef std::unique_lock<std::mutex> mutex_unique_lock;

#endif // CONCURRENT_UTIL_H
