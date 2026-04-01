#include "shared_data.h"

#include <pthread.h>
#include <stdlib.h>

#include "container_of.h"
#include "queue.h"

int shared_data_init(SharedData *data) {
    queue_init(&data->messages);

    pthread_mutex_init(&data->lock, NULL);

    if (sem_init(&data->sem_new_data, 0, 0) != 0) {
        return -1;
    }

    return 0;
}

void shared_data_destroy(SharedData *data) {
    queue_node_t *node = queue_dequeue(&data->messages);
    while (node != NULL) {
        queue_node_t *next = node->next;

        gas_parsed_t *gas_parsed = container_of(node, gas_parsed_t, node);
        free(gas_parsed);

        node = next;
    }

    sem_destroy(&data->sem_new_data);

    pthread_mutex_destroy(&data->lock);
}
