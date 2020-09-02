#include "buttons.h"

void data_init(void)
{
    ESP_ERROR_CHECK(gpio_set_direction(LED_1, GPIO_MODE_OUTPUT));
    ESP_ERROR_CHECK(gpio_set_direction(LED_2, GPIO_MODE_OUTPUT));
    ESP_ERROR_CHECK(gpio_set_direction(BUTTON_1, GPIO_MODE_INPUT));
    ESP_ERROR_CHECK(gpio_set_direction(BUTTON_2, GPIO_MODE_INPUT));
}

void toggle_led(t_button *btn)
{
    if (btn->is_press == true && btn->flag == false)
        btn->flag = true;
    if (btn->is_press == false && btn->flag == true)
    {
        btn->flag = false;
        btn->is_on = !btn->is_on;
        if (btn->is_on == true)
            ESP_ERROR_CHECK(gpio_set_level(btn->led, 1));
        else
            ESP_ERROR_CHECK(gpio_set_level(btn->led, 0));
        vTaskDelay(50 / portTICK_PERIOD_MS);
    }
}

