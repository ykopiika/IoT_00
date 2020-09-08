#ifndef ACCELEROMETER_ACCELEROMETER_H
#define ACCELEROMETER_ACCELEROMETER_H

#include <stdio.h>
#include <stdlib.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_system.h"
#include "driver/spi_master.h"
#include "driver/gpio.h"
#include "driver/dac.h"
#include "math.h"

#define AMP_POWER_PIN 5
#define DELAY 25
#define STEP 30

#define ADXL_HOST  VSPI_HOST
#define DMA_CHAN  0
#define PIN_NUM_MISO  12
#define PIN_NUM_MOSI  13
#define PIN_NUM_CLK  14
#define PIN_NUM_CS  15

#define MAGIC_CONST 0.03

//void make_bip(_Bool status);
//_Bool check_flip(double x, double y, double z);

void accel_init (spi_device_handle_t *spi);
//void adxl345_start (spi_device_handle_t spi);
void adxl345_read_acceleration (spi_device_handle_t spi, int16_t* accs);

#endif
