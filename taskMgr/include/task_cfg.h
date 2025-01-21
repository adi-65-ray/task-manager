#ifndef TASK_CFG_H
#define TASK_CFG_H

#include <FreeRTOS.h>
#include <task.h>
#include "led_out.h"
#include "serial_in.h"
#include "health_monitor_task.h"


#define THREAD_LOCAL
#define HEALTH_MONITOR 

// Task configuration structure
typedef struct TaskConfig {
    const char* taskName;
    int priority;
    int stackSize;
    void (*initFunction)(void);
    void (*mainFunction)(void*);
    void (*deinitFunction)(void);
    TaskHandle_t task_handle;
} TaskConfig;

typedef enum TaskConfigIndex {
    eLED_TASK,
    eINPUT_TASK,
    #ifdef HEALTH_MONITOR
    eHEALTH_MONITOR,
    #endif
    TOTAL_TASKS
}TaskConfigIndex;

// Example task configurations
/*  {Task Name, Priority, Stack size, Init function, Main fucntion, DeInit function, Task Handle(NULL)} */
#define TASK_LIST \
    {"eLED_TASK",           1,      1024,       led_init,               led_main,                   led_deinit,                 NULL},\
    {"eINPUT_TASK",         1,      1024,       serial_init,            input_serial_main,          serial_deinit,              NULL},

#endif // TASK_CFG_H