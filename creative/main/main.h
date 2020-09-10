#ifndef MAIN_H
#define MAIN_H

#include "accelerometer.h"
#include "buttons.h"
#include "dht_11.h"
#include "oled.h"
#include "speaker.h"

typedef struct  s_data
{
    _Bool       is_page_two;
    _Bool       is_updt_tem;
    _Bool       is_updt_hum;
    uint8_t     tem;
    uint8_t     hum;

    t_button    btn1;
    t_button    btn2;
    t_oled      oled;
}               t_data;

void check_buttons(void *db);
void check_dht11(void *db);
void check_print_oled(void *db);
void check_accelerometr(void *db);

#endif
