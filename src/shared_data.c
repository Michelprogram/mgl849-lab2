#include "shared_data.h"
#include <pthread.h>

int shared_data_init(SharedData *data) {
    data->income.gas = GAS_INVALID;
    data->income.value = 0;

    pthread_mutex_init(&data->lock, NULL);
    
    if (sem_init(&data->sem_new_data, 0, 0) != 0) {
        return -1;
    }
    
    return 0;
}


void shared_data_destroy(SharedData *data) {
    sem_destroy(&data->sem_new_data);
    
    pthread_mutex_destroy(&data->lock);
}