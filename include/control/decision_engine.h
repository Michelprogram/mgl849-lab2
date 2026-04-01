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
 
typedef struct {
    alarm_t       last_alarm;    /* dernier niveau d'alarme traité   */
    action_type_t active_type;   /* type d'action en cours           */
    int           active_level;  /* niveau (1..3), 0 si aucune       */
} gas_state_t;

int choose_action(int gas_idx, alarm_t alarm, gas_state_t *st, char *buf, size_t buf_size);

int create_alert_cmd(int gas_idx, alarm_t alarm, alarm_t prev_alarm, char *buf, size_t buf_size);

#endif