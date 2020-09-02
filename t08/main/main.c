////#include "freertos/FreeRTOS.h"
////#include "freertos/task.h"
//#include "driver/gpio.h"
//#include <stdio.h>
//#include "esp_log.h"
//#include "driver/i2c.h"
//#include "img.h"
//#include <string.h>
//
//#define EN_OLED GPIO_NUM_32
//
//#include "sh1106.h"
//#include "font6x8.h"
//
//#define SDA_PIN  GPIO_NUM_21
//#define SCL_PIN  GPIO_NUM_22
//#define I2C_ADDR SH1106_DEFAULT_ADDR
//#define I2C_PORT SH1106_DEFAULT_PORT
//
//#define LCDWIDTH       128
//#define LCDHEIGHT       64
//uint8_t buffer[LCDWIDTH * LCDHEIGHT/8];
//
////#define ESP_SLAVE_ADDR 0x3c
////#define ACK_CHECK_EN 0x1
////
////
//////static void print_error(char *str)
//////{
//////    printf("Error: %s\n", str);
//////    exit(1);
//////}
//
//
//
//
//
//void write_page(uint8_t data, uint8_t page) {
//    i2c_cmd_handle_t cmd = i2c_cmd_link_create();
//    i2c_master_start(cmd);
//    i2c_master_write_byte(cmd, (SH1106_DEFAULT_ADDR << 1) | I2C_MASTER_WRITE, true);
//    i2c_master_write_byte(cmd, 0x80, true); // single command
//    i2c_master_write_byte(cmd, 0xB0 + page, true); // page number
//    i2c_master_write_byte(cmd, 0x40, true); // data stream
//    i2c_master_write(cmd, &data, 1, true);
//    i2c_master_stop(cmd);
//    i2c_master_cmd_begin(SH1106_DEFAULT_PORT, cmd, 10 / portTICK_PERIOD_MS);
//    i2c_cmd_link_delete(cmd);
//}
//void write_lcd(uint8_t *data) {
//    i2c_cmd_handle_t cmd = i2c_cmd_link_create();
//    i2c_master_start(cmd);
//    i2c_master_write_byte(cmd, (SH1106_DEFAULT_ADDR << 1) | I2C_MASTER_WRITE, true);
//    i2c_master_write_byte(cmd, 0x80, true); // single command
//    i2c_master_write_byte(cmd, 0x00, true); // page number
//    i2c_master_write_byte(cmd, 0x40, true); // data stream
//    i2c_master_write(cmd, data, (LCDWIDTH * LCDHEIGHT/8), true);
//    i2c_master_stop(cmd);
//    i2c_master_cmd_begin(SH1106_DEFAULT_PORT, cmd, 10 / portTICK_PERIOD_MS);
//    i2c_cmd_link_delete(cmd);
//}
//
//void draw_pixel(int16_t x, int16_t y, _Bool color)
//{
//        if (color == true)
//            buffer[x+ (y/8)*LCDWIDTH] |=  (1 << (y&7));
//        else
//            buffer[x+ (y/8)*LCDWIDTH] &= ~(1 << (y&7));
//}
//
///*
//    Initialize I2C driver.
//*/
//void init_i2c() {
//    i2c_config_t i2c_config = {
//            .mode = I2C_MODE_MASTER,
//            .sda_io_num = SDA_PIN,
//            .scl_io_num = SCL_PIN,
//            .sda_pullup_en = GPIO_PULLUP_ENABLE,
//            .scl_pullup_en = GPIO_PULLUP_ENABLE,
//            .master.clk_speed = 1000000
//    };
//    i2c_param_config(I2C_PORT, &i2c_config);
//    i2c_driver_install(I2C_PORT, I2C_MODE_MASTER, 0, 0, 0);
//}
//
//void str_to_buf(uint8_t *data, char *str)
//{
//    int len = strlen(str);
//    if (len <= 168)
//    {
//        int x = 0;
//        int y = 0;
//        for (int i = 0; i < len; ++i) {
//            int num_buf = ((y/8) * LCDWIDTH) + x;
//            int font_buf = (str[i] - 32) * 6;
//            data[num_buf++] = font6x8[font_buf++];
//            data[num_buf++] = font6x8[font_buf++];
//            data[num_buf++] = font6x8[font_buf++];
//            data[num_buf++] = font6x8[font_buf++];
//            data[num_buf++] = font6x8[font_buf++];
//            data[num_buf] = font6x8[font_buf];
//            x += 6;
//            if (x >= 126)
//            {
//                x = 0;
//                y += 8;
//            }
//            ets_delay_us(10000);
//        }
//    }
//    else {
//        str_to_buf(data, "NON");
//    }
//}
//
//void app_main() {
//    gpio_set_direction(GPIO_NUM_32, GPIO_MODE_OUTPUT);
//    gpio_set_level(GPIO_NUM_32, 1);
//    ets_delay_us(1000000);
//    init_i2c();
//    sh1106_t display;
//    display.addr = I2C_ADDR;
//    display.port = I2C_PORT;
//    sh1106_init(&display);
//    sh1106_clear(&display);
//    sh1106_update(&display);
//
//
////    memset(&buffer, 0, sizeof(buffer));
////    str_to_buf(buffer, "HELLO world! __ HELLO world! HELLO world! HELLO world! HELLO world!");
////    write_lcd(buffer);
//
//    gpio_set_direction(GPIO_NUM_23, GPIO_MODE_OUTPUT);
//    gpio_set_level(GPIO_NUM_23, 1);
//
//
//}

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_system.h"
#include "driver/spi_master.h"
#include "driver/gpio.h"
#include "driver/dac.h"
#include "math.h"

#define AMP_POWER_PIN 5
#define DELAY 25
#define STEP 30

#define ADXL_HOST  VSPI_HOST
#define DMA_CHAN  0
#define PIN_NUM_MISO  12
#define PIN_NUM_MOSI  13
#define PIN_NUM_CLK  14
#define PIN_NUM_CS  15

static uint8_t adxl345_read (spi_device_handle_t spi, uint8_t address) {
    esp_err_t ret;
    spi_transaction_t trans = {
            .flags = SPI_TRANS_USE_RXDATA | SPI_TRANS_USE_TXDATA,
            .cmd = address | 0x80u,
            .length = 8 // in bits
    };
    ret = spi_device_polling_transmit(spi, &trans);
    assert(ret == ESP_OK);
    return trans.rx_data[0];
}

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
            .length = 8 // in bits
    };
    ret = spi_device_polling_transmit(spi, &trans);
    assert(ret == ESP_OK);
}

static void adxl345_read_acceleration (spi_device_handle_t spi, int16_t* accs) {
    esp_err_t ret;
    uint8_t tx_buffer[3u * sizeof(uint16_t)]; // a dummy buffer
    spi_transaction_t trans = {
            .cmd = 0x80u | 0x40u | 0x32u,
            .length = sizeof(tx_buffer) * 8, // in bits
            .tx_buffer = tx_buffer,
            .rx_buffer = accs
    };
    ret = spi_device_polling_transmit(spi, &trans);
    assert(ret == ESP_OK);
}

static void adxl345_init (spi_device_handle_t spi) {
    uint8_t out;
    out = adxl345_read(spi, 0x00u);
    printf("DEVID: 0x%X\n", out);
    out = adxl345_read(spi, 0x2Cu);
    printf("BW_RATE: 0x%X\n", out);
}

static void adxl345_start (spi_device_handle_t spi) {
    adxl345_write(spi, 0x2Du, 0x08u);
    vTaskDelay(200 / portTICK_PERIOD_MS);
}


static void adxl345_read_acceleration_task (void* pvParameters) {
    int16_t accs[3];
    spi_device_handle_t spi = (spi_device_handle_t)pvParameters;
    while (true) {
        adxl345_read_acceleration(spi, accs);
        printf(
                "ax, ay, az: %d, %d, %d\n",
                (int)accs[0],
                (int)accs[1],
                (int)accs[2]);
        vTaskDelay(100 / portTICK_PERIOD_MS);
    }
}
int popKern(unsigned int x)
{
    int n = 0;
    for (; x; n++)
        x &= (x - 1); // убираем младший бит
    printf("%d\n", n);
    return n;
}
#define MAGIC_CONST 0.03
_Bool check_flip(double x, double y, double z)
{
//    _Bool is_x_low = (fabs(x) < MAGIC_CONST);
//    _Bool is_y_low = (fabs(y) < MAGIC_CONST);
    _Bool is_z_low = (fabs(z) < MAGIC_CONST);
    if (is_z_low)
        return true;
    return false;
}

void make_bip(_Bool status)
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
void print_error(void)
{
    printf("Error: GPIO_NUM is invalid\n");
    exit(1);
}

void leds_set_level(_Bool level)
{
    ESP_ERROR_CHECK(gpio_set_level(GPIO_NUM_26, level));
    ESP_ERROR_CHECK(gpio_set_level(GPIO_NUM_27, level));
    ESP_ERROR_CHECK(gpio_set_level(GPIO_NUM_33, level));
}

void leds_set_direction(void)
{
    ESP_ERROR_CHECK(gpio_set_direction(GPIO_NUM_26, GPIO_MODE_OUTPUT));
    ESP_ERROR_CHECK(gpio_set_direction(GPIO_NUM_27, GPIO_MODE_OUTPUT));
    ESP_ERROR_CHECK(gpio_set_direction(GPIO_NUM_33, GPIO_MODE_OUTPUT));
}

void app_main (void) {

    dac_output_enable(DAC_CHANNEL_1);
    gpio_set_direction(AMP_POWER_PIN, GPIO_MODE_OUTPUT);
    gpio_set_level(AMP_POWER_PIN, 1);

    gpio_set_direction(GPIO_NUM_23, GPIO_MODE_OUTPUT);
    gpio_set_level(GPIO_NUM_23, 1);
    esp_err_t ret;
    spi_device_handle_t spi;
    spi_bus_config_t buscfg = {
            .miso_io_num = PIN_NUM_MISO,
            .mosi_io_num = PIN_NUM_MOSI,
            .sclk_io_num = PIN_NUM_CLK,
            .quadwp_io_num = -1,
            .quadhd_io_num = -1
    };
    spi_device_interface_config_t devcfg = {
            .clock_speed_hz = 1000000, // 1Mbps. nearest clock will be chosen.
            .mode = 3, // CPOL=1, CPHA=1
            .spics_io_num = PIN_NUM_CS,
            .command_bits = 8, // ADXL345 always takes 1+7 bit command (address).
            .queue_size = 1 // I do not know an appropriate size.
    };
    // initializes the SPI bus
    ret = spi_bus_initialize(ADXL_HOST, &buscfg, DMA_CHAN);
    ESP_ERROR_CHECK(ret);
    // attaches the ADXL to the SPI bus
    ret = spi_bus_add_device(ADXL_HOST, &devcfg, &spi);
    ESP_ERROR_CHECK(ret);
    // initializes the ADXL
    adxl345_init(spi);
    // starts sampling
    adxl345_start(spi);
    // periodically reads acceleration

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