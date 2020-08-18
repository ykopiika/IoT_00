#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/gpio.h"
#include "driver/dac.h"

#define AMP_POWER_PIN 5
#define DELAY 40
#define STEP 30

//static void print_error(char *str)
//{
//    printf("Error: %s\n", str);
//    exit(1);
//}

void app_main()
{
    dac_output_enable(DAC_CHANNEL_1);
    gpio_set_direction(AMP_POWER_PIN, GPIO_MODE_OUTPUT);
    gpio_set_level(AMP_POWER_PIN, 1);

    for (int i = 0; i < 250; ++i) {
        for (int i = 0; i < 255; i+=STEP)
        {
            dac_output_voltage(DAC_CHANNEL_1, i);
            ets_delay_us(DELAY);
        }
        ets_delay_us(DELAY);
        for (int i = 255; i > 0; i-=STEP)
        {
            dac_output_voltage(DAC_CHANNEL_1, i);
            ets_delay_us(DELAY);
        }
        ets_delay_us(10);
    }
}
