#include "main.h"

static void print_error(void)
{
    printf("Error: GPIO_NUM is invalid\n");
    exit(1);
}

static void turn_on_led(int led)
{
    if (gpio_set_level(led, 1) == ESP_ERR_INVALID_ARG)
        print_error();
}

static void turn_off_led(int led)
{
    if (gpio_set_level(led, 0) == ESP_ERR_INVALID_ARG)
        print_error();
}

static void data_init(void)
{
    if (gpio_set_direction(LED_1, GPIO_MODE_OUTPUT) == ESP_ERR_INVALID_ARG)
        print_error();
    if (gpio_set_direction(LED_2, GPIO_MODE_OUTPUT) == ESP_ERR_INVALID_ARG)
        print_error();
    if (gpio_set_direction(BUTTON_1, GPIO_MODE_INPUT) == ESP_ERR_INVALID_ARG)
        print_error();
    if (gpio_set_direction(BUTTON_2, GPIO_MODE_INPUT) == ESP_ERR_INVALID_ARG)
        print_error();
}

static void toggle_led(t_button *btn)
{
    if (btn->is_press == true && btn->flag == false)
        btn->flag = true;
    if (btn->is_press == false && btn->flag == true)
    {
        btn->flag = false;
        btn->is_on = !btn->is_on;
        if (btn->is_on == true)
            turn_on_led(btn->led);
        else
            turn_off_led(btn->led);
        vTaskDelay(50 / portTICK_PERIOD_MS);
    }
}

void app_main()
{
    t_button btn1 = (t_button){false, false, false, LED_1};
    t_button btn2 = (t_button){false, false, false, LED_2};
    data_init();
    while (true) {
        btn1.is_press = !gpio_get_level(BUTTON_1);
        btn2.is_press = !gpio_get_level(BUTTON_2);
        toggle_led(&btn1);
        toggle_led(&btn2);
    }
}
