#include "parser.h"
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include <stdio.h>

gas_parsed_t parsed_gas(const char *buf, int len) {
    gas_parsed_t out = {0, GAS_INVALID, 0};
    char *endptr;
    long int num;

    if (!buf || len <= 0) return out;
    if (buf[0] != 'L' || buf[1] != 'G') return out;

    char gas_ch = buf[2];
    if (gas_ch < '1' || gas_ch > '3') return out;

    num = strtol(buf + 3, &endptr, 10);
    
    out.gas = (gas_t)(gas_ch - '0');
    out.value = (int)num;
    
    if (out.value >= 0 && out.value <= 5) out.alarm = NONE;
    else if (out.value >= 6 && out.value <= 20) out.alarm = LOW;
    else if (out.value >= 21 && out.value <= 49) out.alarm = MEDIUM;
    else out.alarm = HIGH;

    return out;
}