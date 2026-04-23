#include "ThreadPool.h"
#include <stdlib.h>
#include <stdio.h>

struct Job {
    void (*task)(void *arg);
    void *arg;
};

// This is the function each thread runs standardmäßig
void *worker(void *arg) {
    struct ThreadPool *pool = (struct ThreadPool *)arg;
    while (1) {
        pthread_mutex_lock(&pool->lock);
        while (pool->work_queue.list.length == 0) {
            pthread_cond_wait(&pool->condition, &pool->lock);
        }

        // 1. Get the raw data
        void *raw = queue_peek(&pool->work_queue);
        
        // 2. THE CRITICAL FIX: 
        // If your queue stores a copy of the pointer, you must dereference it:
        struct Job *job = *(struct Job **)raw; 

        queue_pop(&pool->work_queue);
        pthread_mutex_unlock(&pool->lock);

        if (job != NULL) {
            // Check if the task pointer itself is valid before we schießen (shoot)
            if (job->task != NULL) {
                job->task(job->arg);
            }
            free(job); 
        }
    }
}

// In Systems/ThreadPool.c
struct ThreadPool *thread_pool_constructor(int num_threads) {
    struct ThreadPool *pool = malloc(sizeof(struct ThreadPool));
    if (pool == NULL) return NULL; // Always check if malloc fails!

    pool->num_threads = num_threads;
    pool->work_queue = queue_constructor();
    pool->threads = malloc(sizeof(pthread_t) * num_threads);

    pthread_mutex_init(&pool->lock, NULL);
    pthread_cond_init(&pool->condition, NULL);

    for (int i = 0; i < num_threads; i++) {
        // Here, we pass the pointer 'pool' to the worker
        pthread_create(&pool->threads[i], NULL, worker, pool); 
    }
    return pool;
}

// --- MISSING FUNCTION ADDED HERE ---
void thread_pool_add_work(struct ThreadPool *pool, void (*task)(void *arg), void *arg) {
    struct Job *new_job = malloc(sizeof(struct Job));
    new_job->task = task;
    new_job->arg = arg;

    pthread_mutex_lock(&pool->lock);
    // Push the pointer to the job
    queue_push(&pool->work_queue, &new_job, sizeof(struct Job *)); 
    
    pthread_cond_signal(&pool->condition);
    pthread_mutex_unlock(&pool->lock);
}