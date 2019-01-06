/*
 * libe cross-platform library: I2C bitbang drivers
 *
 * Authors:
 *  Antti Partanen <aehparta@iki.fi>
 */

#ifndef _I2C_H_
#define _I2C_H_

#include <stdint.h>
#if defined(TARGET_X86) || defined(TARGET_RPI)
#include <linux/i2c-dev.h>
#include <sys/ioctl.h>
#endif
#include <libe/os.h>


#if defined(TARGET_AVR) || defined(TARGET_PIC8)  || defined(TARGET_MSP430)

struct i2c_context {};

#ifndef F_I2C
/**
 * I2C clock in hertz.
 */
#define F_I2C               200000
#endif

#ifdef TARGET_AVR

/* I2C clock half wave in CPU clocks */
#define I2C_DELAY()         _delay_loop_1(F_CPU / F_I2C / 3 / 2)

/* I2C SCL port and pin definitions */
#define I2C_SCL_IO          (6 + (8 * 3)) /* pin 6 on PORTD */

/* I2C SDA port and pin definitions */
#define I2C_SDA_IO          (5 + (8 * 3)) /* pin 5 on PORTD */

#elif TARGET_PIC8

/* I2C clock half wave in CPU clocks */
#define I2C_DELAY()         __delay_us(1e6 / F_I2C / 2)

/* I2C SCL port and pin definitions */
#define I2C_SCL_PORT        A
#define I2C_SCL_IO          6

/* I2C SDA port and pin definitions */
#define I2C_SDA_PORT        A
#define I2C_SDA_IO          7

#elif TARGET_MSP430

/* I2C clock half wave in CPU clocks */
#define I2C_DELAY()         __delay_cycles(F_CPU / F_I2C / 2)

/* I2C SCL port and pin definitions */
#define I2C_SCL_PORT        4
#define I2C_SCL_IO          0

/* I2C SDA port and pin definitions */
#define I2C_SDA_PORT        4
#define I2C_SDA_IO          1

#endif

/* SCL macros */
#define I2C_SCL_INPUT()     os_gpio_input(I2C_SCL_IO)
#define I2C_SCL_OUTPUT()    os_gpio_output(I2C_SCL_IO)
#define I2C_SCL_LOW()       os_gpio_low(I2C_SCL_IO)
#define I2C_SCL_HIGH()      os_gpio_high(I2C_SCL_IO)
#define I2C_SCL_READ()      os_gpio_read(I2C_SCL_IO)
/* SDA macros */
#define I2C_SDA_INPUT()     os_gpio_input(I2C_SDA_IO)
#define I2C_SDA_OUTPUT()    os_gpio_output(I2C_SDA_IO)
#define I2C_SDA_LOW()       os_gpio_low(I2C_SDA_IO)
#define I2C_SDA_HIGH()      os_gpio_high(I2C_SDA_IO)
#define I2C_SDA_READ()      os_gpio_read(I2C_SDA_IO)

/* I2C write zero */
#define I2C_WRITE_ZERO() \
do { \
    I2C_SDA_LOW(); \
    I2C_SCL_HIGH(); \
    I2C_DELAY(); \
    I2C_SCL_LOW(); \
    I2C_DELAY(); \
} while (0);

/* I2C write one */
#define I2C_WRITE_ONE() \
do { \
    I2C_SDA_HIGH(); \
    I2C_SCL_HIGH(); \
    I2C_DELAY(); \
    I2C_SCL_LOW(); \
    I2C_DELAY(); \
} while (0);

/* I2C write bit */
#define I2C_WRITE_BIT(bit) \
do { \
    if (bit) I2C_SDA_HIGH(); \
    else I2C_SDA_LOW(); \
    I2C_SCL_HIGH(); \
    I2C_DELAY(); \
    I2C_SCL_LOW(); \
    I2C_DELAY(); \
} while (0);

/* I2C write bit from byte */
#define I2C_WRITE(byte, bit) \
do { \
    if (byte & _BV(bit)) I2C_SDA_HIGH(); \
    else I2C_SDA_LOW(); \
    I2C_SCL_HIGH(); \
    I2C_DELAY(); \
    I2C_SCL_LOW(); \
    I2C_DELAY(); \
} while (0);

/* I2C read bit */
static inline uint8_t I2C_READ_BIT(void)
{
    uint8_t bit;
    I2C_SCL_HIGH();
    I2C_DELAY();
    bit = I2C_SDA_READ();
    I2C_SCL_LOW();
    I2C_DELAY();
    return bit;
}

/* I2C read bit to byte */
#define I2C_READ(byte, bit) \
do { \
    I2C_SCL_HIGH(); \
    I2C_DELAY(); \
    byte |= I2C_SDA_READ() << bit; \
    I2C_SCL_LOW(); \
    I2C_DELAY(); \
} while (0);

#elif TARGET_X86 || TARGET_RPI
struct i2c_context {
    int fd;
    uint8_t last_addr;
};
#endif


/**
 * Initialize I2C bitbang.
 */
struct i2c_context *i2c_open(void *context);
void i2c_close(struct i2c_context *i2c);

int i2c_addr7(struct i2c_context *i2c, uint8_t addr, uint8_t rw);
uint8_t i2c_read_byte(struct i2c_context *i2c, int nack);
int i2c_write_byte(struct i2c_context *i2c, uint8_t byte);
void i2c_stop(struct i2c_context *i2c);

/** Read two bytes, LSB first. */
uint16_t i2c_read_u16(struct i2c_context *i2c, int nack);
/** Read four bytes, LSB first. */
uint32_t i2c_read_u32(struct i2c_context *i2c, int nack);

/**
 * Read register:
 *  - start
 *  - write register
 *  - stop
 *  - start
 *  - read byte
 *  - stop
 */
uint8_t i2c_read_reg_byte(struct i2c_context *i2c, uint8_t address, uint8_t reg);

void i2c_write_reg_byte(struct i2c_context *i2c, uint8_t address, uint8_t reg, uint8_t value);

/* i2c test of all addresses to find i2c devices when debugging */
void i2c_test_dump(void);


#endif /* _I2C_H_ */

