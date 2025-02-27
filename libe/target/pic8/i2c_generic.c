/*
 * pic8 I2C drivers
 *
 * Authors: Antti Partanen <aehparta@iki.fi>
 */

#if defined(USE_I2C) && !defined(USE_I2C_BITBANG)

#include <libe/libe.h>


#define SEN SSP2CON2bits.SEN
#define RSEN SSP2CON2bits.RSEN
#define PEN SSP2CON2bits.PEN
#define RCEN SSP2CON2bits.RCEN
#define ACKEN SSP2CON2bits.ACKEN
#define ACKDT SSP2CON2bits.ACKDT
#define SSPBUF SSP2BUF
#define BF SSP2STATbits.BF
#define ACKSTAT SSP2CON2bits.ACKSTAT
#define SSPIF PIR4bits.SSP2IF
#define BCLIF PIR4bits.BCL2IF

#define _SSPCON1 SSP2CON1
#define _SSPCON2 SSP2CON2
#define _SSPCON3 SSP2CON3
#define _SSPADD SSP2ADD
#define _SSPSTAT SSP2STAT

int8_t i2c_master_open(struct i2c_master *master, void *context, uint32_t frequency, uint8_t scl, uint8_t sda)
{
    /* i2c pins must be inputs */
    gpio_input(sda);
    gpio_input(scl);

    /* calculate clock */
    if (frequency < 1) {
        /* default to 100kHz */
        frequency = 1e5;
    }
    uint8_t sspadd = (uint8_t)((F_CPU / frequency / 4) - 1);

    /* enable i2c */
    _SSPCON1 = 0x28;
    _SSPCON2 = 0x00;
    _SSPCON3 = 0x00;
    _SSPADD = sspadd;
    _SSPSTAT = 0x00;
    SSPIF = 0;
    BCLIF = 0;

    return 0;
}

void i2c_master_close(struct i2c_master *master)
{
    SSP2CON1 = 0x00;
}

int8_t i2c_open(struct i2c_device *dev, struct i2c_master *master, uint8_t address)
{
    dev->master = master;
    dev->address = (uint8_t)(address << 1);
    return i2c_write(dev, NULL, 0);
}

void i2c_close(struct i2c_device *dev)
{
}

int8_t i2c_read(struct i2c_device *dev, void *data, uint8_t size)
{
    SEN = 1;
    while (!SSPIF);
    SSPIF = 0;

    SSPBUF = dev->address | 1;
    while (BF);
    if (ACKSTAT) {
        /* no ack */
        PEN = 1;
        while (!SSPIF);
        SSPIF = 0;
        return -1;
    }

    for (uint8_t *p = data; size > 0; size--, p++) {
        RCEN = 1;
        while (!BF);
        *p = SSPBUF;
        ACKDT = size == 1 ? 1 : 0; /* nack/ack */
        ACKEN = 1;
        while (!SSPIF);
        SSPIF = 0;
    }

    PEN = 1;
    while (!SSPIF);
    SSPIF = 0;

    return 0;
}

int8_t i2c_write(struct i2c_device *dev, void *data, uint8_t size)
{
    SEN = 1;
    while (!SSPIF);
    SSPIF = 0;

    SSPBUF = dev->address;
    while (!SSPIF);
    SSPIF = 0;
    if (ACKSTAT) {
        /* no ack */
        PEN = 1;
        while (!SSPIF);
        SSPIF = 0;
        return -1;
    }

    for (uint8_t *p = data; size > 0; size--, p++) {
        SSPBUF = *p;
        while (!SSPIF);
        if (ACKSTAT) {
            /* no ack */
            PEN = 1;
            while (!SSPIF);
            SSPIF = 0;
            return -1;
        }
    }

    PEN = 1;
    while (!SSPIF);
    SSPIF = 0;

    return 0;
}

#endif
