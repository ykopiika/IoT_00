#include "oled.h"
#include "font6x8.h"

//static void print_error(char *str)
//{
//    printf("Error: %s\n", str);
//    exit(1);
//}

void set_settings(i2c_cmd_handle_t cmd)
{
    ESP_ERROR_CHECK(i2c_master_write_byte(cmd, 0xAE, true)); /// 0b10101110 // off
    ESP_ERROR_CHECK(i2c_master_write_byte(cmd, 0xD5, true)); /// 0b11010101 // clock div
    ESP_ERROR_CHECK(i2c_master_write_byte(cmd, 0x80, true)); /// 0b10000000
    ESP_ERROR_CHECK(i2c_master_write_byte(cmd, 0xA8, true)); /// 0b10101000 // multiplex
    ESP_ERROR_CHECK(i2c_master_write_byte(cmd, 0xFF, true)); /// 0b11111111
    ESP_ERROR_CHECK(i2c_master_write_byte(cmd, 0x8D, true)); /// 0b10001101 // charge pump
    ESP_ERROR_CHECK(i2c_master_write_byte(cmd, 0x14, true)); /// 0b00010100
    ESP_ERROR_CHECK(i2c_master_write_byte(cmd, 0x20, true)); /// 0b00100000 // set my mod
    ESP_ERROR_CHECK(i2c_master_write_byte(cmd, 0x00, true)); /// 0b00000000
    ESP_ERROR_CHECK(i2c_master_write_byte(cmd, 0x10, true)); /// 0b00010000 // high column
    ESP_ERROR_CHECK(i2c_master_write_byte(cmd, 0xB0, true)); /// 0b10110000
    ESP_ERROR_CHECK(i2c_master_write_byte(cmd, 0xC8, true)); /// 0b11001000
    ESP_ERROR_CHECK(i2c_master_write_byte(cmd, 0x00, true)); /// 0b00000000 // low column
    ESP_ERROR_CHECK(i2c_master_write_byte(cmd, 0x10, true)); /// 0b00010000
    ESP_ERROR_CHECK(i2c_master_write_byte(cmd, 0x40, true)); /// 0b01000000
    ESP_ERROR_CHECK(i2c_master_write_byte(cmd, 0xA1, true)); /// 0b10100001 // segment remap
    ESP_ERROR_CHECK(i2c_master_write_byte(cmd, 0xA6, true)); /// 0b10100110
    ESP_ERROR_CHECK(i2c_master_write_byte(cmd, 0x81, true)); /// 0b10000001 // contrast
    ESP_ERROR_CHECK(i2c_master_write_byte(cmd, 0xFF, true)); /// 0b11111111
    ESP_ERROR_CHECK(i2c_master_write_byte(cmd, 0xAF, true)); /// 0b10101111 // on
}

void oled_init(uint8_t addr, i2c_port_t port)
{
    i2c_cmd_handle_t cmd = i2c_cmd_link_create();
    ESP_ERROR_CHECK(i2c_master_start(cmd));
    ESP_ERROR_CHECK(i2c_master_write_byte(cmd,
                        (addr << 1) | I2C_MASTER_WRITE, true));
    ESP_ERROR_CHECK(i2c_master_write_byte(cmd, 0x00, true)); // command stream
    set_settings(cmd);
    ESP_ERROR_CHECK(i2c_master_stop(cmd));
    ESP_ERROR_CHECK(i2c_master_cmd_begin(port, cmd, 10 / portTICK_PERIOD_MS));
    i2c_cmd_link_delete(cmd);
}

void sh1106_write_page(sh1106_t *display, uint8_t page) {
    i2c_cmd_handle_t cmd = i2c_cmd_link_create();
    i2c_master_start(cmd);
    i2c_master_write_byte(cmd, (display->addr << 1) | I2C_MASTER_WRITE, true);
    i2c_master_write_byte(cmd, 0x80, true); // single command
    i2c_master_write_byte(cmd, 0xB0 + page, true);
    i2c_master_write_byte(cmd, 0x40, true); // data stream
    i2c_master_write(cmd, display->pages[page], 128, true);
    i2c_master_stop(cmd);
    i2c_master_cmd_begin(display->port, cmd, 10 / portTICK_PERIOD_MS);
    i2c_cmd_link_delete(cmd);
}

void sh1106_update(sh1106_t *display) {
    for (uint8_t i = 0; i < 16; i++) {
        if (display->changes & (1 << i)) {
            sh1106_write_page(display, i);
        }
    }
    display->changes = 0x0000;
}

//void sh1106_clear(sh1106_t *display) {
//    for (uint8_t i = 0; i < 16; i++) {
//        for (uint8_t j = 0; j < 128; j++) {
//            display->pages[i][j] = 0x00;
//        }
//    }
//    display->changes = 0xffff;
//}

void sh1106_fill(sh1106_t *display) {
    for (uint8_t i = 0; i < 16; i++) {
        for (uint8_t j = 0; j < 128; j++) {
            display->pages[i][j] = 0xff;
        }
    }
    display->changes = 0xffff;
}

void sh1106_set(sh1106_t *display, uint8_t x, uint8_t y, bool s) {
    const uint8_t i = y / 8;
    if (s) {
        display->pages[i][x] |= (1 << (y % 8));
    } else {
        display->pages[i][x] &= ~(1 << (y % 8));
    }
    display->changes |= (1 << i);
}

//void sh1106_set(sh1106_t *display, uint8_t x, uint8_t y, bool s) {
//    const uint8_t i = y / 8;
//    if (s) {
//        display->pages[i][x] |= (1 << (y % 8));
//    } else {
//        display->pages[i][x] &= ~(1 << (y % 8));
//    }
//    display->changes |= (1 << i);
//}

bool sh1106_get(sh1106_t *display, uint8_t x, uint8_t y) {
    return display->pages[y / 8][x] & (1 << (y % 8));
}

void write_lcd(uint8_t *data) {
    i2c_cmd_handle_t cmd = i2c_cmd_link_create();
    ESP_ERROR_CHECK(i2c_master_start(cmd));
    ESP_ERROR_CHECK(i2c_master_write_byte(cmd, (OLED_ADDR << 1) | I2C_MASTER_WRITE, true));
    ESP_ERROR_CHECK(i2c_master_write_byte(cmd, 0x80, true)); // single command
    ESP_ERROR_CHECK(i2c_master_write_byte(cmd, 0x00, true)); // page number
    ESP_ERROR_CHECK(i2c_master_write_byte(cmd, 0x40, true)); // data stream
    ESP_ERROR_CHECK(i2c_master_write(cmd, data, (LCDWIDTH * LCDHEIGHT), true));
    ESP_ERROR_CHECK(i2c_master_stop(cmd));
    ESP_ERROR_CHECK(i2c_master_cmd_begin(I2C_PORT, cmd, 10 / portTICK_PERIOD_MS));
    i2c_cmd_link_delete(cmd);
}

void draw_pixel(int16_t x, uint16_t y, _Bool color)
{
    if (color == true)
        buffer[x + ((y >> 3U)*LCDWIDTH)] |=  (1 << (y&7));
    else
        buffer[x + ((y >> 3U)*LCDWIDTH)] &= ~(1 << (y&7));
}

/*
    Initialize I2C driver.
*/


void str_to_oled(uint8_t *data, char *str)
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
            data[num_buf] = font6x8[font_buf];
            x += 6;
            if (x >= 126)
            {
                x = 0;
                y++;
            }
            write_lcd(data);
            ets_delay_us(70000);
        }
    }
    else {
        str_to_oled(data, "NON");
    }
}

void init_i2c(void)
{
    i2c_config_t i2c_config = {
            .mode = I2C_MODE_MASTER,
            .sda_io_num = SDA_PIN,
            .scl_io_num = SCL_PIN,
            .sda_pullup_en = GPIO_PULLUP_ENABLE,
            .scl_pullup_en = GPIO_PULLUP_ENABLE,
            .master.clk_speed = 1000000
    };
    ESP_ERROR_CHECK(i2c_param_config(I2C_PORT, &i2c_config));
    ESP_ERROR_CHECK(i2c_driver_install(I2C_PORT, I2C_MODE_MASTER, 0, 0, 0));
}

void oled_clear(uint8_t addr, i2c_port_t port, uint8_t *buffer)
{
    memset(&buffer, 0, sizeof(buffer));
    write_lcd(buffer);

}

void init_oled(uint8_t *buffer)
{
    ESP_ERROR_CHECK(gpio_set_direction(GPIO_NUM_32, GPIO_MODE_OUTPUT));
    ESP_ERROR_CHECK(gpio_set_level(GPIO_NUM_32, 1));
    ets_delay_us(1000000);
    init_i2c();
    sh1106_t display;
    display.addr = OLED_ADDR;
    display.port = I2C_PORT;
    oled_init(OLED_ADDR, I2C_PORT);
    oled_clear(OLED_ADDR, I2C_PORT, buffer);
//    sh1106_clear(&display);
    sh1106_update(&display);
}