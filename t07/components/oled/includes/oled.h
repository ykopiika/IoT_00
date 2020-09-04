#ifndef OLED_H
#define OLED_H

#include <stdio.h>
#include <string.h>
#include "esp_log.h"
#include "driver/gpio.h"
#include "driver/i2c.h"

#define OLED_ADDR   0x3C
#define I2C_PORT    I2C_NUM_0
#define OLED_EN     GPIO_NUM_32
#define SDA_PIN     GPIO_NUM_21
#define SCL_PIN     GPIO_NUM_22

#define LCDWIDTH       128
#define LCDHEIGHT       8

typedef struct  s_oled
{
    uint8_t     addr;
    i2c_port_t  port;
    uint8_t     oled_buf[LCDWIDTH * LCDHEIGHT];
}               t_oled;

//void sh1106_init(sh1106_t *display);
void oled_init(uint8_t addr, i2c_port_t port);
//void sh1106_write_page(sh1106_t *display, uint8_t page);
//void sh1106_update(sh1106_t *display);
//void sh1106_clear(sh1106_t *display);
//void sh1106_fill(sh1106_t *display);
//void sh1106_set(sh1106_t *display, uint8_t x, uint8_t y, bool s);
//bool sh1106_get(sh1106_t *display, uint8_t x, uint8_t y);
void write_lcd(uint8_t *data);
void draw_pixel(int16_t x, uint16_t y, _Bool color);
void str_to_oled(uint8_t *data, char *str);
void init_oled(t_oled *oled);

#endif
