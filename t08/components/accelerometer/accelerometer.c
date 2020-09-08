#include "accelerometer.h"

static void value_to_register (spi_device_handle_t spi,
                               uint8_t regstr, uint8_t value)
{
    spi_transaction_t trans = {
            .cmd = regstr,
            .tx_buffer = &value,
            .length = 8
    };
    ESP_ERROR_CHECK(spi_device_polling_transmit(spi, &trans));
}

void read_acceleration (spi_device_handle_t spi, int16_t *xyz)
{
    uint8_t tx_buffer[3U * sizeof(uint16_t)];
    spi_transaction_t trans = {
            .cmd = 0x80U | 0x40U | 0x32U,
            .length = sizeof(tx_buffer) * 8,
            .tx_buffer = tx_buffer,
            .rx_buffer = xyz
    };
    ESP_ERROR_CHECK(spi_device_polling_transmit(spi, &trans));
}

void accel_init (spi_device_handle_t *spi)
{
    spi_bus_config_t bus_config = {
            .miso_io_num = PIN_NUM_MISO,
            .mosi_io_num = PIN_NUM_MOSI,
            .sclk_io_num = PIN_NUM_CLK,
            .quadwp_io_num = -1,
            .quadhd_io_num = -1
    };
    spi_device_interface_config_t dev_config = {
            .clock_speed_hz = 1000000,
            .mode = 3,
            .spics_io_num = PIN_NUM_CS,
            .command_bits = 8,
            .queue_size = 1
    };
    ESP_ERROR_CHECK(spi_bus_initialize(ADXL_HOST, &bus_config, 0));
    ESP_ERROR_CHECK(spi_bus_add_device(ADXL_HOST, &dev_config, spi));
    value_to_register(*spi, 0x2Du, 0x08u);
    vTaskDelay(200 / portTICK_PERIOD_MS);
}
