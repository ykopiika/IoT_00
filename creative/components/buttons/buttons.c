#include "buttons.h"

void buttons_init(void)
{
    ESP_ERROR_CHECK(gpio_set_direction(BUTTON_1, GPIO_MODE_INPUT));
    ESP_ERROR_CHECK(gpio_set_direction(BUTTON_2, GPIO_MODE_INPUT));
}

void toggle_button(t_button *btn)
{
    if (btn->is_press == true && btn->flag == false)
        btn->flag = true;
    if (btn->is_press == false && btn->flag == true)
    {
        btn->flag = false;
        btn->is_on = true;
        vTaskDelay(50 / portTICK_PERIOD_MS);
    }
}

