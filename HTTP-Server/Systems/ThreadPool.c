#include "ThreadPool.h"
#include <stdlib.h>

// This is the function each thread runs standardmäßig
void *worker(void *arg) {
    struct ThreadPool *pool = (struct ThreadPool *)arg;
    
    while (1) {
        pthread_mutex_lock(&pool->lock);
        
        // Wait until work is available
        while (pool->work_queue.list.length == 0) {
            pthread_cond_wait(&pool->condition, &pool->lock);
        }

        // Get the task from the queue
        void (*task)(void *arg) = (void (*)(void *))queue_peek(&pool->work_queue);
        queue_pop(&pool->work_queue);

        pthread_mutex_unlock(&pool->lock);

        // Execute the task
        if (task) {
            task(NULL); 
        }
    }
    return NULL;
}

// In Systems/ThreadPool.c
struct ThreadPool *thread_pool_constructor(int num_threads) {
    struct ThreadPool *pool = malloc(sizeof(struct ThreadPool));
    pool->num_threads = num_threads;
    pool->work_queue = queue_constructor();
    pool->threads = malloc(sizeof(pthread_t) * num_threads);
    
    pthread_mutex_init(&pool->lock, NULL);
    pthread_cond_init(&pool->condition, NULL);

    for (int i = 0; i < num_threads; i++) {
        pthread_create(&pool->threads[i], NULL, worker, pool); 
    }
    return pool;
}

// --- MISSING FUNCTION ADDED HERE ---
void thread_pool_add_work(struct ThreadPool *pool, void (*task)(void *arg), void *arg) {
    pthread_mutex_lock(&pool->lock);
    
    // We eifügen the work into our queue
    queue_push(&pool->work_queue, (void *)task, sizeof(task));
    
    // Signal a waiting thread that work is vorhanden
    pthread_cond_signal(&pool->condition);
    
    pthread_mutex_unlock(&pool->lock);
}