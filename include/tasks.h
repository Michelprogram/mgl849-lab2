#ifndef TASKS_H
#define TASKS_H

#include <pthread.h>
#include "shared_data.h"
#include "socket.h"  

//Tasks 
// socket_receiver.c   (T1)
// decision_engine.c   (T2)
// socket_sender.c     (T3)
// led_display.c       (T4)
// metrics.c           (T5)

/* Ex: typedef struct {
    SharedData *data;
    int i2c_fd;
    Socket *sock;
} SensorTaskArgs; */

#endif
