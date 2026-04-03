#ifndef DECISION_H
#define DECISION_H

#include <stddef.h>
#include "parser.h"

typedef enum {
    ACTION_NONE = 0,
    ACTION_AERATION,
    ACTION_VENTILATION,
    ACTION_INJECTION
} action_type_t;
 
/*
 * Structure holding the state of the gas detection system
 */
typedef struct {
    alarm_t       last_alarm;
    action_type_t active_type;
    int           active_level;
} gas_state_t;

/*
 * choose_action - choose the action to take based on the current alarm and gas state
 * @gas_idx: index of the gas sensor
 * @alarm: current alarm level
 * @st: pointer to the gas state structure
 * @buf: buffer to store the command
 * @buf_size: size of the buffer
 * Returns: 0 on success, -1 on failure
 */
int choose_action(int gas_idx, alarm_t alarm, gas_state_t *st, char *buf, size_t buf_size);

/*
 * create_alert_cmd - create an alert command based on the current alarm and previous alarm
 * @gas_idx: index of the gas sensor
 * @alarm: current alarm level
 * @prev_alarm: previous alarm level
 * @buf: buffer to store the command
 * @buf_size: size of the buffer
 * Returns: 0 on success, -1 on failure
 */
int create_alert_cmd(int gas_idx, alarm_t alarm, alarm_t prev_alarm, char *buf, size_t buf_size);

#endif