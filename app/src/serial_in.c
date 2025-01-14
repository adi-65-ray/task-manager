#include <stdio.h>
#include <pico/stdlib.h>
#include <hardware/gpio.h>
#include <pico/binary_info.h>
#include <FreeRTOS.h>
#include <task.h>
#include "serial_in.h"

#define MAX_MESSAGE_LENGTH 10

static int led_rate = INVALID_INPUT;

void serial_init()
{
    printf("Serial init\n");
    led_rate = INVALID_INPUT;
    uart_init(uart0, 115200);
}

void serial_deinit()
{
    printf("Serial deinit\n");
    led_rate = INVALID_INPUT;
    uart_deinit(uart0);
}

void input_serial_main(void* arg) 
{
    printf("Input serial main\n");
    char message[MAX_MESSAGE_LENGTH] = {'\0'};
    int message_pos = 0;
    int fl_led_rate = 0;
    while (true) 
    {
        if (uart_is_readable(uart0)) 
        {
            int c = getchar();
            if (c != '\n' && c != '\r') 
            {
                if (message_pos < (MAX_MESSAGE_LENGTH - 1)) 
                {
                    if (c >='0' && c <= '9')
                    {
                        message[message_pos++] = c;
                        fl_led_rate = fl_led_rate * 10 + (c - '0');
                    }
                    else
                    {
                        printf("Invalid input\n");
                        fl_led_rate = 0;
                    }
                }
                else
                {
                    printf("Message too long\n");
                    fl_led_rate = 0;
                }
            } 
            else 
            {
                if (fl_led_rate < INVALID_INPUT) 
                {
                    printf("input Invalid\n");
                }
                else
                {
                    led_rate = fl_led_rate;
                }
                message[message_pos++] = '\0';
                printf("Received: %s\n", message);
                message_pos = 0;
                fl_led_rate = 0;
            }
        }
        else
        {
            vTaskDelay(pdMS_TO_TICKS(10));
        }
    }

}

void frequency(int* const freq_out)
{
    *freq_out = led_rate;
}