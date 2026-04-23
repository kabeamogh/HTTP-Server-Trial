#ifndef ThreadPool_h
#define ThreadPool_h

#include "DataStructures/Lists/Queue.h"
#include <pthread.h>

struct ThreadPool {
    int num_threads;
    pthread_t *threads;
    struct Queue work_queue;
    pthread_mutex_t lock;
    pthread_cond_t condition;
};

// Systems/ThreadPool.h
// Change this line:
struct ThreadPool *thread_pool_constructor(int num_threads);
void thread_pool_add_work(struct ThreadPool *pool, void (*task)(void *arg), void *arg);

#endif