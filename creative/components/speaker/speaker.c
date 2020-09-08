#include "speaker.h"

void make_bip(void)
{
    for (int i = 0; i < 250; ++i) {
        for (int i = 0; i < 255; i+=STEP)
        {
            ESP_ERROR_CHECK(dac_output_voltage(DAC_CHANNEL_1, i));
            ets_delay_us(DELAY);
        }
        ets_delay_us(DELAY);
        for (int i = 255; i > 0; i-=STEP)
        {
            ESP_ERROR_CHECK(dac_output_voltage(DAC_CHANNEL_1, i));
            ets_delay_us(DELAY);
        }
        ets_delay_us(10);
    }
}

void init_speaker(void)
{
    ESP_ERROR_CHECK(dac_output_enable(DAC_CHANNEL_1));
    ESP_ERROR_CHECK(gpio_set_direction(AMP_POWER_PIN, GPIO_MODE_OUTPUT));
    ESP_ERROR_CHECK(gpio_set_level(AMP_POWER_PIN, 1));
}