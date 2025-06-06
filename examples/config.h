/*
 * Platform specific configuration defaults.
 */


#ifdef TARGET_AVR
/* i2c */
#define I2C_BITBANG_SCL     28
#define I2C_BITBANG_SDA     27
#define CFG_I2C_CONTEXT     NULL
#define CFG_I2C_FREQUENCY   100000
#define CFG_I2C_SCL         GPIOD6
#define CFG_I2C_SDA         GPIOD5
/* spi */
#define CFG_SPI_CONTEXT     NULL
#define CFG_SPI_FREQUENCY   0
#define CFG_SPI_MISO        0
#define CFG_SPI_MOSI        0
#define CFG_SPI_SCLK        0
/* nrf24l01+ */
#define CFG_NRF_SS          GPIOB0
#define CFG_NRF_CE          GPIOB1
/* hd44780 */
#define CFG_HD44780_D4      GPIOB0
#define CFG_HD44780_D5      GPIOB1
#define CFG_HD44780_D6      GPIOB2
#define CFG_HD44780_D7      GPIOB3
#define CFG_HD44780_EN      GPIOB6
#define CFG_HD44780_RW      GPIOB5
#define CFG_HD44780_RS      GPIOB4
#endif


#ifdef TARGET_PIC8
/* i2c */
#define CFG_I2C_CONTEXT     NULL
#define CFG_I2C_FREQUENCY   100000
#define CFG_I2C_SCL         GPIOD6
#define CFG_I2C_SDA         GPIOD5
/* spi */
#define CFG_SPI_CONTEXT     NULL
#define CFG_SPI_FREQUENCY   0
#define CFG_SPI_MISO        255
#define CFG_SPI_MOSI        255
#define CFG_SPI_SCLK        255
/* nrf24l01+ */
#define CFG_NRF_SS          255
#define CFG_NRF_CE          255
#endif

#ifdef TARGET_PIC32
/* spi */
#define CFG_SPI_CONTEXT     NULL
#define CFG_SPI_FREQUENCY   0
#define CFG_SPI_MISO        0
#define CFG_SPI_MOSI        0
#define CFG_SPI_SCLK        0
/* nrf24l01+ */
#define CFG_NRF_SS          0
#define CFG_NRF_CE          1
#endif


#ifdef TARGET_X86
/* ftdi */
#define CFG_FTDI_VID        0 /* example 0x0403 */
#define CFG_FTDI_PID        0 /* example 0x6011 which is FT4232H */
#define CFG_FTDI_DESC       NULL
#define CFG_FTDI_SERIAL     NULL
/* i2c */
#define I2C_BITBANG_SCL     0
#define I2C_BITBANG_SDA     1
#define CFG_I2C_CONTEXT     NULL
#define CFG_I2C_FREQUENCY   100000
#define CFG_I2C_SCL         0
#define CFG_I2C_SDA         1
/* spi */
#define CFG_SPI_CONTEXT     0
#define CFG_SPI_FREQUENCY   0
#define CFG_SPI_MISO        2
#define CFG_SPI_MOSI        1
#define CFG_SPI_SCLK        0
/* pcd8544 */
#define CFG_PCD8544_CE     3
#define CFG_PCD8544_DC     4
#define CFG_PCD8544_RESET  5
/* nrf24l01+ */
#define CFG_NRF_SS          3
#define CFG_NRF_CE          4
/* hd44780 */
#define CFG_HD44780_D4      16
#define CFG_HD44780_D5      17
#define CFG_HD44780_D6      18
#define CFG_HD44780_D7      19
#define CFG_HD44780_EN      20
#define CFG_HD44780_RW      21
#define CFG_HD44780_RS      22
#endif


#ifdef TARGET_MSP430
/* spi */
#define CFG_SPI_CONTEXT     NULL
#define CFG_SPI_FREQUENCY   0
#define CFG_SPI_MISO        0
#define CFG_SPI_MOSI        0
#define CFG_SPI_SCLK        0
/* nrf24l01+ */
#define CFG_NRF_SS          0
#define CFG_NRF_CE          0
#endif


#ifdef TARGET_RPI
/* spi */
#define CFG_SPI_CONTEXT     NULL
#define CFG_SPI_FREQUENCY   0
#define CFG_SPI_MISO        0
#define CFG_SPI_MOSI        0
#define CFG_SPI_SCLK        0
/* nrf24l01+ */
#define CFG_NRF_SS          8
#define CFG_NRF_CE          25
#endif

#ifdef TARGET_OPI
/* spi */
#define CFG_SPI_CONTEXT     NULL
#define CFG_SPI_FREQUENCY   0
#define CFG_SPI_MISO        0
#define CFG_SPI_MOSI        0
#define CFG_SPI_SCLK        0
/* nrf24l01+ */
#define CFG_NRF_SS          13
#define CFG_NRF_CE          2
#endif


#ifdef TARGET_ESP32
/* i2c */
#define CFG_I2C_CONTEXT     NULL
#define CFG_I2C_FREQUENCY   400000
#define CFG_I2C_SCL         23
#define CFG_I2C_SDA         18
// #define CFG_I2C_SCL         4 /* wemos integrated oled module */
// #define CFG_I2C_SDA         5 /* wemos integrated oled module */
/* spi */
#define CFG_SPI_CONTEXT     ((void *)HSPI_HOST)
#define CFG_SPI_FREQUENCY   0
#define CFG_SPI_MISO        GPIO_NUM_12
#define CFG_SPI_MOSI        GPIO_NUM_13
#define CFG_SPI_SCLK        GPIO_NUM_14
/* nrf24l01+ */
#define CFG_NRF_SS          GPIO_NUM_26
#define CFG_NRF_CE          GPIO_NUM_27
/* hd44780 */
#define CFG_HD44780_D4      GPIO_NUM_15
#define CFG_HD44780_D5      GPIO_NUM_2
#define CFG_HD44780_D6      GPIO_NUM_4
#define CFG_HD44780_D7      GPIO_NUM_5
#define CFG_HD44780_EN      GPIO_NUM_18
#define CFG_HD44780_RW      GPIO_NUM_19
#define CFG_HD44780_RS      GPIO_NUM_21
#endif

