/*
 * Blink LED using gpio calls.
 */

#include <libe/libe.h>


#ifdef TARGET_ESP32
int app_main(int argc, char *argv[])
#else
int main(int argc, char *argv[])
#endif
{
	void *context = I2C_CONTEXT;
	struct i2c_master i2c;
	struct i2c_device dev;

	/* low level init: initializes some system basics depending on platform */
	os_init();
	log_init();

	/* open i2c */
#if defined(TARGET_LINUX) && !defined(USE_I2C_BITBANG)
	ERROR_IF_R(argc < 2, 1, "give i2c device as first argument");
	context = argv[1];
#endif
	ERROR_IF_R(i2c_master_open(&i2c, context, I2C_FREQUENCY, I2C_SCL, I2C_SDA), 1, "unable to open i2c device");
	ERROR_IF_R(ak1595_open(&dev, &i2c, AK1595_A0), 1, "AK1595 not found");

	DEBUG_MSG("reg: %02x", i2c_read_reg_byte(&dev, 0x36));

	while (1) {
		os_wdt_reset();

		NOTICE_MSG("advertise");

		/* times to transmit (0 is continous) */
		i2c_write_reg_byte(&dev, 0x03, 0x00);

		/* continous transmit interval in ms (+adv_delay_enb) */
		uint16_t advintvl = 100 + 0x8000;
		i2c_write_reg_byte(&dev, 0x04, advintvl >> 8); /* high byte */
		i2c_write_reg_byte(&dev, 0x05, advintvl & 0xff); /* low byte */


		/* type */
		i2c_write_reg_byte(&dev, 0x0c, 0x02);

		/* data length */
		uint8_t size = 6 + 3; /* mac + flags */

		/* mac address */
		i2c_write_reg_byte(&dev, 0x0e, 0x01);
		i2c_write_reg_byte(&dev, 0x0f, 0x01);
		i2c_write_reg_byte(&dev, 0x10, 0x01);
		i2c_write_reg_byte(&dev, 0x11, 0x01);
		i2c_write_reg_byte(&dev, 0x12, 0x01);
		i2c_write_reg_byte(&dev, 0x13, 0x01);

		/* flags */
		i2c_write_reg_byte(&dev, 0x14, 2);
		i2c_write_reg_byte(&dev, 0x15, 0x01);
		i2c_write_reg_byte(&dev, 0x16, 0x05);

		/* name */
		i2c_write_reg_byte(&dev, 0x17, 5);
		i2c_write_reg_byte(&dev, 0x18, 0x08);
		i2c_write_reg_byte(&dev, 0x19, 0x62);
		i2c_write_reg_byte(&dev, 0x1a, 0x63);
		i2c_write_reg_byte(&dev, 0x1b, 0x64);
		i2c_write_reg_byte(&dev, 0x1c, 0x65);
		size += 6;

		/* battery level (uuid: 0x180f) */
		i2c_write_reg_byte(&dev, 0x1d, 4);
		i2c_write_reg_byte(&dev, 0x1e, 0x16);
		i2c_write_reg_byte(&dev, 0x1f, 0x0f);
		i2c_write_reg_byte(&dev, 0x20, 0x18);
		i2c_write_reg_byte(&dev, 0x21, 77);
		size += 5;

		/* set data length */
		i2c_write_reg_byte(&dev, 0x06, 0xc0 + 2 + size); /* full pdu length (+2 from mode and length fields inside data itself) */
		i2c_write_reg_byte(&dev, 0x0d, size);

		/* send */
		i2c_write_reg_byte(&dev, 0x36, 0x01);

		os_delay_ms(500);
		DEBUG_MSG("reg: %02x", i2c_read_reg_byte(&dev, 0x36));
		// i2c_write_reg_byte(&dev, 0x36, 0x00);
	}

	ak1595_close(&dev);
	i2c_master_close(&i2c);

	return 0;
}
