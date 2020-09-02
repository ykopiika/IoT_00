#include "flashing_leds.h"

void app_main(void)
{
    ledc_timer_config_t ledc_timer;
    ledc_channel_config_t ledc_channel;
    data_init(&ledc_timer, &ledc_channel);
    while (true)
    {
        dac_flashing();
        pwm_flashing(&ledc_channel);
    }
}
