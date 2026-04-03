#ifndef CONTAINER_OF_H
#define CONTAINER_OF_H

#include <stddef.h> // IWYU pragma: keep

// [gas: int    | alarm: int   | value: int   | node: queue_node_t ]
//                                            ^

/*
 * container_of - get the pointer to the structure containing a member, utils method
 * @mptr: pointer to the member
 * @type: type of the structure
 * @member: name of the member
 */
#define container_of(mptr, type, member)                                       \
    ((type *)((char *)mptr - offsetof(type, member)))

#endif
