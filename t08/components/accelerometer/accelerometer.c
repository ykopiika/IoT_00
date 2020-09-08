#include "accelerometer.h"

static void adxl345_write (
        spi_device_handle_t spi,
        uint8_t address,
        uint8_t value)
{
    esp_err_t ret;
    spi_transaction_t trans = {
            .flags = SPI_TRANS_USE_RXDATA,
            .cmd = address,
            .tx_buffer = &value,
            .length = 8
    };
    ret = spi_device_polling_transmit(spi, &trans);
    assert(ret == ESP_OK);
}

void adxl345_read_acceleration (spi_device_handle_t spi, int16_t *accs) {
    esp_err_t ret;
    uint8_t tx_buffer[3U * sizeof(uint16_t)];
    spi_transaction_t trans = {
            .cmd = 0x80U | 0x40U | 0x32U,
            .length = sizeof(tx_buffer) * 8,
            .tx_buffer = tx_buffer,
            .rx_buffer = accs
    };
    ret = spi_device_polling_transmit(spi, &trans);
    assert(ret == ESP_OK);
}

void accel_init (spi_device_handle_t *spi) {
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
    adxl345_write(*spi, 0x2Du, 0x08u);
    vTaskDelay(200 / portTICK_PERIOD_MS);
}
