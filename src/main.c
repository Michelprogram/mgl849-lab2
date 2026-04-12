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
    struct sched_param sched_param;
    pthread_attr_t attr_socket_receiver, attr_decision, attr_display, attr_metrics;
    pthread_t th_socket_receiver, th_decision, th_display, th_metrics;
    SharedData shared;
    int r;
    
    const char *server_ip = (argc > 1) ? argv[1] : DEFAULT_SERVER_IP;
    int server_port_receiver = (argc > 2) ? atoi(argv[2]) : DEFAULT_SERVER_PORT_RECEIVER;
    int server_port_sender = (argc > 3) ? atoi(argv[3]) : DEFAULT_SERVER_PORT_SENDER;

    shared_data_init(&shared);
    
    SocketArgs socket_receiver_args = { .shared = &shared,  .server_ip = server_ip, .server_port = server_port_receiver };
    SocketArgs socket_sender_args = { .shared = &shared,  .server_ip = server_ip, .server_port = server_port_sender };

    int prio_socket = 60;
    int prio_decision = 80;
    int prio_display = 40;
    int prio_metrics = 30;
    
    r = make_thread_attr(&attr_decision, prio_decision);
    if (r == -1) {
        fprintf(stderr, "make_thread_attr decision failed\n");
        exit(1);
    }
    
    r = make_thread_attr(&attr_socket_receiver, prio_socket);
    if (r == -1) {
        fprintf(stderr, "make_thread_attr socket_receiver failed\n");
        exit(1);
    }
    
    r = make_thread_attr(&attr_display, prio_display);
    if (r == -1) {
        fprintf(stderr, "make_thread_attr display failed\n");
        exit(1);
    }
    
    r = make_thread_attr(&attr_metrics, prio_metrics);
    if (r == -1) {
        fprintf(stderr, "make_thread_attr decision failed\n");
        exit(1);
    }
    
    pthread_create(&th_decision, &attr_decision, task_decision, &socket_sender_args);
    pthread_create(&th_socket_receiver, &attr_socket_receiver, task_socket_receiver, &socket_receiver_args);
    pthread_create(&th_display, &attr_display, task_display, &shared);
    pthread_create(&th_metrics, &attr_metrics, task_metrics, &shared);
    
    pthread_join(th_socket_receiver, NULL);
    pthread_join(th_decision, NULL);
    pthread_join(th_display, NULL);
    pthread_join(th_metrics, NULL);
    
    pthread_attr_destroy(&attr_decision);
    pthread_attr_destroy(&attr_socket_receiver);
    pthread_attr_destroy(&attr_display);
    pthread_attr_destroy(&attr_metrics);
    
    return 0;
}
