#include "dht_11.h"

static void print_error(char *str)
{
    if (str)
        printf("Error: %s\n", str);
    exit(1);
}

static int count_status(int time, _Bool status)
{
    int count = 0;
    while (gpio_get_level(DH11_DATA_PIN) == status){
        if (count > time)
            return -1;
        ets_delay_us(1);
        count++;
    }
    if (count == 0)
        return -1;
    return count;
}

static void call_to_dht11()
{
    ESP_ERROR_CHECK(gpio_set_direction(DH11_POWER_PIN, GPIO_MODE_OUTPUT));
    ESP_ERROR_CHECK(gpio_set_direction(DH11_DATA_PIN, GPIO_MODE_OUTPUT));
    ESP_ERROR_CHECK(gpio_set_level(DH11_POWER_PIN, 1));
    ESP_ERROR_CHECK(gpio_set_level(DH11_DATA_PIN, 1));
    ets_delay_us(2000100);
    ESP_ERROR_CHECK(gpio_set_level(DH11_DATA_PIN, 0));
    ets_delay_us(18000);
    ESP_ERROR_CHECK(gpio_set_level(DH11_DATA_PIN, 1));
    ets_delay_us(30);
    ESP_ERROR_CHECK(gpio_set_direction(DH11_DATA_PIN, GPIO_MODE_INPUT));
    if (count_status(80, 0) == -1)
        print_error("STAGE_1 - Sensor response failed");
    if (count_status(80, 1) == -1)
        print_error("STAGE_2 - Sensor response failed");
}

static void set_bit(int result, uint8_t *arr, int *j, int i)
{
    if (result > 28)
    {
        arr[*j] <<= 1U;
        arr[*j] += 1U;
    }
    else
        arr[*j] <<= 1U;
    arr[*j]+=0;
    if (i % 8 == 0)
        (*j)++;
}

void get_value_dht11(uint8_t *temperature, uint8_t *humidity)
{
    int result = 0;
    uint8_t arr[5];
    if (!temperature || !humidity) {
        printf("Temperature and/or Humidity pointers is NULL!\n");
        return;
    }
    bzero(&arr, sizeof(arr));
    call_to_dht11();
    for (int i = 1, j = 0; i < 41; i++) {
        if (count_status(50, 0) == -1)
            print_error("STAGE_3 - Sensor response failed");
        if ((result = count_status(70, 1)) == -1)
            print_error("STAGE_4 - Sensor response failed");
        set_bit(result, arr, &j, i);
    }
    if (arr[0] + arr[1] + arr[2] + arr[3] != arr[4])
        print_error("Invalid checksum");
    *humidity = arr[0];
    *temperature = arr[2];
}
