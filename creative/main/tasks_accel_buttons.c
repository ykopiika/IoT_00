#include "main.h"

static void board_orientation_settings(i2c_cmd_handle_t cmd, _Bool is_down)
{
    if(is_down)
    {
        ESP_ERROR_CHECK(i2c_master_write_byte(cmd, 0xA1, true));
        ESP_ERROR_CHECK(i2c_master_write_byte(cmd, 0xC8, true));
    }
    else
    {
        ESP_ERROR_CHECK(i2c_master_write_byte(cmd, 0xA0, true));
        ESP_ERROR_CHECK(i2c_master_write_byte(cmd, 0xC0, true));
    }
}

static void change_oled_settings(t_oled *oled, _Bool *is_down)
{
    if (!oled || !is_down)
        ESP_ERROR_CHECK(ESP_ERR_INVALID_ARG);
    *is_down = !*is_down;
    i2c_cmd_handle_t cmd = i2c_cmd_link_create();
    ESP_ERROR_CHECK(i2c_master_start(cmd));
    ESP_ERROR_CHECK(i2c_master_write_byte(cmd,
                         (oled->addr << 1U) | I2C_MASTER_WRITE, true));
    ESP_ERROR_CHECK(i2c_master_write_byte(cmd, 0x00, true));
    ESP_ERROR_CHECK(i2c_master_write_byte(cmd, 0xAE, true));
    board_orientation_settings(cmd, *is_down);
    ESP_ERROR_CHECK(i2c_master_write_byte(cmd, 0xAF, true));
    ESP_ERROR_CHECK(i2c_master_stop(cmd));
    print_error(i2c_master_cmd_begin(
                oled->port, cmd, 10 / portTICK_PERIOD_MS),
                __func__, __LINE__, "master_cmd_begin failed");
    i2c_cmd_link_delete(cmd);
    display_pixels(oled);
}

void check_buttons(void *db)
{
    t_data *d = (t_data*)db;
    while(true)
    {
        d->btn1.is_press = !gpio_get_level(BUTTON_1);
        d->btn2.is_press = !gpio_get_level(BUTTON_2);
        toggle_button(&d->btn1);
        toggle_button(&d->btn2);
        vTaskDelay(20 / portTICK_PERIOD_MS);
    }
}

void check_accelerometer(void *db)
{
    t_data *d = (t_data*)db;
    _Bool is_board_flipped = false;
    double y = 0.0;
    int16_t xyz[3];
    read_acceleration(d->spi, xyz);
    y = 0.94 * y + 0.06 * ((double)xyz[1] / 256.0);
    d->is_down = (y > 0.00) ? true : false;
    while(true)
    {
        read_acceleration(d->spi, xyz);
        y = 0.94 * y + 0.06 * ((double)xyz[1] / 256.0);
        is_board_flipped = (y < 0.00) ? true : false;
        if((is_board_flipped && !d->is_down) || (!is_board_flipped && d->is_down))
            change_oled_settings(&d->oled, &d->is_down);
        vTaskDelay(20 / portTICK_PERIOD_MS);
    }
}

