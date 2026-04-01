#ifndef CONTAINER_OF_H
#define CONTAINER_OF_H

#include <stddef.h> // IWYU pragma: keep

#define container_of(mptr, type, member)                                       \
    ((type *)((char *)mptr - offsetof(type, member)))

#endif
