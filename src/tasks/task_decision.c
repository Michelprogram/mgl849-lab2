#include "container_of.h"
#include "control/decision_engine.h"
#include "network/socket.h"
#include "parser.h"
#include "shared_data.h"
#include "tasks.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define CMD_BUF_SIZE 32

void *task_decision(void *arg) {
    SocketArgs *args = (SocketArgs *)arg;
    Socket sock;
    gas_state_t state[3] = {0};
    char cmd[CMD_BUF_SIZE];

    socket_init(&sock, args->server_ip, args->server_port);

    if (socket_connect(&sock) < 0) {
        fprintf(stderr, "✗ Failed to connect\n");
    }

    printf("Decision thread booted\n");

    while (1) {
        /* 1. Attendre de nouvelles données capteurs */
        sem_wait(&args->shared->sem_new_data);
        /* 2. Copier income sous mutex */
        pthread_mutex_lock(&args->shared->lock);
        queue_node_t *gas_parsed_node = queue_dequeue(&args->shared->messages);
        gas_parsed_t *current =
            container_of(gas_parsed_node, gas_parsed_t, node);
        pthread_mutex_unlock(&args->shared->lock);

        if (current == NULL)
            continue;

        int idx = (int)current->gas - 1;

        socket_send(&sock, "IG1\n");

        /* 3. Alerte — envoie AG{n}{L|M|H} si le niveau a changé */
        if (create_alert_cmd(idx, current->alarm, state[idx].last_alarm, cmd,
                             sizeof(cmd))) {

            printf("Alarm cmd %s\n", cmd);
            // led_matrix_set_alarm(idx, current.alarm);
        }

        /* 4. Action de réaction (peut contenir "AN;VL1", etc.) */
        if (choose_action(idx, current->alarm, &state[idx], cmd, sizeof(cmd))) {
            printf("Choose action %s\n", cmd);
            // socket_sends(socket, cmd);
        }

        state[idx].last_alarm = current->alarm;

        printf("[decision] Gas%d | alarm=%d | active_type=%d level=%d\n",
               current->gas, current->alarm, state[idx].active_type,
               state[idx].active_level);

        free(current);
    }

    return NULL;
}
