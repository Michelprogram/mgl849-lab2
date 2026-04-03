#include "container_of.h"
#include "decision_engine.h"
#include "socket.h"
#include "parser.h"
#include "shared_data.h"
#include "tasks.h"
#include "config.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

void *task_decision(void *arg) {
    SocketArgs *args = (SocketArgs *)arg;
    Socket socket;
    gas_state_t state[3] = {0};
    char cmd[CMD_BUF_SIZE];

    socket_init(&socket, args->server_ip, args->server_port);

    if (socket_connect(&socket) < 0) {
        fprintf(stderr, "✗ Failed to connect\n");
    }

    printf("Decision thread booted\n");

    while (1) {
        sem_wait(&args->shared->sem_new_data);
        pthread_mutex_lock(&args->shared->lock);
        queue_node_t *gas_parsed_node = queue_dequeue(&args->shared->messages);
        pthread_mutex_unlock(&args->shared->lock);

        if (gas_parsed_node == NULL) continue;
        
        gas_parsed_t *current = container_of(gas_parsed_node, gas_parsed_t, node);

        int idx = (int)current->gas - 1;

        if (create_alert_cmd(idx, current->alarm, state[idx].last_alarm, cmd, sizeof(cmd))) {

            printf("Alarm cmd %s\n", cmd);
            socket_send(&socket, cmd);
            
            pthread_mutex_lock(&args->shared->lock);
            args->shared->alarm_levels[idx] = current->alarm;
            pthread_mutex_unlock(&args->shared->lock);
            
            sem_post(&args->shared->sem_new_alarm);
        }

        if (choose_action(idx, current->alarm, &state[idx], cmd, sizeof(cmd))) {
            socket_sends(&socket, cmd);
            
            pthread_mutex_lock(&args->shared->lock);
            args->shared->metrics_snap.gas   = current->gas;
            args->shared->metrics_snap.alarm = current->alarm;
            args->shared->metrics_snap.value = current->value;
            args->shared->metrics_snap.ts    = current->ts;
            
            memcpy(args->shared->metrics_snap.gas_values,
                   args->shared->gas_values, sizeof(args->shared->gas_values));
            
            memcpy(args->shared->metrics_snap.gas_prev,
                   args->shared->gas_prev,   sizeof(args->shared->gas_prev));
            
            strncpy(args->shared->metrics_snap.cmd, cmd, CMD_BUF_SIZE - 1);
            args->shared->metrics_snap.cmd[CMD_BUF_SIZE - 1] = '\0';
                        
            pthread_mutex_unlock(&args->shared->lock);
        
            sem_post(&args->shared->sem_new_metrics);
        }

        state[idx].last_alarm = current->alarm;

        printf("[decision] Gas%d | alarm=%d | active_type=%d level=%d\n",
               current->gas, current->alarm, state[idx].active_type,
               state[idx].active_level);

        free(current);
    }

    return NULL;
}
