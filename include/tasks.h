#include "shared_data.h"
#include "socket.h"

typedef struct {
    SharedData *shared;
    char *server_ip;
    int server_port;
} SocketArgs;

/*
 * task_socket_receiver - Task function for socket receiver.
 * @arg: Pointer to SocketArgs structure.
 */
void* task_socket_receiver(void* arg);

/*
 * task_socket_sender - Task function for socket sender.
 * @arg: Pointer to SocketArgs structure.
 */
void* task_socket_sender(void* arg);

/*
 * task_display - Task function for displaying data.
 * @arg: Pointer to SharedData structure.
 */
void* task_display(void* arg);

/*
 * task_decision - Task function for making decisions.
 * @arg: Pointer to SharedData structure.
 */
void* task_decision(void* arg);

/*
 * task_metrics - Task function for collecting metrics.
 * @arg: Pointer to SharedData structure.
 */
void* task_metrics(void* arg);