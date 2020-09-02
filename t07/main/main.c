#include <stdio.h>
#include <string.h>
#include "esp_log.h"
#include "driver/gpio.h"
#include "driver/i2c.h"

#include "sh1106.h"
#include "font6x8.h"

#define SDA_PIN  GPIO_NUM_21
#define SCL_PIN  GPIO_NUM_22
#define I2C_ADDR SH1106_DEFAULT_ADDR
#define I2C_PORT SH1106_DEFAULT_PORT

#define LCDWIDTH       128
#define LCDHEIGHT       8
uint8_t buffer[LCDWIDTH * LCDHEIGHT];

//static void print_error(char *str)
//{
//    printf("Error: %s\n", str);
//    exit(1);
//}

void write_page(uint8_t data, uint8_t page) {
    i2c_cmd_handle_t cmd = i2c_cmd_link_create();
    i2c_master_start(cmd);
    i2c_master_write_byte(cmd, (SH1106_DEFAULT_ADDR << 1) | I2C_MASTER_WRITE, true);
    i2c_master_write_byte(cmd, 0x80, true); // single command
    i2c_master_write_byte(cmd, 0xB0 + page, true); // page number
    i2c_master_write_byte(cmd, 0x40, true); // data stream
    i2c_master_write(cmd, &data, 1, true);
    i2c_master_stop(cmd);
    i2c_master_cmd_begin(SH1106_DEFAULT_PORT, cmd, 10 / portTICK_PERIOD_MS);
    i2c_cmd_link_delete(cmd);
}
void write_lcd(uint8_t *data) {
    i2c_cmd_handle_t cmd = i2c_cmd_link_create();
    i2c_master_start(cmd);
    i2c_master_write_byte(cmd, (SH1106_DEFAULT_ADDR << 1) | I2C_MASTER_WRITE, true);
    i2c_master_write_byte(cmd, 0x80, true); // single command
    i2c_master_write_byte(cmd, 0x00, true); // page number
    i2c_master_write_byte(cmd, 0x40, true); // data stream
    i2c_master_write(cmd, data, (LCDWIDTH * LCDHEIGHT), true);
    i2c_master_stop(cmd);
    i2c_master_cmd_begin(SH1106_DEFAULT_PORT, cmd, 10 / portTICK_PERIOD_MS);
    i2c_cmd_link_delete(cmd);
}

void draw_pixel(int16_t x, int16_t y, _Bool color)
{
//    y = y/8;
    if (color == true)
        buffer[x+ ((y/8)*LCDWIDTH)] |=  (1 << (y&7));
    else
        buffer[x+ ((y/8)*LCDWIDTH)] &= ~(1 << (y&7));
}

/*
    Initialize I2C driver.
*/
void init_i2c() {
    i2c_config_t i2c_config = {
            .mode = I2C_MODE_MASTER,
            .sda_io_num = SDA_PIN,
            .scl_io_num = SCL_PIN,
            .sda_pullup_en = GPIO_PULLUP_ENABLE,
            .scl_pullup_en = GPIO_PULLUP_ENABLE,
            .master.clk_speed = 1000000
    };
    i2c_param_config(I2C_PORT, &i2c_config);
    i2c_driver_install(I2C_PORT, I2C_MODE_MASTER, 0, 0, 0);
}

void str_to_buf(uint8_t *data, char *str)
{
    int len = strlen(str);
    if (len <= 168)
    {
        int x = 0;
        int y = 0;
        for (int i = 0; i < len; ++i) {
            int num_buf = (y * LCDWIDTH) + x;
            int font_buf = (str[i] - 32) * 6;
            data[num_buf++] = font6x8[font_buf++];
            data[num_buf++] = font6x8[font_buf++];
            data[num_buf++] = font6x8[font_buf++];
            data[num_buf++] = font6x8[font_buf++];
            data[num_buf++] = font6x8[font_buf++];
            data[num_buf++] = font6x8[font_buf++];
            x += 6;
            if (x >= 126)
            {
                x = 0;
                y++;
            }
            write_lcd(buffer);
            ets_delay_us(70000);
        }
    }
    else {
        str_to_buf(data, "NON");
    }
}

void app_main() {
    gpio_set_direction(GPIO_NUM_32, GPIO_MODE_OUTPUT);
    gpio_set_level(GPIO_NUM_32, 1);
    ets_delay_us(1000000);
    init_i2c();
    sh1106_t display;
    display.addr = I2C_ADDR;
    display.port = I2C_PORT;
    sh1106_init(&display);
    sh1106_clear(&display);
    sh1106_update(&display);


    memset(&buffer, 0, sizeof(buffer));
//    str_to_buf(buffer,
//               "All the greatest technological inventions created by man - the plane, the car, the computer - are more indicative of his laziness than of high intelligence.");
    write_lcd(buffer);
    for (int i = 0; i < 64; ++i) {
        for (int j = 0; j < 124; ++j) {
            draw_pixel(j,i,1);
            write_lcd(buffer);
        }
    }
}











//void sh1106_init(i2c_port_t i2c_num){
//    i2c_cmd(i2c_num,0xAE);//--turn off oled panel
//    i2c_cmd(i2c_num,0x00);//---set low column address
//    i2c_cmd(i2c_num,0x10);//---set high column address0-131 X
//    i2c_cmd(i2c_num,0x40);//--set start line address 0-63  Y
//    i2c_cmd(i2c_num,0x81);//--set contrast control register
//    i2c_cmd(i2c_num,0xFF);
//    i2c_cmd(i2c_num,0xA1);//--Set SEG/Column Mapping     a1/a0
//    i2c_cmd(i2c_num,0xC8);//Set COM/Row Scan Direction   c8/c0
//    i2c_cmd(i2c_num,0xA6);//--set normal display
//    i2c_cmd(i2c_num,0xA8);//--set multiplex ratio(1 to 64)
//    i2c_cmd(i2c_num,0x3F);//--1/64 duty
//    i2c_cmd(i2c_num,0xD3);//-set display offset	Shift Mapping RAM Counter
//    i2c_cmd(i2c_num,0x00);//-not offset
//    i2c_cmd(i2c_num,0xD5);//--set display clock divide ratio/oscillator frequency
//    i2c_cmd(i2c_num,0x50);//--set divide ratio, Set Clock as 100 Frames/Sec
//    i2c_cmd(i2c_num,0xD9);//--set pre-charge period
//    i2c_cmd(i2c_num,0x22);//Set Pre-Charge as 15 Clocks & Discharge as 1 Clock
//    i2c_cmd(i2c_num,0xDA);//--set com pins hardware configuration
//    i2c_cmd(i2c_num,0x12);
//    i2c_cmd(i2c_num,0xDB);//--set vcomh
//    i2c_cmd(i2c_num,0x40);//Set VCOM Deselect Level
//    i2c_cmd(i2c_num,0xA4);// Disable Entire Display On (0xa4/0xa5)
//    i2c_cmd(i2c_num,0xA6);// Disable Inverse Display On (0xa6/a7)
//    i2c_cmd(i2c_num,0xAF);//--turn on oled panel
//}
