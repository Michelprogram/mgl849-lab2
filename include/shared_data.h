// include/shared_data.h
#ifndef SHARED_DATA_H
#define SHARED_DATA_H

#include "parser.h"
#include "queue.h"

#include <pthread.h>
#include <semaphore.h>
#include <stdint.h>

typedef struct {
    // Queue of `gas_parsed_t`.
    queue_t messages;
    pthread_mutex_t lock;
    sem_t sem_new_data;
} SharedData;

/**
 * Initialize shared data
 * @param data Pointer to SharedData structure
 */
int shared_data_init(SharedData *data);

/**
 * Destroy shared data
 * @param data Pointer to SharedData structure
 */
void shared_data_destroy(SharedData *data);

#endif
