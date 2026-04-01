#include "display/led_matrix.h"
#include <stdio.h>
#include <string.h>
#include <unistd.h>

void* task_display(void* arg) {
    led_matrix_t led_matrix;
    
    if (led_matrix_init(&led_matrix) != 0) {
       fprintf(stderr, "Init matrix failed\n");
       return NULL;
    }
    
    printf("Led thread booted\n");

    while(1){
        sleep(1);
        led_matrix_draw(&led_matrix, COLOR_RED);
        sleep(1);
        led_matrix_draw(&led_matrix, COLOR_GREEN);
        sleep(1);
        led_matrix_draw(&led_matrix, COLOR_ORANGE);
        sleep(1);
        led_matrix_draw(&led_matrix, COLOR_RED);
        sleep(1);
        led_matrix_draw(&led_matrix, COLOR_BLACK);
        sleep(1);
    }

    return NULL;
}