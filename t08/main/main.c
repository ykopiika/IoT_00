#include "accelerometer.h"

static _Bool check_flip(double x, double y, double z)
{
//    _Bool is_x_low = (fabs(x) < MAGIC_CONST);
//    _Bool is_y_low = (fabs(y) < MAGIC_CONST);
    _Bool is_z_low = (fabs(z) < MAGIC_CONST);
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
                dac_output_voltage(DAC_CHANNEL_1, i);
                ets_delay_us(DELAY);
            }
            ets_delay_us(DELAY);
            for (int i = 255; i > 0; i -= STEP) {
                dac_output_voltage(DAC_CHANNEL_1, i);
                ets_delay_us(DELAY);
            }
            ets_delay_us(10);
        }
        ets_delay_us(1000);
    }
}

//void print_error(void)
//{
//    printf("Error: GPIO_NUM is invalid\n");
//    exit(1);
//}

static void leds_set_level(_Bool level)
{
    ESP_ERROR_CHECK(gpio_set_level(GPIO_NUM_26, level));
    ESP_ERROR_CHECK(gpio_set_level(GPIO_NUM_27, level));
    ESP_ERROR_CHECK(gpio_set_level(GPIO_NUM_33, level));
}

static void leds_set_direction(void)
{
    ESP_ERROR_CHECK(gpio_set_direction(GPIO_NUM_26, GPIO_MODE_OUTPUT));
    ESP_ERROR_CHECK(gpio_set_direction(GPIO_NUM_27, GPIO_MODE_OUTPUT));
    ESP_ERROR_CHECK(gpio_set_direction(GPIO_NUM_33, GPIO_MODE_OUTPUT));
}

void app_main (void) {

    ESP_ERROR_CHECK(dac_output_enable(DAC_CHANNEL_1));
    ESP_ERROR_CHECK(gpio_set_direction(AMP_POWER_PIN, GPIO_MODE_OUTPUT));
    ESP_ERROR_CHECK(gpio_set_level(AMP_POWER_PIN, 1));
    ESP_ERROR_CHECK(gpio_set_direction(GPIO_NUM_23, GPIO_MODE_OUTPUT));
    ESP_ERROR_CHECK(gpio_set_level(GPIO_NUM_23, 1));

    spi_device_handle_t spi;
    accel_init(&spi);

    int16_t accs[3];
    double filtered_x = 0.0;
    double filtered_y = 0.0;
    double filtered_z = 0.0;
    leds_set_direction();
    _Bool on = true;
    while (on) {
        adxl345_read_acceleration(spi, accs);
        double x = (double)accs[0]/256.0;
        double y = (double)accs[1]/256.0;
        double z = (double)accs[2]/256.0;
        filtered_x = 0.94 * filtered_x + 0.06 * x;
        filtered_y = 0.94 * filtered_y + 0.06 * y;
        filtered_z = 0.94 * filtered_z + 0.06 * z;
        _Bool is_board_flipped = check_flip(filtered_x,filtered_y,filtered_z);
        leds_set_level(is_board_flipped);
        make_bip(is_board_flipped);

        printf(
                "x, y, z: %2.2f, %2.2f, %2.2f\n",
                x,
                y,
                z);
        vTaskDelay(10 / portTICK_PERIOD_MS);
    }
}
///0.93 - 1.05
//0xA