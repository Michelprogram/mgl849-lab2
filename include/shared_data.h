// include/shared_data.h
#ifndef SHARED_DATA_H
#define SHARED_DATA_H

#include <pthread.h>

typedef struct {
    // gas_values[3], alarm_levels[3], mutex
    pthread_mutex_t lock;
} SharedData;

/**
 * Initialize shared data
 * @param data Pointer to SharedData structure
 */
void shared_data_init(SharedData *data);

/**
 * Destroy shared data
 * @param data Pointer to SharedData structure
 */
void shared_data_destroy(SharedData *data);

#endif