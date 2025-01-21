#include <stdio.h>
#include <pico/stdlib.h>
#include <hardware/gpio.h>
#include <pico/binary_info.h>
#include "stubs.h"
#ifndef LED_TEST
#include "taskMgr.h"
#endif

int main() 
{
    stdio_init_all();
    sleep_ms(1000);

    #ifdef LED_TEST
    printf("led init\n");
    gpio_init(PICO_DEFAULT_LED_PIN);
    gpio_set_dir(PICO_DEFAULT_LED_PIN, GPIO_OUT);
    gpio_put(PICO_DEFAULT_LED_PIN, 0);

    while (true) 
    {
        gpio_put(PICO_DEFAULT_LED_PIN, 1);
        printf("LED on\n");
        sleep_ms(500);
        gpio_put(PICO_DEFAULT_LED_PIN, 0);
        printf("LED off\n");
        sleep_ms(500);
    }
    #else
    
    printf("Number of tasks %d\n", TOTAL_TASKS);
    printf("Wait for 10 sec...\n");
    sleep_ms(10000);
    
    printf("task initialising\n");
    task_init_all();
    vTaskCoreAffinitySet(NULL, (1 << 0));
    vTaskStartScheduler();
    task_deinit_all();
    
    printf("scheduler stopped\n");
    #endif
    return 0;
}

