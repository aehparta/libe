/*
 * pic8 I2C drivers
 *
 * Authors: Antti Partanen <aehparta@iki.fi>
 */

#if defined(USE_I2C) && !defined(USE_I2C_BITBANG)

#include <libe/libe.h>


int8_t i2c_master_open(struct i2c_master *master, void *context, uint32_t frequency, uint8_t scl, uint8_t sda)
{
    /* i2c pins must be input as default */
	gpio_input(scl);
	gpio_input(sda);

	/* mapping of pins */
	SSP2CLKPPS = scl;
	SSP2DATPPS = sda;
	os_pin_pps(scl, 0x1a);
	os_pin_pps(sda, 0x1b);

	/* calculate clock */
	if (frequency < 1) {
		/* default to 100kHz */
		frequency = 1e5;
	}
	uint8_t sspadd = (uint8_t)((F_CPU / frequency / 4) - 1);

	/* enable i2c */
#ifdef SSPCON1
	SSPSTAT = 0x00;
	SSPCON1 = 0x28;
	SSPCON2 = 0;
	SSPCON3 = 0;
	SSPADD = sspadd;
#elif defined(SSP2CON1)
	SSP2STAT = 0x00;
	SSP2CON1 = 0x28;
	SSP2CON2 = 0;
	SSP2CON3 = 0;
	SSP2ADD = sspadd;
#else
	SSPSTAT = 0x00;
	SSPCON = 0x28;
	SSPADD = sspadd;
#endif

	/* clear interrupt flags */
	PIR2bits.SSP2IF = 0;
	PIR2bits.BCL2IF = 0;

	return 0;
}

void i2c_master_close(struct i2c_master *master)
{
}

int8_t i2c_open(struct i2c_device *dev, struct i2c_master *master, uint8_t address)
{
	dev->master = master;
	dev->address = (uint8_t)address << 1;
	return i2c_write(dev, NULL, 0);
}

void i2c_close(struct i2c_device *dev)
{

}

int8_t i2c_read(struct i2c_device *dev, void *data, uint8_t size)
{
	uint8_t *p = data;
	return 0;
}

int8_t i2c_write(struct i2c_device *dev, void *data, uint8_t size)
{
	SSP2CON2bits.SEN = 1;
	while (SSP2STATbits.BF);
	
	os_delay_ms(100);

	SSP2BUF = dev->address;
	while (SSP2STATbits.BF);
	if (SSP2CON2bits.ACKSTAT) {
		/* no ack */
		SSP2CON2bits.PEN = 1;
		return -1;
	}

	for (uint8_t *p = data; size > 0; size--, p++) {
		SSP2BUF = *p;
		while (SSP2STATbits.BF);
		*p = SSP2BUF;
	}

	SSP2CON2bits.PEN = 1;

	return 0;
}

#endif
