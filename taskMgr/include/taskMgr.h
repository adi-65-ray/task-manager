#ifndef TASKMGR_H
#define TASKMGR_H

#include "task_cfg.h"

#define INVALID_TASK_INDEX TOTAL_TASKS
#define appASSERT(x,y)\
    if( (( y ) != NULL ) && !x ) { \
        printf("Assertion failed: %s\n",y); \
        taskDISABLE_INTERRUPTS(); \
    }\
    else { \
        taskDISABLE_INTERRUPTS(); \
    }\
    configASSERT(x);

extern TaskHandle_t get_task_handle(TaskConfigIndex* const index);

// Function declarations
extern void task_init_all(void);
extern void task_deinit_all(void);

extern void task_init_single(uint8_t* const task_index);
extern void task_deinit_single(uint8_t* const task_index);

#endif // TASKMGR_H