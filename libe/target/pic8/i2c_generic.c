/*
 * pic8 I2C drivers
 *
 * Authors: Antti Partanen <aehparta@iki.fi>
 */

#if defined(USE_I2C) && !defined(USE_I2C_BITBANG)

#include <libe/libe.h>


#if PIC8_I2C_MSSP == 1

#define CON1        SSP1CON1
#define CON2        SSP1CON2
#define CON3        SSP1CON3
#define ADD         SSP1ADD
#define STAT        SSP1STAT

#define SEN         SSP1CON2bits.SEN
#define RSEN        SSP1CON2bits.RSEN
#define PEN         SSP1CON2bits.PEN
#define RCEN        SSP1CON2bits.RCEN
#define ACKEN       SSP1CON2bits.ACKEN
#define ACKDT       SSP1CON2bits.ACKDT
#define SSPBUF      SSP1BUF
#define BF          SSP1STATbits.BF
#define ACKSTAT     SSP1CON2bits.ACKSTAT

#define WAIT_IDLE() while (SSP1STATbits.R_nW | (0x1F & SSP1CON2))

#elif PIC8_I2C_MSSP == 2

#define CON1        SSP2CON1
#define CON2        SSP2CON2
#define CON3        SSP2CON3
#define ADD         SSP2ADD
#define STAT        SSP2STAT

#define SEN         SSP2CON2bits.SEN
#define RSEN        SSP2CON2bits.RSEN
#define PEN         SSP2CON2bits.PEN
#define RCEN        SSP2CON2bits.RCEN
#define ACKEN       SSP2CON2bits.ACKEN
#define ACKDT       SSP2CON2bits.ACKDT
#define SSPBUF      SSP2BUF
#define BF          SSP2STATbits.BF
#define ACKSTAT     SSP2CON2bits.ACKSTAT

#define WAIT_IDLE() while (SSP2STATbits.R_nW | (0x1F & SSP2CON2))

#else
#error "no I2C MSSP module selected, define PIC8_I2C_MSSP to 1 or 2"
#endif


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
    CON1 = 0x28;
    CON2 = 0x00;
    CON3 = 0x00;
    ADD = sspadd;
    STAT = 0x00;

    return 0;
}

void i2c_master_close(struct i2c_master *master)
{
    CON1 = 0x00;
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
    WAIT_IDLE();
    SEN = 1;
    while (SEN);

    SSPBUF = dev->address | 1;
    WAIT_IDLE();
    if (ACKSTAT) {
        /* no ack */
        PEN = 1;
        return -1;
    }

    for (uint8_t *p = data; size > 0; size--, p++) {
        RCEN = 1;
        WAIT_IDLE();
        *p = SSPBUF;
        if (size > 1) {
            ACKDT = 0; /* ack */
            ACKEN = 1;
            WAIT_IDLE();
        } else {
            ACKDT = 1; /* nack */
            ACKEN = 1;
            while (ACKEN);
        }
    }

    PEN = 1;
    return 0;
}

int8_t i2c_write(struct i2c_device *dev, void *data, uint8_t size)
{
    WAIT_IDLE();
    SEN = 1;
    while (SEN);

    SSPBUF = dev->address;
    WAIT_IDLE();
    if (ACKSTAT) {
        /* no ack */
        PEN = 1;
        return -1;
    }

    for (uint8_t *p = data; size > 0; size--, p++) {
        SSPBUF = *p;
        WAIT_IDLE();
        if (ACKSTAT) {
            /* no ack */
            PEN = 1;
            return -1;
        }
    }

    PEN = 1;
    return 0;
}

#endif
