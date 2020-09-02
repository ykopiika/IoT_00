#include "dht_11.h"

void app_main()
{
    uint8_t tem = 0;
    uint8_t hum = 0;
    get_value_dht11(&tem,&hum);
    printf("Temperature: %d C\n", tem);
    printf("Humidity: %d %%\n", hum);
}


