#ifndef MAIN_H
#define MAIN_H

#include "accelerometer.h"
#include "buttons.h"
#include "dht_11.h"
#include "oled_init.h"
#include "speaker.h"
#include "error_functions.h"

typedef struct  s_data
{
    spi_device_handle_t spi;
    uint8_t             tem;    // last temperature value
    uint8_t             hum;    // last humidity value

    _Bool               is_page_two;    // current displayed page
    _Bool               is_updt_tem;    // do i need to update the temperature?
    _Bool               is_updt_hum;    // do i need to update the humidity?
    _Bool               is_down;        // is board down?

    t_button            btn1;
    t_button            btn2;
    t_oled              oled;

}               t_data;

void check_buttons(void *db);
void check_dht11(void *db);
void check_print_oled(void *db);
void check_accelerometer(void *db);

#endif
