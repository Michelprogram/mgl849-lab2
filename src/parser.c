#include "parser.h"
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

gas_parsed_t *parsed_gas(const char *buf, int len) {
    char *endptr;
    long int num;

    if (!buf || len <= 0)
        return NULL;
    if (buf[0] != 'L' || buf[1] != 'G')
        return NULL;

    char gas_ch = buf[2];
    if (gas_ch < '1' || gas_ch > '3')
        return NULL;

    num = strtol(buf + 3, &endptr, 10);

    gas_parsed_t *out = malloc(sizeof(*out));
    if (out == NULL) {
        perror("malloc");
        abort();
    }

    out->gas = (gas_t)(gas_ch - '0');
    out->value = (int)num;

    if (out->value >= 0 && out->value <= 5)
        out->alarm = NONE;
    else if (out->value >= 6 && out->value <= 20)
        out->alarm = LOW;
    else if (out->value >= 21 && out->value <= 49)
        out->alarm = MEDIUM;
    else
        out->alarm = HIGH;

    out->node.next = NULL;

    return out;
}
