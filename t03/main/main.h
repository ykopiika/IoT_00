#ifndef BUTTON_MAIN_H
#define BUTTON_MAIN_H

#include <stdio.h>
#include <stdlib.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/gpio.h"
#include <driver/dac.h>

#define LED_1 GPIO_NUM_27
#define LED_2 GPIO_NUM_26
#define BUTTON_1 GPIO_NUM_39
#define BUTTON_2 GPIO_NUM_18

typedef struct  s_button {
    _Bool   flag;
    _Bool   is_press;
    _Bool   is_on;
    int     led;
}               t_button;

#endif
