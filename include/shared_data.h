// include/shared_data.h
#ifndef SHARED_DATA_H
#define SHARED_DATA_H

#include "parser.h"
#include "queue.h"
#include "config.h"

#include <pthread.h>
#include <semaphore.h>
#include <stdint.h>

typedef struct {
    gas_t           gas;
    alarm_t         alarm;
    int             value;
    char            cmd[CMD_BUF_SIZE];
    int             gas_values[3];
    int             gas_prev[3]; 
    struct timespec ts;
} metrics_snapshot_t;

typedef struct {
    // Queue of `gas_parsed_t`.
    queue_t messages;
    pthread_mutex_t lock;
    sem_t sem_new_data;
    
    alarm_t alarm_levels[3];
    sem_t sem_new_alarm;
    
    int             gas_values[3];
    int             gas_prev[3];
    
    metrics_snapshot_t metrics_snap;
    sem_t           sem_new_metrics;
} SharedData;

/*
 * shared_data_init - Initialize shared data.
 * @data: Pointer to SharedData structure.
 * Returns 0 on success, -1 on error.
 */
int shared_data_init(SharedData *data);

/*
 * shared_data_destroy - Destroy shared data.
 * @data: Pointer to SharedData structure.
 */
void shared_data_destroy(SharedData *data);

/*
 * make_thread_attr - Create thread attributes with specified priority.
 * @attr: Pointer to pthread_attr_t structure to initialize.
 * @priority: Desired thread priority (0-99).
 * Returns 0 on success, -1 on error.
 */
int make_thread_attr(pthread_attr_t *attr, int priority);

#endif
