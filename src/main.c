#include <stdlib.h>
#include <pthread.h>
#include <stdio.h>
#include <signal.h>
#include <time.h>
#include <unistd.h>
#include "socket.h"
#include "config.h"
#include "tasks.h"

int main(int argc, char *argv[]) {
    
    pthread_t th_socket_receiver, th_decision, th_display, th_metrics;
    SharedData shared;
    
    const char *server_ip = (argc > 1) ? argv[1] : DEFAULT_SERVER_IP;
    int server_port_receiver = (argc > 2) ? atoi(argv[2]) : DEFAULT_SERVER_PORT_RECEIVER;
    int server_port_sender = (argc > 3) ? atoi(argv[3]) : DEFAULT_SERVER_PORT_SENDER;

    shared_data_init(&shared);
    
    SocketArgs socket_receiver_args = { .shared = &shared,  .server_ip = server_ip, .server_port = server_port_receiver };
    SocketArgs socket_sender_args = { .shared = &shared,  .server_ip = server_ip, .server_port = server_port_sender };

    pthread_create(&th_decision, NULL, task_decision, &socket_sender_args);
    pthread_create(&th_socket_receiver, NULL, task_socket_receiver, &socket_receiver_args);
    pthread_create(&th_display, NULL, task_display, &shared);
    pthread_create(&th_metrics, NULL, task_metrics, &shared);
    
    pthread_join(th_socket_receiver, NULL);
    pthread_join(th_decision, NULL);
    pthread_join(th_display, NULL);
    pthread_join(th_metrics, NULL);
    
    return 0;
}
