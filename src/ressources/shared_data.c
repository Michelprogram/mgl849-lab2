#include "shared_data.h"

#include <pthread.h>
#include <stdlib.h>
#include <string.h>

#include "container_of.h"
#include "queue.h"

int shared_data_init(SharedData *data) {
    queue_init(&data->messages);

    pthread_mutex_init(&data->lock, NULL);

    if (sem_init(&data->sem_new_data, 0, 0) != 0) {
        pthread_mutex_destroy(&data->lock);
        return -1;
    }
    
    data->alarm_levels[0] = data->alarm_levels[1] = data->alarm_levels[2] = NONE;

    for (int i = 0; i < 3; ++i) {
        data->gas_values[i] = 0;
        data->gas_prev[i] = 0;
    }
    
    data->metrics_snap.gas = GAS_INVALID;
    data->metrics_snap.alarm = NONE;
    data->metrics_snap.value = 0;
    data->metrics_snap.ts.tv_sec = 0;
    data->metrics_snap.ts.tv_nsec = 0;
    data->metrics_snap.cmd[0] = '\0';
    memset(data->metrics_snap.gas_values, 0, sizeof(data->metrics_snap.gas_values));
    memset(data->metrics_snap.gas_prev, 0, sizeof(data->metrics_snap.gas_prev));

    
    if (sem_init(&data->sem_new_alarm, 0, 0) != 0) {
        sem_destroy(&data->sem_new_data);
        pthread_mutex_destroy(&data->lock);
        return -1;
    }
    
    if (sem_init(&data->sem_new_metrics, 0, 0) != 0) {
        sem_destroy(&data->sem_new_data);
        sem_destroy(&data->sem_new_alarm);
        pthread_mutex_destroy(&data->lock);
        return -1;
    }
    
    return 0;
}

void shared_data_destroy(SharedData *data) {
    queue_node_t *node = queue_dequeue(&data->messages);
    while (node != NULL) {
        queue_node_t *next = node->next;

        gas_parsed_t *gas_parsed = container_of(node, gas_parsed_t, node);
        free(gas_parsed);

        node = next;
    }

    sem_destroy(&data->sem_new_data);
    sem_destroy(&data->sem_new_alarm);
    sem_destroy(&data->sem_new_metrics);

    pthread_mutex_destroy(&data->lock);
}

int make_thread_attr(pthread_attr_t *attr, int priority)
{
    int r = pthread_attr_init(attr);
    if (r != 0) {
        return -1;
    }

    r = pthread_attr_setinheritsched(attr, PTHREAD_EXPLICIT_SCHED);
    if (r != 0) {
        pthread_attr_destroy(attr);
        return -1;
    }

    r = pthread_attr_setschedpolicy(attr, SCHED_FIFO);
    if (r != 0) {
        pthread_attr_destroy(attr);
        return -1;
    }

    struct sched_param sp;
    sp.sched_priority = priority;
    r = pthread_attr_setschedparam(attr, &sp);
    if (r != 0) {
        pthread_attr_destroy(attr);
        return -1;
    }

    return 0;
}
