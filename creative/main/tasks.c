#include "main.h"

static void print_page(t_data *db, _Bool is_page_one)
{
    char str[17];
    db->is_page_two = !is_page_one;
    clear_oled(&db->oled);
    if (is_page_one)
        sprintf(str, "Temperature %dC", db->tem);
    else
        sprintf(str, "Humidity %d%%", db->hum);
    str_to_oled(&db->oled, str);
}

void check_buttons(void *db)
{
    _Bool on = true;
    t_data *d = (t_data*)db;
    while(on)
    {
        d->btn1.is_press = !gpio_get_level(BUTTON_1);
        d->btn2.is_press = !gpio_get_level(BUTTON_2);
        toggle_button(&d->btn1);
        toggle_button(&d->btn2);
        vTaskDelay(20 / portTICK_PERIOD_MS);
    }
}

static void write_value(t_data* d, uint8_t tem, uint8_t hum)
{
    if ((tem != d->tem) && (tem != 0))
    {
        d->is_updt_tem = true;
        d->tem = tem;
        printf(T_SLV"\t\t\t Temperature changed\n"R);
    }
    else
        d->is_updt_tem = false;

    if ((hum != d->hum) && (hum != 0))
    {
        d->is_updt_hum = true;
        d->hum = hum;
        printf(T_SLV"\t\t\t Humidity changed\n"R);
    }
    else
        d->is_updt_hum = false;
}

void check_dht11(void *db)
{
    uint8_t tem = 0;
    uint8_t hum = 0;
    _Bool on = true;
    t_data *d = (t_data*)db;
    while(on)
    {
        get_value_dht11(&tem, &hum);
        if ((tem != d->tem) || (hum != d->hum))
            write_value(d, tem, hum);
        printf("tem: %dC, hum: %d%%\n", d->tem, d->hum);
        vTaskDelay(2000 / portTICK_PERIOD_MS);
    }
}

void check_print_oled(void *db)
{
    t_data *d = (t_data*)db;
    _Bool on = true;
    while(on)
    {
        if (d->btn1.is_on)
        {
            if (d->is_page_two)
                print_page(db,1);
            make_bip();
            d->btn1.is_on = false;
        }
        if (d->btn2.is_on)
        {
            if (!d->is_page_two)
                print_page(db,0);
            make_bip();
            d->btn2.is_on = false;
        }
        if (d->is_updt_tem || d->is_updt_hum)
        {
            if (d->is_updt_tem && !d->is_page_two)
                print_page(db,1);
            else if (d->is_updt_hum && d->is_page_two)
                print_page(db,0);
            d->is_updt_tem = false;
            d->is_updt_hum = false;
        }
        vTaskDelay(20 / portTICK_PERIOD_MS);
    }
}

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

void check_accelerometer(void *db)
{
    t_data *d = (t_data*)db;
    _Bool on = true;
    _Bool is_board_flipped = false;
    int16_t xyz[3];
    t_dbl_xyz filtered = {0.0, 0.0, 0.0};       //TODO: change to one y double
    read_acceleration(d->spi, xyz);
    filtered.y = 0.94 * filtered.y + 0.06 * ((double)xyz[1] / 256.0);
    d->is_down = (filtered.y > 0.00) ? true : false;
    while(on)
    {
        read_acceleration(d->spi, xyz);
        filtered.y = 0.94 * filtered.y + 0.06 * ((double)xyz[1] / 256.0);
        is_board_flipped = (filtered.y < 0.00) ? true : false;
        if((is_board_flipped && !d->is_down) || (!is_board_flipped && d->is_down))
            change_oled_settings(&d->oled, &d->is_down);
        vTaskDelay(20 / portTICK_PERIOD_MS);
    }
}

//TODO: change oled settings

