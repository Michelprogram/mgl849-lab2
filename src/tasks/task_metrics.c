
#define _GNU_SOURCE
#include "shared_data.h"
#include "config.h"
#include "metrics.h"
#include <stdio.h>
#include <time.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>
#include <stdlib.h>

void *task_metrics(void *arg) {
    SharedData *shared = (SharedData *)arg;

    FILE *f = fopen(METRIC_FILE_PATH, "ab+");
    if (!f) fprintf(stderr, "Metrics: failed to open %s\n", METRIC_FILE_PATH);

    printf("Metrics thread booted\n");
    
    if (f) { fprintf(f, "----------------------------------------------\n"); fflush(f); }

    while (1) {
        sem_wait(&shared->sem_new_metrics);

        pthread_mutex_lock(&shared->lock);
        metrics_snapshot_t snap = shared->metrics_snap;
        pthread_mutex_unlock(&shared->lock);

        struct timespec t_now;
        clock_gettime(CLOCK_MONOTONIC, &t_now);

        double elapsed_ms =
            (double)(t_now.tv_sec  - snap.ts.tv_sec)  * 1000.0 +
            (double)(t_now.tv_nsec - snap.ts.tv_nsec) / 1.0e6;
        
        int cost = compute_cost(snap.cmd);
        
        double efficiency = compute_efficiency(snap.gas_prev, snap.gas_values);

        struct timespec t_wall;
        clock_gettime(CLOCK_REALTIME, &t_wall);
        char timestr[32];
        struct tm tm_info;
        localtime_r(&t_wall.tv_sec, &tm_info);
        strftime(timestr, sizeof(timestr), "%Y-%m-%d %H:%M:%S", &tm_info);

        if (f) {
            fprintf(f, "%s Gas%d alarm=%d value=%d elapsed_ms=%.3f cost=%d efficiency=%.4f action=%s\n",
                    timestr, (int)snap.gas, (int)snap.alarm,
                    snap.value, elapsed_ms, cost, efficiency, snap.cmd);
            fflush(f);
        }
        
    }

    fclose(f);
    return NULL;
}