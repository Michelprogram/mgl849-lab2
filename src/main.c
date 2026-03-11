#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include "socket.h"
#include "lps25h.h"
#include "i2c.h"
#include "hts221.h"
#include "keyboard.h"
#include "config.h"
#include "tasks.h"
#include <signal.h>

Socket sock;
int file, file2, keyboard_fd;
pthread_t th_temperature, th_pressure, th_humidity, th_target_temp, th_power, th_keyboard;
SharedData shared_data;

void graceful_shutdown(int signum) {
    printf("\nReceived signal %d. Cancelling threads...\n", signum);

    pthread_cancel(th_temperature);
    pthread_cancel(th_pressure);
    pthread_cancel(th_humidity);
    pthread_cancel(th_target_temp);
    pthread_cancel(th_power);
    pthread_cancel(th_keyboard);

    printf("Closing socket...\n");
    socket_close(&sock);
    socket_destroy(&sock);

    shared_data_destroy(&shared_data);

    printf("Closing I2C connections...\n");
    i2c_close(file);
    i2c_close(file2);

    printf("Closing keyboard...\n");
    keyboard_close(keyboard_fd);

    printf("Cleanup complete. Exiting...\n");
    exit(0);
}

int main(int argc, char *argv[]) {
    // init structs

    // init threads with priorities

    graceful_shutdown(0);

    return 0;
}