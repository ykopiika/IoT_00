#include <string.h>
#include "driver/uart.h"
#include "driver/gpio.h"

//static void print_error(char *str)
//{
//    printf("Error: %s\n", str);
//    exit(1);
//}

void app_main()
{
    char* test_str = "\e[0;41mRED\e[0m \e[0;42mGREEN\e[0m \e[0;44mBLUE\e[0m \e[0;49mDEFAULT\e[0m\n";
    uart_config_t uart_config = {
            .baud_rate = 9600,
            .data_bits = UART_DATA_8_BITS,
            .parity = UART_PARITY_DISABLE,
            .stop_bits = UART_STOP_BITS_1,
            .flow_ctrl = UART_HW_FLOWCTRL_DISABLE,
    };
    uart_param_config(UART_NUM_1, &uart_config);
    uart_set_pin(UART_NUM_1, 17, 16, UART_PIN_NO_CHANGE, UART_PIN_NO_CHANGE);
    uart_driver_install(UART_NUM_1, 1024, 0, 0, NULL, 0);
    uart_write_bytes(UART_NUM_1, (const char*)test_str, strlen(test_str));
}
