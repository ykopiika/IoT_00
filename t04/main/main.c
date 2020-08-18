#include "main.h"


static void print_error(char *str)
{
    printf("Error: %s\n", str);
    exit(1);
}

int count_status(int time, _Bool status)
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

void print_bits(uint8_t n)
{
    if (n > 1)
        print_bits(n/2);
    char c = ((n % 2)+48);
    write(1, &c, 1);
}

void app_main()
{
    int result = 0;
    uint8_t arr[5];
    bzero(&arr, sizeof(arr));

    gpio_set_direction(DH11_POWER_PIN, GPIO_MODE_OUTPUT);
    gpio_set_direction(DH11_DATA_PIN, GPIO_MODE_OUTPUT);
    gpio_set_level(DH11_POWER_PIN, 1);
    gpio_set_level(DH11_DATA_PIN, 1);
    ets_delay_us(3000000);
    gpio_set_level(DH11_DATA_PIN, 0);
    ets_delay_us(18000);
    gpio_set_level(DH11_DATA_PIN, 1);
    ets_delay_us(30);
    gpio_set_direction(DH11_DATA_PIN, GPIO_MODE_INPUT);

    if ((result = count_status(80, 0)) == -1)
        print_error("STAGE_1 - Sensor response failed");
    if ((result = count_status(80, 1)) == -1)
        print_error("STAGE_2 - Sensor response failed");

    for (int i = 1, j = 0; i < 41; i++)
    {
        if ((result = count_status(50, 0)) == -1)
            print_error("STAGE_3 - Sensor response failed");
        if ((result = count_status(70, 1)) == -1)
            print_error("STAGE_4 - Sensor response failed");
        if (result > 28)
        {
            arr[j] <<= 1;
            arr[j] += 1;
        }
        else
            arr[j] <<= 1;
        arr[j]+=0;
        if (i % 8 == 0)
            j++;
    }
    for (int i = 0; i < 5; i++)
    {
        print_bits(arr[i]);
        write(1, "\n", 1);
    }
    if (arr[0] + arr[1] + arr[2] + arr[3] != arr[4])
        print_error("Invalid checksum");
    write(1, "\n", 1);
    printf("%d%% - humidity\n", arr[0]);
    printf("%dC - temperature\n", arr[2]);
}


