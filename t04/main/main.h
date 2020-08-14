#ifndef BUTTON_MAIN_H
#define BUTTON_MAIN_H

#include <stdio.h>
#include <stdlib.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/gpio.h"
#include <driver/dac.h>
#include <unistd.h>
#include <string.h>

#define LED_1 GPIO_NUM_27
#define LED_2 GPIO_NUM_26
#define BUTTON_1 GPIO_NUM_39
#define BUTTON_2 GPIO_NUM_18
#define DH11_DATA_PIN 4
#define DH11_POWER_PIN 2

#endif
