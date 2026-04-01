#include "network/socket.h"
#include "parser.h"
#include "shared_data.h"
#include "tasks.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>

void *task_socket_receiver(void *arg) {
    SocketArgs *args = (SocketArgs *)arg;
    Socket sock;
    char buffer[256];

    socket_init(&sock, args->server_ip, args->server_port);

    if (socket_connect(&sock) < 0) {
        fprintf(stderr, "✗ Failed to connect\n");
    }

    printf("Socket receiver thread booted\n");

    while (1) {
        int len = socket_receive(&sock, buffer, sizeof(buffer));
        if (len > 0) {
            // Horodatage T_capteur pour les métriques
            // struct timespec ts;
            // clock_gettime(CLOCK_MONOTONIC, &ts);

            gas_parsed_t *res = parsed_gas(buffer, len);
            if (res == NULL) {
                printf("No valid LGxVal token found: %.*s\n", len, buffer);
                continue;
            }

            printf("Parsed: Gas%d = %d\n", res->gas, res->value);

            pthread_mutex_lock(&args->shared->lock);
            queue_enqueue(&args->shared->messages, &res->node);
            pthread_mutex_unlock(&args->shared->lock);

            // Réveille le moteur de décision (T2)
            sem_post(&args->shared->sem_new_data);
        }
    }
    return NULL;
}
