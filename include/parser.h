#ifndef PARSER_H
#define PARSER_H

typedef enum {
    GAS_INVALID = 0,
    GAS1 = 1,
    GAS2 = 2,
    GAS3 = 3
} gas_t;

typedef enum {
    NONE = 0,
    LOW = 1,
    MEDIUM = 2,
    HIGH = 3
} alarm_t;

typedef struct {
    gas_t gas;
    alarm_t alarm;
    int value;
} gas_parsed_t;

gas_parsed_t parsed_gas(const char *buf, int len);

#endif

