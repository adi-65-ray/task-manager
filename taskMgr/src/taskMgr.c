#include <stdio.h>
#include <string.h>
#include <pico/stdlib.h>
#include <hardware/gpio.h>
#include <pico/binary_info.h>
#include <FreeRTOS.h>
#include <task.h>
#include "task_cfg.h"
#include "taskMgr.h"


static TaskConfig taskConfigs[TOTAL_TASKS] = {
    TASK_LIST
    #ifdef HEALTH_MONITOR
    {"eHEALTH_MONITOR",     1,      1024,       health_monitor_init,    health_monitor_main,        health_monitor_deinit,      NULL},
    #endif
};

#ifdef THREAD_LOCAL
static uint8_t task_index[TOTAL_TASKS] = {INVALID_TASK_INDEX};
#endif

void task_init_all()
{
    BaseType_t task_err;
    #ifdef THREAD_LOCAL
    uint8_t* index = NULL;
    #endif

    stdio_init_all();
    appASSERT((TOTAL_TASKS >= sizeof(taskConfigs) / sizeof(TaskConfig)), "Task count mismatch");
    sleep_ms(1000);
    
    for (int i = 0; i < TOTAL_TASKS; i++) 
    {
        if (taskConfigs[i].initFunction != NULL) {
            taskConfigs[i].initFunction();
            printf("initialised task %s\n", taskConfigs[i].taskName);
        }

        #ifdef THREAD_LOCAL
        task_index[i] = i;
        index = &task_index[i];

        task_err = xTaskCreate( \
        taskConfigs[i].mainFunction, \
        taskConfigs[i].taskName, \
        taskConfigs[i].stackSize, \
        (void*)index,\
        taskConfigs[i].priority, \
        &(taskConfigs[i].task_handle)\
        );

        if (task_err == pdPASS) {
            // Store the configuration pointer in the task's local storage
            vTaskSetThreadLocalStoragePointer(taskConfigs[i].task_handle, 0, (void*)index);
            printf("TLS creates of task: %s at index: %d\n", taskConfigs[i].taskName, *index);
        } else {
            // Task creation failed, free the allocated memory
            printf("Failed to create task %s\n", taskConfigs[i].taskName);
            task_index[i] = INVALID_TASK_INDEX;
        }
        #else
        task_err = xTaskCreate( \
        taskConfigs[i].mainFunction, \
        taskConfigs[i].taskName, \
        taskConfigs[i].stackSize, \
        NULL,\
        taskConfigs[i].priority, \
        &(taskConfigs[i].task_handle)\
        );

        if (task_err != pdPASS) {
            printf("Failed to create task %s\n", taskConfigs[i].taskName);
        }
        #endif
    }
}

void task_deinit_all()
{
    for (int i = 0; i < TOTAL_TASKS; i++) 
    {
        if (taskConfigs[i].task_handle != NULL) {
            if (taskConfigs[i].deinitFunction != NULL) {
                taskConfigs[i].deinitFunction();
                printf("deinitialised task %s\n", taskConfigs[i].taskName);
            }
            #ifdef THREAD_LOCAL
            uint8_t* index = pvTaskGetThreadLocalStoragePointer(taskConfigs[i].task_handle, 0);
            *index = INVALID_TASK_INDEX;
            #endif
            vTaskDelete(taskConfigs[i].task_handle);
            printf("Delete task %s\n", taskConfigs[i].taskName);
        }
    }
}

inline TaskHandle_t get_task_handle(TaskConfigIndex* const index)
{
    return taskConfigs[(int)*index].task_handle;
}

#ifdef THREAD_LOCAL
void task_init_single(uint8_t* const task_index)
{
    uint8_t index = *task_index;
    BaseType_t task_err;

    if(index >= INVALID_TASK_INDEX) {
        printf("Invalid task index\n");
        return;
    }
    
    if (taskConfigs[index].initFunction != NULL) {
        taskConfigs[index].initFunction();
        printf("initialised task %s\n", taskConfigs[index].taskName);
    }

    task_err = xTaskCreate( \
    taskConfigs[index].mainFunction, \
    taskConfigs[index].taskName, \
    taskConfigs[index].stackSize, \
    (void*)task_index, \
    taskConfigs[index].priority, \
    &(taskConfigs[index].task_handle)\
    );

    if (task_err == pdPASS) {
            // Store the configuration pointer in the task's local storage
        vTaskSetThreadLocalStoragePointer(taskConfigs[index].task_handle, 0, (void*)task_index);
    } else {
        // Task creation failed, free the allocated memory
        printf("Failed to create task %s\n", taskConfigs[index].taskName);
        *task_index = INVALID_TASK_INDEX;
    }
    
}

void task_deinit_single(uint8_t* const get_task_index)
{
    uint8_t index = *get_task_index;

    if(index >= INVALID_TASK_INDEX) {
        printf("Invalid task index\n");
        return;
    }

    if (taskConfigs[index].task_handle != NULL) {
        if (taskConfigs[index].deinitFunction != NULL) {
            taskConfigs[index].deinitFunction();
            printf("deinitialised task %s\n", taskConfigs[index].taskName);
        }
        *get_task_index = INVALID_TASK_INDEX;
        vTaskDelete(taskConfigs[index].task_handle);
        printf("Delete task %s\n", taskConfigs[index].taskName);
    }
}
#else
void task_init_single(uint8_t* const task_index)
{

}

void task_deinit_single(uint8_t* const get_task_index)
{

}
#endif

