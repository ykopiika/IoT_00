#ifndef BUTTON_BUTTONS_H
#define BUTTON_BUTTONS_H

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/gpio.h"
#include <driver/dac.h>

#define BUTTON_1    GPIO_NUM_39
#define BUTTON_2    GPIO_NUM_18

typedef struct  s_button {
    _Bool       flag;
    _Bool       is_press;
    _Bool       is_on;
}               t_button;

void buttons_init(void);
void toggle_button(t_button *btn);

#endif
