#include <stdio.h>
#include <pico/stdlib.h>
#include <FreeRTOS.h>
#include <task.h>
#include "health_monitor_task.h"
#include "task_cfg.h"
#include "taskMgr.h"

static TickType_t idle_start_tick;
static uint32_t total_idle_ticks;

void health_monitor_init() {
    printf("health monitor init\n");
    idle_start_tick = xTaskGetTickCount();
    total_idle_ticks = 0;
}

void health_monitor_deinit() {
    printf("health monitor init\n");
    idle_start_tick = xTaskGetTickCount();
    total_idle_ticks = 0;
}

void vApplicationMallocFailedHook(void) {
    fflush(stdout);
    printf("Malloc failed\n");
    //task_deinit_all();
    taskDISABLE_INTERRUPTS();
    for( ;; );
}

void vApplicationStackOverflowHook(TaskHandle_t xTask, char *pcTaskName) {
    fflush(stdout);
    printf("Stack overflow in task %s\n", pcTaskName);
    #ifdef THREAD_LOCAL
    uint8_t* index = (uint8_t*)pvTaskGetThreadLocalStoragePointer(xTask, 0);
    task_deinit_single(index);
    //task_init_single(index);
    #else
    taskDISABLE_INTERRUPTS();
    for( ;; );
    #endif
}

void vApplicationIdleHook(void) {
    // This hook is called when the scheduler has no tasks to run.
    // It can be used to put the CPU in a low power state.
    fflush(stdout);
    //idle_start_tick = xTaskGetTickCount();
    //printf("Idle hook\n");
}

void vApplicationTickHook(void) {
    // This hook is called from the tick interrupt.
    // It can be used to perform periodic processing.
    /*
    if (idle_start_tick != 0) {
        total_idle_ticks += ((uint32_t)xTaskGetTickCount() - idle_start_tick);
        idle_start_tick = 0;
    }*/
    //printf("Tick hook\n");
}

void monitor_tasks(TaskHandle_t xTask) {
    uint8_t min_stack_remains = (uint8_t)uxTaskGetStackHighWaterMark(xTask);
    printf("Task %s stack remaining (in bytes): %d\n", pcTaskGetName(xTask), min_stack_remains);
}

void health_monitor_main(void* args) {
    printf("Health monitor main\n");
    int min_heap_size = 0;
    TaskHandle_t task_handle = NULL;
    while (1)
    {
        printf("Health monitor task.......\n");
        vTaskDelay(pdMS_TO_TICKS(1000));
        min_heap_size = xPortGetMinimumEverFreeHeapSize();
        /*
        uint32_t total_ticks = (uint32_t) xTaskGetTickCount();
        uint32_t cpu_usage = (uint32_t) (((total_ticks) - total_idle_ticks)/total_ticks) * 100;
        printf("Total cpu time: %d \n", cpu_usage);
        */
        printf("Minimum heap size free: %d bytes\n", min_heap_size);
        for (int i = 0; i < TOTAL_TASKS; i++) {
            task_handle = (TaskHandle_t)get_task_handle((TaskConfigIndex*)&i);
            if (task_handle != NULL) {
                monitor_tasks(task_handle);
            }
        }
        
    }
}
