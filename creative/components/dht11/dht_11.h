#ifndef BUTTON_DHT_11_H
#define BUTTON_DHT_11_H

#include <stdio.h>
#include <string.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/gpio.h"

#define DH11_DATA_PIN 4
#define DH11_POWER_PIN 2

void dht11_init(void);
void get_value_dht11(uint8_t *temperature, uint8_t *humidity);

#endif
