#include "accelerometer.h"

static _Bool check_flip(t_dbl_xyz filtered)
{
    _Bool is_z_low = (fabs(filtered.z) < MAGIC_CONST);
    if (is_z_low)
        return true;
    return false;
}

static void make_bip(_Bool status)
{
    if (status == true)
    {
        for (int i = 0; i < 250; ++i) {
            for (int i = 0; i < 255; i += STEP) {
                ESP_ERROR_CHECK(dac_output_voltage(DAC_CHANNEL_1, i));
                ets_delay_us(DELAY);
            }
            ets_delay_us(DELAY);
            for (int i = 255; i > 0; i -= STEP) {
                ESP_ERROR_CHECK(dac_output_voltage(DAC_CHANNEL_1, i));
                ets_delay_us(DELAY);
            }
            ets_delay_us(10);
        }
        ets_delay_us(1000);
    }
}

static void leds_set_level(_Bool level)
{
    ESP_ERROR_CHECK(gpio_set_level(GPIO_NUM_26, level));
    ESP_ERROR_CHECK(gpio_set_level(GPIO_NUM_27, level));
    ESP_ERROR_CHECK(gpio_set_level(GPIO_NUM_33, level));
}

static void leds_amplif_init(void)
{
    ESP_ERROR_CHECK(dac_output_enable(DAC_CHANNEL_1));
    ESP_ERROR_CHECK(gpio_set_direction(AMP_POWER_PIN, GPIO_MODE_OUTPUT));
    ESP_ERROR_CHECK(gpio_set_level(AMP_POWER_PIN, 1));
    ESP_ERROR_CHECK(gpio_set_direction(GPIO_NUM_23, GPIO_MODE_OUTPUT));
    ESP_ERROR_CHECK(gpio_set_level(GPIO_NUM_23, 1));
    ESP_ERROR_CHECK(gpio_set_direction(GPIO_NUM_26, GPIO_MODE_OUTPUT));
    ESP_ERROR_CHECK(gpio_set_direction(GPIO_NUM_27, GPIO_MODE_OUTPUT));
    ESP_ERROR_CHECK(gpio_set_direction(GPIO_NUM_33, GPIO_MODE_OUTPUT));
}

void app_main (void)
{
    spi_device_handle_t spi;
    _Bool is_board_flipped = false;
    int16_t xyz[3];
    t_dbl_xyz filtered = {0.0, 0.0, 0.0};
    leds_amplif_init();
    accel_init(&spi);
    while (true) {
        read_acceleration(spi, xyz);
        filtered.x = 0.94 * filtered.x + 0.06 * ((double)xyz[0] / 256.0);
        filtered.y = 0.94 * filtered.y + 0.06 * ((double)xyz[1] / 256.0);
        filtered.z = 0.94 * filtered.z + 0.06 * ((double)xyz[2] / 256.0);
        is_board_flipped = check_flip(filtered);
        leds_set_level(is_board_flipped);
        make_bip(is_board_flipped);
        printf("x,y,z: %2.2f, %2.2f, %2.2f\n", filtered.x, filtered.y, filtered.z);
        vTaskDelay(10 / portTICK_PERIOD_MS);
    }
}
