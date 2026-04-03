#ifndef INTRUSIVE_QUEUE_H
#define INTRUSIVE_QUEUE_H

typedef struct queue_node {
    struct queue_node *next;
} queue_node_t;

typedef struct {
    queue_node_t *head;
    queue_node_t *tail;
} queue_t;

/*
 * queue_init - Initialize a queue.
 * @queue: Pointer to the queue to initialize.
 */
void queue_init(queue_t *queue);

/*
 * queue_enqueue - Enqueue a node into the queue.
 * @queue: Pointer to the queue to enqueue into.
 * @node: Pointer to the node to enqueue.
 */
void queue_enqueue(queue_t *queue, queue_node_t *node);

/*
 * queue_dequeue - Dequeue a node from the queue.
 * @queue: Pointer to the queue to dequeue from.
 * Returns a pointer to the dequeued node, or NULL if the queue is empty.
 */
queue_node_t *queue_dequeue(queue_t *queue);

#endif
