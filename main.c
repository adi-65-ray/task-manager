#include <stdio.h>
#include <pico/stdlib.h>
#include <hardware/gpio.h>
#include <pico/binary_info.h>
#include "stubs.h"
#ifndef LED_TEST
#include "taskMgr.h"
#endif

#ifdef LED_TEST
const uint8_t LED_PIN = PICO_DEFAULT_LED_PIN;
#endif

int main() 
{
    stdio_init_all();

    #ifdef LED_TEST
    printf("led init\n");
    gpio_init(LED_PIN);
    gpio_set_dir(LED_PIN, GPIO_OUT);
    gpio_put(LED_PIN, 0);

    while (true) 
    {
        gpio_put(LED_PIN, 1);
        printf("LED on\n");
        sleep_ms(500);
        gpio_put(LED_PIN, 0);
        printf("LED off\n");
        sleep_ms(500);
    }
    #else
    sleep_ms(1000);
    printf("Number of tasks %d\n", TOTAL_TASKS);
    
    printf("task initialising\n");
    task_init_all();
    vTaskStartScheduler();
    task_deinit_all();
    
    printf("scheduler stopped\n");
    #endif
    return 0;
}

