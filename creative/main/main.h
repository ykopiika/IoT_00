#ifndef MAIN_H
#define MAIN_H

#include "accelerometer.h"
#include "buttons.h"
#include "dht_11.h"
#include "oled.h"
#include "speaker.h"
#include "color.h"

typedef struct  s_data
{
    spi_device_handle_t spi;
    uint8_t             tem;
    uint8_t             hum;

    _Bool               is_page_two;
    _Bool               is_updt_tem;
    _Bool               is_updt_hum;
    _Bool               is_down;

    t_button            btn1;
    t_button            btn2;
    t_oled              oled;

}               t_data;

void check_buttons(void *db);
void check_dht11(void *db);
void check_print_oled(void *db);
void check_accelerometer(void *db);

#endif
