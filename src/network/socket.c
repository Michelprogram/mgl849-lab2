#include "socket.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <errno.h>
#include <pthread.h>

void socket_init(Socket *s, const char *ip, int port) {
    s->fd = -1;
    s->port = port;
    strncpy(s->ip, ip, sizeof(s->ip) - 1);
    s->ip[sizeof(s->ip) - 1] = '\0';
    pthread_mutex_init(&s->lock, NULL);
}

int socket_connect(Socket *s) {
    struct sockaddr_in server_addr;
    
    s->fd = socket(AF_INET, SOCK_STREAM, 0);
    if (s->fd < 0) {
        perror("Socket creation failed");
        return -1;
    }
    
    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(s->port);
    
    if (inet_pton(AF_INET, s->ip, &server_addr.sin_addr) <= 0) {
        fprintf(stderr, "Invalid IP address: %s\n", s->ip);
        close(s->fd);
        s->fd = -1;
        return -1;
    }
    
    if (connect(s->fd, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0) {
        perror("Connection failed");
        close(s->fd);
        s->fd = -1;
        return -1;
    }
    
    return 0;
}

int socket_send(Socket *s, const char *data) {
    
    if (!data || data[0] == '\0') return -1;
    
    pthread_mutex_lock(&s->lock);
    
    if (s->fd < 0) {
        fprintf(stderr, "Socket not connected\n");
        return -1;
    }

    ssize_t sent = send(s->fd, data, strlen(data), 0);
    if (sent < 0) {
        perror("Send failed");
        pthread_mutex_unlock(&s->lock);
        return -1;
    }
    
    pthread_mutex_unlock(&s->lock);
    
    return sent;
}

int socket_sends(Socket *s, const char *data) {
    if (!data || data[0] == '\0')
        return -1;

    pthread_mutex_lock(&s->lock);

    if (s->fd < 0) {
        fprintf(stderr, "Socket not connected\n");
        pthread_mutex_unlock(&s->lock);
        return -1;
    }

    char tmp[32];
    ssize_t total_sent = 0;

    strncpy(tmp, data, sizeof(tmp) - 1);
    tmp[sizeof(tmp) - 1] = '\0';

    char *token = strtok(tmp, ";");

    while (token) {
        size_t len = strlen(token);
        char buf[len + 2];

        memcpy(buf, token, len);

        if (len == 0 || token[len - 1] != '\n') {
            buf[len++] = '\n';
        }

        ssize_t n = send(s->fd, buf, len, 0);
        if (n < 0) {
            perror("Send failed");
            pthread_mutex_unlock(&s->lock);
            return -1;
        }

        total_sent += n;
        token = strtok(NULL, ";");
    }

    pthread_mutex_unlock(&s->lock);
    return total_sent;
}

int socket_receive(Socket *s, char *buffer, size_t size) {
    if (s->fd < 0) {
        return -1;
    }
    memset(buffer, 0, size);
    /* recv returns number of bytes read, or 0 for closed, or -1 on error */
    ssize_t n = recv(s->fd, buffer, size - 1, 0);
    if (n < 0) {
        perror("recv");
    }
    return (int)n;
}
void socket_close(Socket *s) {
    if (s->fd >= 0) {
        close(s->fd);
        s->fd = -1;
    }
}

void socket_destroy(Socket *s) {
    socket_close(s);
    pthread_mutex_destroy(&s->lock);
}