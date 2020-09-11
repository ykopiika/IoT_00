#include "main.h"

void init_components(t_data *db)
{
    dht11_init();
    init_oled(&db->oled);
    accel_init(&db->spi);
    buttons_init();
    init_speaker();
}

void app_main (void)
{
    t_data data;
    bzero(&data, sizeof(data));
    init_components(&data);
    xTaskCreate(check_dht11, "check_dht11", 2048, &data, 10, NULL);
    xTaskCreate(check_accelerometer, "check_dht11", 2048, &data, 5, NULL);
    xTaskCreate(check_buttons, "check_buttons", 2048, &data, 5, NULL);
    xTaskCreate(check_print_oled, "check_print_oled", 2048, &data, 5, NULL);
}

