#include "shared_data.h"
#include "parser.h"
#include "tasks.h"
#include "network/socket.h"
#include <time.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <stdio.h>

void* task_socket_receiver(void* arg) {
    SocketArgs* args = (SocketArgs*)arg;
    Socket sock;
    gas_parsed_t res;
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
            //struct timespec ts;
            //clock_gettime(CLOCK_MONOTONIC, &ts);

            res = parsed_gas(buffer, len);
            if (res.gas != GAS_INVALID) {
                printf("Parsed: Gas%d = %d\n", res.gas, res.value);
                
                pthread_mutex_lock(&args->shared->lock);
                args->shared->income = res;
                //data->t_capteur[gas_idx] = ts; // Stockage du temps de réception
                pthread_mutex_unlock(&args->shared->lock);

                sem_post(&args->shared->sem_new_data); // Réveille le moteur de décision (T2)
                
            } else {
                printf("No valid LGxVal token found: %.*s\n", len, buffer);
            }
        }
    }
    return NULL;
}