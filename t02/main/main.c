#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <driver/gpio.h>
#include <driver/dac.h>
#include "driver/ledc.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#define DELAY                   15
#define LEVEL                   120
#define LEDC_TEST_DUTY          255
#define LEDC_TEST_FADE_TIME     1200

void data_init(ledc_timer_config_t *ledc_timer,
               ledc_channel_config_t *ledc_channel)
{
    dac_output_enable(DAC_CHANNEL_2);
    *ledc_timer = (ledc_timer_config_t){
            .duty_resolution = LEDC_TIMER_8_BIT,
            .freq_hz = 1000,
            .speed_mode = LEDC_HIGH_SPEED_MODE,
            .timer_num = LEDC_TIMER_0,
            .clk_cfg = LEDC_AUTO_CLK,
    };
    ledc_timer_config(ledc_timer);
    *ledc_channel = (ledc_channel_config_t){
            .channel    = LEDC_CHANNEL_0,
            .duty       = 0,
            .gpio_num   = GPIO_NUM_27,
            .speed_mode = LEDC_HIGH_SPEED_MODE,
            .hpoint     = 0,
            .timer_sel  = LEDC_TIMER_0
    };
    ledc_channel_config(ledc_channel);
    ledc_fade_func_install(0);
}

void app_main(void)
{
    ledc_timer_config_t ledc_timer;
    ledc_channel_config_t ledc_channel;
    data_init(&ledc_timer, &ledc_channel);
    while (true)
    {
        for (uint8_t i = LEVEL; i < 255; i++)
        {
            dac_output_voltage(DAC_CHANNEL_2, i);
            vTaskDelay(DELAY / portTICK_PERIOD_MS);
        }
        vTaskDelay(30 / portTICK_PERIOD_MS);
        for (uint8_t i = 255; i > LEVEL; i--)
        {
            dac_output_voltage(DAC_CHANNEL_2, i);
            vTaskDelay(DELAY / portTICK_PERIOD_MS);
        }
        ledc_set_fade_with_time(ledc_channel.speed_mode, ledc_channel.channel,
                                LEDC_TEST_DUTY, LEDC_TEST_FADE_TIME);
        ledc_fade_start(ledc_channel.speed_mode,
                        ledc_channel.channel, LEDC_FADE_WAIT_DONE);
        vTaskDelay(30 / portTICK_PERIOD_MS);
        ledc_set_fade_with_time(ledc_channel.speed_mode,
                                ledc_channel.channel, 0, LEDC_TEST_FADE_TIME);
        ledc_fade_start(ledc_channel.speed_mode,
                        ledc_channel.channel, LEDC_FADE_WAIT_DONE);
    }
}
