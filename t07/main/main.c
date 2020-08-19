//#include "freertos/FreeRTOS.h"
//#include "freertos/task.h"
#include "driver/gpio.h"
#include <stdio.h>
#include "esp_log.h"
#include "driver/i2c.h"
#include <string.h>

//
//
#define EN_OLED GPIO_NUM_32
//#define ESP_SLAVE_ADDR 0x3c
//#define ACK_CHECK_EN 0x1
//
//
////static void print_error(char *str)
////{
////    printf("Error: %s\n", str);
////    exit(1);
////}
//
//static esp_err_t i2c_master_write_slave(i2c_port_t i2c_num, uint8_t *data_wr, size_t size)
//{
//    i2c_cmd_handle_t cmd = i2c_cmd_link_create();
//    i2c_master_start(cmd);
//    i2c_master_write_byte(cmd, (0b011110 << 1) | I2C_MASTER_WRITE, ACK_CHECK_EN);    /*! ESP_SLAVE_ADDR ESP32 slave address, you can set any 7bit value */
//    i2c_master_write(cmd, data_wr, size, ACK_CHECK_EN);
//    i2c_master_stop(cmd);
//    esp_err_t ret = i2c_master_cmd_begin(i2c_num, cmd, 1000 / portTICK_RATE_MS);
//    i2c_cmd_link_delete(cmd);
//    return ret;
//}
//
//static esp_err_t i2c_master_init(void)
//{
//    int i2c_master_port = 1;
//    i2c_config_t conf;
//    conf.mode = I2C_MODE_MASTER;
//    conf.sda_io_num = 21;
//    conf.sda_pullup_en = GPIO_PULLUP_ENABLE;
//    conf.scl_io_num = 22;
//    conf.scl_pullup_en = GPIO_PULLUP_ENABLE;
//    conf.master.clk_speed = 1000;
//    i2c_param_config(i2c_master_port, &conf);
//    return i2c_driver_install(i2c_master_port, conf.mode, 0, 0, 0);
//}
//
//static esp_err_t i2c_slave_init(void)
//{
//    int i2c_slave_port = 2;
//    i2c_config_t conf_slave;
//    conf_slave.mode = I2C_MODE_SLAVE;
//    conf_slave.sda_io_num = 21;
//    conf_slave.sda_pullup_en = GPIO_PULLUP_ENABLE;
//    conf_slave.scl_io_num = 22;
//    conf_slave.scl_pullup_en = GPIO_PULLUP_ENABLE;
//    conf_slave.slave.addr_10bit_en = 0;
//    conf_slave.slave.slave_addr = ESP_SLAVE_ADDR;
//    i2c_param_config(i2c_slave_port, &conf_slave);
//    return i2c_driver_install(i2c_slave_port, conf_slave.mode, 1024, 1024, 0);
//}
//
//void app_main()
//{
//    uint8_t *data = (uint8_t *)malloc(512);//128
//    memset(data, 255, sizeof(*data));
//
//    gpio_set_direction(EN_OLED, GPIO_MODE_OUTPUT);
//    gpio_set_level(EN_OLED, 1);
//
//    ESP_ERROR_CHECK(i2c_master_init());
//    ESP_ERROR_CHECK(i2c_slave_init());
//    ESP_ERROR_CHECK( i2c_master_write_slave(1, data, 128));
//
////    for (int i = 0; i < 250; ++i) {
////        ets_delay_us(10);
////    }
//}



//!---------------------------------------------------------------------------------------------
//!---------------------------------------------------------------------------------------------


////#include "stm32f10x.h"
//#define SH1106_I2C_ADDRESS          0x78
//#define SH1106_I2C_SPEED            100000 //300000
//// GPIO OLED
//#define SH1106_I2C                  I2C1
//#define SH1106_I2C_CLK              RCC_APB1Periph_I2C1
//#define SH1106_I2C_SCL_PIN          GPIO_Pin_22
//#define SH1106_I2C_SCL_GPIO_PORT    GPIOB
//#define SH1106_I2C_SCL_GPIO_CLK     RCC_APB2Periph_GPIOB
//#define SH1106_I2C_SDA_PIN          GPIO_Pin_21
//#define SH1106_I2C_SDA_GPIO_PORT    GPIOB
//#define SH1106_I2C_SDA_GPIO_CLK     RCC_APB2Periph_GPIOB
//#define SH1106_I2C_DR               ((uint32_t)0x40005410)
//void OLED_SH1106_I2C_init(void);
//void UG2864ASGGG14_Init(void);
//void OLED_SH1106_Clear(uint8_t data);
////#include "stm32f10x.h"
////#include "sh1106.h"
////=============== Настройка интерфейса I2C ===============
//void OLED_SH1106_I2C_init(void) {
//    I2C_InitTypeDef I2C_InitStructure;
//    GPIO_InitTypeDef GPIO_InitStructure;
//    RCC_APB1PeriphClockCmd(SH1106_I2C_CLK, ENABLE);
//    RCC_APB2PeriphClockCmd(SH1106_I2C_SCL_GPIO_CLK | SH1106_I2C_SDA_GPIO_CLK,
//                           ENABLE);
//
//    GPIO_InitStructure.GPIO_Pin = SH1106_I2C_SCL_PIN;
//    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_10MHz;
//    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_OD;
//    GPIO_Init(SH1106_I2C_SCL_GPIO_PORT, &GPIO_InitStructure);
//    GPIO_InitStructure.GPIO_Pin = SH1106_I2C_SDA_PIN;
//    GPIO_Init(SH1106_I2C_SDA_GPIO_PORT, &GPIO_InitStructure);
//    I2C_InitStructure.I2C_Mode = I2C_Mode_I2C;
//    I2C_InitStructure.I2C_DutyCycle = I2C_DutyCycle_2;
//    I2C_InitStructure.I2C_OwnAddress1 = SH1106_I2C_ADDRESS;
//    I2C_InitStructure.I2C_Ack = I2C_Ack_Enable;
//    I2C_InitStructure.I2C_AcknowledgedAddress= I2C_AcknowledgedAddress_7bit;
//    I2C_InitStructure.I2C_ClockSpeed = SH1106_I2C_SPEED;
//    I2C_Cmd(SH1106_I2C, ENABLE);
//    I2C_Init(SH1106_I2C, &I2C_InitStructure);
//}
////=============== Передача команды по I2C ===============
//void WriteC(uint8_t COM) {
//    while (I2C_GetFlagStatus(I2C1, I2C_FLAG_BUSY));
//    I2C_GenerateSTART(I2C1, ENABLE);
//    while (!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_MODE_SELECT));
//// адрес устройства
//    I2C_Send7bitAddress(I2C1, 0x78, I2C_Direction_Transmitter);
//    while (!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED));
//    I2C_SendData(I2C1, 0x00); // контрольный байт
//    while (!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_BYTE_TRANSMITTED));
//    I2C_SendData(I2C1, COM); // команда
//    while (!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_BYTE_TRANSMITTED));
//    I2C_GenerateSTOP(I2C1, ENABLE);
//}
////=============== Передача данных по I2C ===============
//void WriteD(uint8_t DAT) {
//    while (I2C_GetFlagStatus(I2C1, I2C_FLAG_BUSY));
//    I2C_GenerateSTART(I2C1, ENABLE);
//    while (!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_MODE_SELECT));
//// адрес устройства
//    I2C_Send7bitAddress(I2C1, 0x78, I2C_Direction_Transmitter);
//    while (!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED));
//    I2C_SendData(I2C1, 0x40); // контрольный байт
//    while (!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_BYTE_TRANSMITTED));
//    I2C_SendData(I2C1, DAT); // данные
//    while (!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_BYTE_TRANSMITTED));
//    I2C_GenerateSTOP(I2C1, ENABLE);
//}
//
////=============== Минимальная настройка дисплея UG-2864ASGGG14 ======
//void UG2864ASGGG14_Init(void) {
//    WriteC(0xAE); /*выключить дисплей*/
//    WriteC(0x00); /*младшие биты адреса колонки*/
//    WriteC(0x10); /*старшие биты адреса колонки*/
//    WriteC(0x40); /*стартовая строка*/
//    WriteC(0xB0); /*стартовая страница*/
//    WriteC(0x81); /*установка контраста*/
//    WriteC(0x80); /*128*/
//    WriteC(0xA1); /*отображение слева направо*/
//    WriteC(0xA6); /*нормальный режим*/
//    WriteC(0xad); /*включить генератор подкачки 7.4В*/
//    WriteC(0x8b);
//    WriteC(0x30);
//    WriteC(0xAF); /*включить дисплей*/
//}
////=============== Заполнение всего дисплея константой ===============
//void OLED_SH1106_Clear() {
//    static uint8_t page = 0xB0;
//    uint8_t x, y;
//    for(y = 0; y < 8; y++) {
//        WriteC(page);
//        page++;
//        for(x = 0; x < 132; x++){
//            WriteD(0x02); }
//    }
////-------------------------
//    page = 0xB0;
//}
//
//void app_main()
//{
//    uint32_t i32;
//    for (i32=0; i32<100000; i32++) ; // Программная задержка
//        OLED_SH1106_I2C_init(); // Настройка I2C
//    for (i32=0; i32<100000; i32++) ;
//        UG2864ASGGG14_Init(); // Настройка дисплея
//    for (i32=0; i32<100000; i32++) ;
//    while (1) {
//        for (s=0; s<256; s++) OLED_SH1106_Clear();
//    }
//}




//!---------------------------------------------------------------------------------------------
//!---------------------------------------------------------------------------------------------

//#include <stdio.h>
//#include "driver/i2c.h"
//
//#define I2C_MASTER_NUM    I2C_NUM_0
//#define I2C_MASTER_SCL_IO    22    /*!< gpio number for I2C master clock */
//#define I2C_MASTER_SDA_IO    21    /*!< gpio number for I2C master data  */
//#define I2C_MASTER_TX_BUF_DISABLE   0   /*!< I2C master do not need buffer */
//#define I2C_MASTER_RX_BUF_DISABLE   0   /*!< I2C master do not need buffer */
//#define I2C_MASTER_FREQ_HZ    100000     /*!< I2C master clock frequency,<400 khz*/
//#define ACK_CHECK_EN   0x1     /*!< I2C master will check ack from slave*/
//#define SH1106_I2C_ADDRESS   0x78
//
//esp_err_t i2c_cmd(i2c_port_t i2c_num,uint8_t cmdx){
//    i2c_cmd_handle_t cmd = i2c_cmd_link_create();
//    i2c_master_start(cmd);
//    i2c_master_write_byte(cmd, SH1106_I2C_ADDRESS, ACK_CHECK_EN);
//    i2c_master_write_byte(cmd, 0x00, ACK_CHECK_EN);//CMD START...
//    i2c_master_write_byte(cmd, cmdx, ACK_CHECK_EN);//CMD
//    i2c_master_stop(cmd);
//    esp_err_t ret = i2c_master_cmd_begin(i2c_num, cmd, 200 / portTICK_RATE_MS);
//    i2c_cmd_link_delete(cmd);
//    if (ret == ESP_FAIL) {
//        printf("cmd err:%d\n",ret);
//        return ret;
//    }
//    return ESP_OK;
//}
//esp_err_t i2c_data(i2c_port_t i2c_num,uint8_t datas){
//    i2c_cmd_handle_t cmd = i2c_cmd_link_create();
//    i2c_master_start(cmd);
//    i2c_master_write_byte(cmd, SH1106_I2C_ADDRESS,ACK_CHECK_EN);
//    i2c_master_write_byte(cmd, 0x40, ACK_CHECK_EN);//data cmd contra data
//    i2c_master_write_byte(cmd, datas, ACK_CHECK_EN);
//    i2c_master_stop(cmd);
//    esp_err_t ret = i2c_master_cmd_begin(i2c_num, cmd, 200 / portTICK_RATE_MS);
//    i2c_cmd_link_delete(cmd);
//    if (ret == ESP_FAIL) {
//        printf("data err:%d\n",ret);
//        return ret;
//    }
//    return ESP_OK;
//}
//esp_err_t oled_setxy(i2c_port_t i2c_num,uint8_t x,uint8_t y)
//{
//    uint8_t h=(0xf0&x)>>4;
//    uint8_t l=(0x0f&x)>>4;
//    esp_err_t ret=i2c_cmd(i2c_num,0xb0|y);  //0154 3210
//    ret=i2c_cmd(i2c_num,l|0x01);            //0000 3210
//    ret=i2c_cmd(i2c_num,h|0x10);            //0001 7654
//    return ret;
//}
//esp_err_t oled_cls(i2c_port_t i2c_num)
//{
//    uint8_t x,y,ret;
//    for(y=0;y<64;y++)
//    {
//        if(oled_setxy(i2c_num,0,y))
//            return ESP_OK;
//        for(x=0;x<132;x++)
//        {
//            if(i2c_data(i2c_num,0x00))
//                return ESP_OK;
//        }
//    }
//    return ret;
//}
//
//void sh1106_init(i2c_port_t i2c_num){
//    i2c_cmd(i2c_num,0xAE);//--turn off oled panel
//    i2c_cmd(i2c_num,0x00);//---set low column address
//    i2c_cmd(i2c_num,0x10);//---set high column address0-131 X
//    i2c_cmd(i2c_num,0x40);//--set start line address 0-63  Y
//    i2c_cmd(i2c_num,0x81);//--set contrast control register
//    i2c_cmd(i2c_num,0xFF);
//    i2c_cmd(i2c_num,0xA1);//--Set SEG/Column Mapping     a1/a0
//    i2c_cmd(i2c_num,0xC8);//Set COM/Row Scan Direction   c8/c0
//    i2c_cmd(i2c_num,0xA6);//--set normal display
//    i2c_cmd(i2c_num,0xA8);//--set multiplex ratio(1 to 64)
//    i2c_cmd(i2c_num,0x3F);//--1/64 duty
//    i2c_cmd(i2c_num,0xD3);//-set display offset	Shift Mapping RAM Counter
//    i2c_cmd(i2c_num,0x00);//-not offset
//    i2c_cmd(i2c_num,0xD5);//--set display clock divide ratio/oscillator frequency
//    i2c_cmd(i2c_num,0x50);//--set divide ratio, Set Clock as 100 Frames/Sec
//    i2c_cmd(i2c_num,0xD9);//--set pre-charge period
//    i2c_cmd(i2c_num,0x22);//Set Pre-Charge as 15 Clocks & Discharge as 1 Clock
//    i2c_cmd(i2c_num,0xDA);//--set com pins hardware configuration
//    i2c_cmd(i2c_num,0x12);
//    i2c_cmd(i2c_num,0xDB);//--set vcomh
//    i2c_cmd(i2c_num,0x40);//Set VCOM Deselect Level
//    i2c_cmd(i2c_num,0xA4);// Disable Entire Display On (0xa4/0xa5)
//    i2c_cmd(i2c_num,0xA6);// Disable Inverse Display On (0xa6/a7)
//    i2c_cmd(i2c_num,0xAF);//--turn on oled panel
//}
//
//esp_err_t i2c_testx(i2c_port_t i2c_num){
//    sh1106_init(I2C_MASTER_NUM);
//    oled_cls(I2C_MASTER_NUM);
//    oled_setxy(I2C_MASTER_NUM,3,3);
//    i2c_data(I2C_MASTER_NUM,0x00);
//    i2c_data(I2C_MASTER_NUM,0x76);
//    i2c_data(I2C_MASTER_NUM,0x89);
//    i2c_data(I2C_MASTER_NUM,0x89);
//    i2c_data(I2C_MASTER_NUM,0x89);
//    i2c_data(I2C_MASTER_NUM,0x72);
//    vTaskDelay(50000 / portTICK_RATE_MS);
//    return ESP_OK;
//}
//
//void i2c_test_task()
//{
//    esp_err_t ret;
//    while (1) {
//        ret =i2c_testx(I2C_MASTER_NUM);
//        if (ret == ESP_OK) {
//            printf("espok\n");
//        } else {
//            printf("No ack, sensor not connected...skip...\n");
//        }
//
//    }
//}
//
//esp_err_t i2c_master_init()
//{
//    i2c_port_t i2c_master_port = I2C_MASTER_NUM;
//    i2c_config_t conf;
//    conf.mode = I2C_MODE_MASTER;
//    conf.sda_io_num = I2C_MASTER_SDA_IO;
//    conf.sda_pullup_en = GPIO_PULLUP_ENABLE;
//    conf.scl_io_num = I2C_MASTER_SCL_IO;
//    conf.scl_pullup_en = GPIO_PULLUP_ENABLE;
//    conf.master.clk_speed = I2C_MASTER_FREQ_HZ;
//    i2c_param_config(i2c_master_port, &conf);
//    esp_err_t ret=i2c_driver_install(i2c_master_port, conf.mode, I2C_MASTER_RX_BUF_DISABLE, I2C_MASTER_TX_BUF_DISABLE, 0);
//    return ret;
//}
//void app_main()
//{
//    gpio_set_direction(EN_OLED, GPIO_MODE_OUTPUT);
//    gpio_set_level(EN_OLED, 1);
//    esp_err_t ret=i2c_master_init();
//    if(ret!=ESP_OK){
//        printf("i2c_init err%d",ret);
//    }
//    xTaskCreate(i2c_test_task, "i2c_test_task_0", 1024 * 2,0, 10, NULL);
//}




//!---------------------------------------------------------------------------------------------
//!---------------------------------------------------------------------------------------------


#include "driver/gpio.h"
#include "driver/i2c.h"

#include "sh1106.h"

#define SDA_PIN  GPIO_NUM_21
#define SCL_PIN  GPIO_NUM_22
#define I2C_ADDR SH1106_DEFAULT_ADDR
#define I2C_PORT SH1106_DEFAULT_PORT

void write_page(uint8_t *data, uint8_t page) {
    i2c_cmd_handle_t cmd = i2c_cmd_link_create();
    i2c_master_start(cmd);
    i2c_master_write_byte(cmd, (SH1106_DEFAULT_ADDR << 1) | I2C_MASTER_WRITE, true);
    i2c_master_write_byte(cmd, 0b10000000, true); // single command
    i2c_master_write_byte(cmd, 0b10110000 + page, true); // page number
    i2c_master_write_byte(cmd, 0x40, true); // data stream
    i2c_master_write(cmd, data, 128, true);
    i2c_master_stop(cmd);
    i2c_master_cmd_begin(SH1106_DEFAULT_PORT, cmd, 10 / portTICK_PERIOD_MS);
    i2c_cmd_link_delete(cmd);
}

/*
    Initialize I2C driver.
*/
void init_i2c() {
    i2c_config_t i2c_config = {
            .mode = I2C_MODE_MASTER,
            .sda_io_num = SDA_PIN,
            .scl_io_num = SCL_PIN,
            .sda_pullup_en = GPIO_PULLUP_ENABLE,
            .scl_pullup_en = GPIO_PULLUP_ENABLE,
            .master.clk_speed = 1000000
    };
    i2c_param_config(I2C_PORT, &i2c_config);
    i2c_driver_install(I2C_PORT, I2C_MODE_MASTER, 0, 0, 0);
}

void app_main() {
    gpio_set_direction(GPIO_NUM_32, GPIO_MODE_OUTPUT);
    gpio_set_level(GPIO_NUM_32, 1);
    ets_delay_us(1000000);
    init_i2c();
    // setup sh1106 display
    sh1106_t display;
    display.addr = I2C_ADDR;
    display.port = I2C_PORT;
    sh1106_init(&display);
    sh1106_clear(&display);
    sh1106_update(&display);
    // draw triangle
//    for (uint8_t y = 0; y < 128; y++) {
//        for (uint8_t x = 0; x < 128; x++) {
//            sh1106_set(&display, x, y, 1);
//            sh1106_set(&display, x, y, 0);
//        }
//    }
    // update display
//    sh1106_update(&display);
    uint8_t arr[128];
    memset(&arr,255,sizeof(arr));
    write_page(arr,0);
//    for (int i = 0; i < 8; ++i) {
//        for (int j = 0; j < 128 ; ++j) {
//            write_page(0b11111111,i);
//            if (i != 0)
//                write_page(0b00000000,i - 1);
//            ets_delay_us(30000);
//        }
//    }




}
