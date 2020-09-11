#include "oled.h"
#include "font6x8.h"

static void set_commands(t_oled *oled)
{
    if (!oled)
        ESP_ERROR_CHECK(ESP_ERR_INVALID_ARG);
    i2c_cmd_handle_t cmd = i2c_cmd_link_create();
    ESP_ERROR_CHECK(i2c_master_start(cmd));
    ESP_ERROR_CHECK(i2c_master_write_byte(cmd,
                        (oled->addr << 1U) | I2C_MASTER_WRITE, true));
    ESP_ERROR_CHECK(i2c_master_write_byte(cmd, 0x00, true)); // command stream
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
    ESP_ERROR_CHECK(i2c_master_stop(cmd));
    ESP_ERROR_CHECK(i2c_master_cmd_begin(oled->port, cmd, 10 / portTICK_PERIOD_MS));
    i2c_cmd_link_delete(cmd);
}

void display_pixels(t_oled *oled)
{
    if (!oled)
        ESP_ERROR_CHECK(ESP_ERR_INVALID_ARG);
    i2c_cmd_handle_t cmd = i2c_cmd_link_create();
    ESP_ERROR_CHECK(i2c_master_start(cmd));
    ESP_ERROR_CHECK(i2c_master_write_byte(cmd, (oled->addr << 1) | I2C_MASTER_WRITE, true));
    ESP_ERROR_CHECK(i2c_master_write_byte(cmd, 0x80, true)); // single command
    ESP_ERROR_CHECK(i2c_master_write_byte(cmd, 0x00, true)); // page number
    ESP_ERROR_CHECK(i2c_master_write_byte(cmd, 0x40, true)); // data stream
    ESP_ERROR_CHECK(i2c_master_write(cmd, oled->pixels, sizeof(oled->pixels), true));
    ESP_ERROR_CHECK(i2c_master_stop(cmd));
    print_error(i2c_master_cmd_begin(oled->port, cmd, 10 / portTICK_PERIOD_MS),
                __func__, __LINE__, "master_cmd_begin failed");
    i2c_cmd_link_delete(cmd);
}

void put_pixel(uint8_t *pixels, int16_t x, uint16_t y, _Bool color)
{
    if (!pixels)
        ESP_ERROR_CHECK(ESP_ERR_INVALID_ARG);
    if (color == true)
        pixels[x + ((y >> 3U) * LCDWIDTH)] |=  (1U << (y & 7U));
    else
        pixels[x + ((y >> 3U) * LCDWIDTH)] &= ~(1U << (y & 7U));
}

static void char_to_pixels(uint8_t *pixels, int *pixels_index, int char_index)
{
    pixels[(*pixels_index)++] = font6x8[char_index++];
    pixels[(*pixels_index)++] = font6x8[char_index++];
    pixels[(*pixels_index)++] = font6x8[char_index++];
    pixels[(*pixels_index)++] = font6x8[char_index++];
    pixels[(*pixels_index)++] = font6x8[char_index++];
    pixels[(*pixels_index)] = font6x8[char_index];
}

void str_to_oled(t_oled *oled, char *str)
{
    if (!oled || !str)
        ESP_ERROR_CHECK(ESP_ERR_INVALID_ARG);
    int len = strlen(str);
    int calc = LCDWIDTH - (len * 6);
    int x = (calc > 0) ? (calc / 2) : 0;
    int y = 3;
    for (int i = 0; (i < len) && (i < 168); ++i) {
        int pixels_index = (y * LCDWIDTH) + x;
        int char_index = (str[i] - 32) * 6;
        char_to_pixels(oled->pixels, &pixels_index, char_index);
        x += 6;
        if (x >= 126)
        {
            x = 0;
            y++;
        }
    }
    display_pixels(oled);
}

static void init_i2c(void)
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

void clear_oled(t_oled *oled) //TODO: rename function
{
    if (!oled)
        ESP_ERROR_CHECK(ESP_ERR_INVALID_ARG);
    memset(oled->pixels, 0, sizeof(oled->pixels));
}

void init_oled(t_oled *oled)
{
    if (!oled)
        ESP_ERROR_CHECK(ESP_ERR_INVALID_ARG);
    *oled = (t_oled){.addr = OLED_ADDR, .port = I2C_PORT, .pixels = {0}};
    ESP_ERROR_CHECK(gpio_set_direction(OLED_EN, GPIO_MODE_OUTPUT));
    ESP_ERROR_CHECK(gpio_set_level(OLED_EN, 1));
    ets_delay_us(1000000);
    init_i2c();
    set_commands(oled);
    clear_oled(oled);
    display_pixels(oled);
}
