/* Hello World Example

   This example code is in the Public Domain (or CC0 licensed, at your option.)

   Unless required by applicable law or agreed to in writing, this
   software is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR
   CONDITIONS OF ANY KIND, either express or implied.
*/
#include <stdio.h>
#include <stdlib.h>
#include "driver/gpio.h"

void print_error(void)
{
    printf("Error: GPIO_NUM is invalid\n");
    exit(1);
}

void app_main()
{  
    if (gpio_set_direction(GPIO_NUM_26, GPIO_MODE_OUTPUT) == ESP_ERR_INVALID_ARG)
        print_error();
    if (gpio_set_direction(GPIO_NUM_27, GPIO_MODE_OUTPUT) == ESP_ERR_INVALID_ARG)
        print_error();
    if (gpio_set_direction(GPIO_NUM_33, GPIO_MODE_OUTPUT) == ESP_ERR_INVALID_ARG)
        print_error();
    while (true)
    {
        if (gpio_set_level(GPIO_NUM_26, 1) == ESP_ERR_INVALID_ARG) 
            print_error();     
        if (gpio_set_level(GPIO_NUM_27, 1) == ESP_ERR_INVALID_ARG) 
            print_error();     
        if (gpio_set_level(GPIO_NUM_33, 1) == ESP_ERR_INVALID_ARG) 
            print_error();       
    }
}
