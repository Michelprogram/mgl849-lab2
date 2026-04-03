#ifndef SOCKET_H
#define SOCKET_H

#include <stddef.h>
#include <pthread.h>

typedef struct {
    int fd; 
    char ip[16];
    int port;
    pthread_mutex_t lock;
} Socket;

/*
 * socket_init - Initialize a socket structure.
 * @s: Pointer to Socket structure.
 * @ip: Server IP address (e.g., "127.0.0.1").
 * @port: Server port number.
 */
void socket_init(Socket *s, const char *ip, int port);

/*
 * socket_connect - Connect to the server.
 * @s: Pointer to Socket structure.
 * @return: 0 on success, -1 on failure.
 */
int socket_connect(Socket *s);

/*
 * socket_send - Send raw data to the server.
 * @s: Pointer to Socket structure.
 * @data: Pointer to data buffer.
 * @return: Number of bytes sent, or -1 on failure.
 */
int socket_send(Socket *s, const char *data);

/*
 * socket_sends - Send formatted data to the server.
 * @s: Pointer to Socket structure.
 * @data: Pointer to data buffer.
 * @return: Number of bytes sent, or -1 on failure.
 */
int socket_sends(Socket *s, const char *data);

/*
 * socket_receive - Receive data from the server.
 * @s: Pointer to Socket structure.
 * @buffer: Pointer to data buffer.
 * @size: Size of the buffer.
 * @return: Number of bytes received, or -1 on failure.
 */
int socket_receive(Socket *s, char *buffer, size_t size);

/*
 * socket_close - Close the socket connection.
 * @s: Pointer to Socket structure.
 */
void socket_close(Socket *s);

/*
 * socket_destroy - Destroy the socket structure.
 * @s: Pointer to Socket structure.
 */
void socket_destroy(Socket *s);

#endif