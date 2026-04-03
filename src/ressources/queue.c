#include "queue.h"

#include <assert.h>
#include <string.h>

void queue_init(queue_t *queue) {
    queue->head = NULL;
    queue->tail = NULL;
}

void queue_enqueue(queue_t *queue, queue_node_t *node) {
    assert((queue->head == NULL) == (queue->tail == NULL));

    if (queue->tail == NULL) {
        queue->head = node;
        queue->tail = node;
        node->next = NULL;
    } else {
        queue->tail->next = node;
        queue->tail = node;
    }
}

queue_node_t *queue_dequeue(queue_t *queue) {
    assert((queue->head == NULL) == (queue->tail == NULL));

    if (queue->head == NULL) {
        return NULL;
    }

    queue_node_t *node = queue->head;

    if (node->next == NULL) {
        queue->head = NULL;
        queue->tail = NULL;
    } else {
        queue->head = node->next;
    }

    return node;
}
