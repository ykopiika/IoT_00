#include "buttons.h"

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
        vTaskDelay(50 / portTICK_PERIOD_MS);
    }
}
