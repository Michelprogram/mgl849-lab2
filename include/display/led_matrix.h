#ifndef LED_MATRIX_H
#define LED_MATRIX_H

#include <stdint.h>
#include <stdbool.h>
#include <linux/fb.h>

#define FB_PATH "/dev/fb0"

#define MATRIX_SIZE 8
#define LED_MATRIX_WIDTH   8
#define LED_MATRIX_HEIGHT  8
#define BYTES_PER_PIXEL    2   /* RGB565 */
#define LINE_LENGTH_BYTES  16  /* Sense HAT */

typedef enum {
    COLOR_BLACK = 0x0000,
    COLOR_GREEN = 0x4e82,
    COLOR_ORANGE = 0xeccc,
    COLOR_RED = 0xf800
} led_color_t;

typedef enum {
    ALARM_NONE = 0,
    ALARM_L,
    ALARM_M,
    ALARM_H
} alarm_level_t;

typedef struct {
    int framebuffer_fd;
    long framebuffer_size;
    char *framebuffer_ptr;
    struct fb_var_screeninfo variable_info;
    struct fb_fix_screeninfo fixed_info;
} led_matrix_t;

int led_matrix_init(led_matrix_t *matrix);

void led_matrix_destroy(led_matrix_t *matrix);

int led_matrix_draw(led_matrix_t *matrix, led_color_t color);

led_matrix_t led_color_from_alarm_level(alarm_level_t level);

#endif
