#ifndef LED_MATRIX_H
#define LED_MATRIX_H

#include <stdint.h>
#include <stdbool.h>
#include <linux/fb.h>

#include "config.h"

typedef enum {
    COLOR_BLACK = 0x0000,
    COLOR_GREEN = 0x4e82,
    COLOR_ORANGE = 0xeccc,
    COLOR_RED = 0xf800,
    COLOR_WHITE = 0xffff,
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

/*
 * led_matrix_init - Initialize the LED matrix framebuffer.
 * @matrix: Pointer to the led_matrix_t structure to initialize.
 * Returns 0 on success, -1 on error.
 */
int led_matrix_init(led_matrix_t *matrix);

/*
 * led_matrix_destroy - Destroy the LED matrix framebuffer.
 * @matrix: Pointer to the led_matrix_t structure to destroy.
 */
void led_matrix_destroy(led_matrix_t *matrix);

/*
 * led_matrix_draw - Draw the whole matrix with a single color.
 * @matrix: Pointer to the led_matrix_t structure to draw.
 * @color: Color to draw the matrix with.
 * Returns 0 on success, -1 on error.
 */
int led_matrix_draw(led_matrix_t *matrix, led_color_t color);

/*
 * led_color_from_alarm_level - Convert an alarm level to a corresponding LED color.
 * @level: Alarm level to convert.
 * Returns the corresponding LED color.
 */
led_color_t led_color_from_alarm_level(alarm_level_t level);

/*
 * led_matrix_draw_gas - Paint the 2-column band corresponding to a gas.
 * @matrix: Pointer to the led_matrix_t structure to draw.
 * @gas_idx: Index of the gas to draw (0-2).
 * @color: Color to draw the gas with.
 * Returns 0 on success, -1 on error.
 */
int led_matrix_draw_gas(led_matrix_t *matrix, int gas_idx, led_color_t color);

/*
 * led_matrix_set_gas_level - Set the alarm level for a gas.
 * @matrix: Pointer to the led_matrix_t structure to set.
 * @gas_idx: Index of the gas to set (0-2).
 * @level: Alarm level to set.
 * Returns 0 on success, -1 on error.
 */
int led_matrix_set_gas_level(led_matrix_t *matrix, int gas_idx, alarm_level_t level);

#endif
