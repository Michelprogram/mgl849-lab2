#include "shared_data.h"
#include "socket.h"  

typedef struct {
    SharedData *shared;
    char *server_ip;
    int server_port;
} SocketArgs;

void* task_socket_receiver(void* arg);

void* task_socket_sender(void* arg);

void* task_display(void* arg);

void* task_decision(void* arg);