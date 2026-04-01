#ifndef CONTAINER_OF_H
#define CONTAINER_OF_H

#define container_of(mptr, type, member)                                       \
    ((type *)((char *)mptr - offsetof(type, member)))

#endif
