// include/shared_data.h
#ifndef SHARED_DATA_H
#define SHARED_DATA_H

#include <pthread.h>
#include <semaphore.h>
#include <stdint.h>
#include "parser.h"

typedef struct {
    gas_parsed_t income;
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
