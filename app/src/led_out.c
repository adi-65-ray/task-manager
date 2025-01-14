#include <stdio.h>
#include <pico/stdlib.h>
#include <hardware/gpio.h>
#include <pico/binary_info.h>
#include <FreeRTOS.h>
#include <task.h>
#include "led_out.h"
#include "serial_in.h"

const uint LED_PIN = PICO_DEFAULT_LED_PIN;

void led_init()
{
    printf("led init\n");
    gpio_init(LED_PIN);
    gpio_set_dir(LED_PIN, GPIO_OUT);
    gpio_put(LED_PIN, 0);
}

void led_main(void* arg) 
{
    int led_rate = INVALID_INPUT;
    printf("LED main\n");
    while (true) 
    {
        (void)frequency(&led_rate);
        if (led_rate <= INVALID_INPUT) {
            gpio_put(LED_PIN, 0);
        }
        else
        {
            gpio_put(LED_PIN, 1);
            printf("LED on\n");
            vTaskDelay(pdMS_TO_TICKS(led_rate));
            gpio_put(LED_PIN, 0);
            printf("LED off\n");
            vTaskDelay(pdMS_TO_TICKS(led_rate));
        }   
    }
}

void led_deinit()
{
    printf("Serial deinit\n");
    gpio_put(LED_PIN, 0);
}