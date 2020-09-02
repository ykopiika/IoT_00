#include "driver/gpio.h"

void app_main()
{
    ESP_ERROR_CHECK(gpio_set_direction(GPIO_NUM_26, GPIO_MODE_OUTPUT));
    ESP_ERROR_CHECK(gpio_set_direction(GPIO_NUM_27, GPIO_MODE_OUTPUT));
    ESP_ERROR_CHECK(gpio_set_direction(GPIO_NUM_33, GPIO_MODE_OUTPUT));
    ESP_ERROR_CHECK(gpio_set_level(GPIO_NUM_26, 1));
    ESP_ERROR_CHECK(gpio_set_level(GPIO_NUM_27, 1));
    ESP_ERROR_CHECK(gpio_set_level(GPIO_NUM_33, 1));
}
