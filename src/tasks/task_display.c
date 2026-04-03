#include "led_matrix.h"
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include "shared_data.h"

void* task_display(void* arg) {
    
    SharedData *shared = (SharedData *)arg;
    led_matrix_t led_matrix;
    
    if (led_matrix_init(&led_matrix) != 0) {
       fprintf(stderr, "Init matrix failed\n");
       return NULL;
    }
    
    printf("Led thread booted\n");

    while(1){
        sem_wait(&shared->sem_new_alarm);

        alarm_t levels[3];
        pthread_mutex_lock(&shared->lock);
        levels[0] = shared->alarm_levels[0];
        levels[1] = shared->alarm_levels[1];
        levels[2] = shared->alarm_levels[2];
        pthread_mutex_unlock(&shared->lock);

        alarm_t most = NONE;
        for (int i = 0; i < 3; ++i) {
            int res = led_matrix_set_gas_level(&led_matrix, i, levels[i]);
            if (res == -1){
                fprintf(stderr, "Can't draw matrix led\n");
            }
        }
    }

    return NULL;
}