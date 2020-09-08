#ifndef SPEAKER_SPEAKER_H
#define SPEAKER_SPEAKER_H

#include "freertos/FreeRTOS.h"
#include "driver/gpio.h"
#include "driver/dac.h"

#define AMP_POWER_PIN 5
#define DELAY 40
#define STEP 30

void init_speaker(void);
void make_bip(void);

#endif
