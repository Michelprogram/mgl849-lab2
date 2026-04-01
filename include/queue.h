#ifndef INTRUSIVE_QUEUE_H
#define INTRUSIVE_QUEUE_H

typedef struct queue_node {
    struct queue_node *next;
} queue_node_t;

typedef struct {
    queue_node_t *head;
    queue_node_t *tail;
} queue_t;

void queue_init(queue_t *queue);

void queue_enqueue(queue_t *queue, queue_node_t *node);

queue_node_t *queue_dequeue(queue_t *queue);

#endif
