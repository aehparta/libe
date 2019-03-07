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

struct i2c_master {};
struct i2c_device {
	uint8_t address;
};

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
#define I2C_SCL_IO          8

/* I2C SDA port and pin definitions */
#define I2C_SDA_IO          9

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
    if (bit) { I2C_SDA_HIGH(); } \
    else I2C_SDA_LOW(); \
    I2C_SCL_HIGH(); \
    I2C_DELAY(); \
    I2C_SCL_LOW(); \
    I2C_DELAY(); \
} while (0);

/* I2C write bit from byte */
#define I2C_WRITE(byte, bit) \
do { \
    if (byte & _BV(bit)) { I2C_SDA_HIGH(); } \
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
struct i2c_master {
	int fd;
};
struct i2c_device {
	struct i2c_master *master;
	uint8_t address;
};
#endif


/*
 * I2C functions
 */

int i2c_master_open(struct i2c_master *master, void *context);
void i2c_master_close(struct i2c_master *master);

int i2c_open(struct i2c_device *dev, struct i2c_master *master, uint8_t address);
void i2c_close(struct i2c_device *dev);

int i2c_read(struct i2c_device *i2c, void *data, size_t size);
int i2c_write(struct i2c_device *i2c, void *data, size_t size);


#endif /* _I2C_H_ */

