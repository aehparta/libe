/*
 * libe cross-platform library: I2C linux drivers
 *
 * Authors:
 *  Antti Partanen <aehparta@iki.fi>
 */

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <libe/os.h>
#include <libe/debug.h>
#include <libe/i2c.h>


struct i2c_context *i2c_open(void *context)
{
	struct i2c_context *i2c;
	int fd;

	if (!context) {
		WARN_MSG("i2c device is invalid (null)");
		return NULL;
	}

	fd = open(context, O_RDWR);
	ERROR_IF_R(fd < 0, NULL, "unable to open i2c device: %s", context);

	// ERR_IF_R(ioctl(fd, I2C_SLAVE, SSD1306_I2C_ADDR) < 0 ) {
	// 	printf("SSD1306 ioctl error : %s\r\n", strerror(errno));
	// }

	SALLOC(i2c, NULL);
	i2c->fd = fd;

	return i2c;
}


void i2c_close(struct i2c_context *i2c)
{
	if (i2c) {
		if (i2c->fd > -1) {
			close(i2c->fd);
		}
		free(i2c);
	}
}


int i2c_addr7(struct i2c_context *i2c, uint8_t addr, uint8_t rw)
{
	ERROR_IF_R(ioctl(i2c->fd, I2C_SLAVE, addr) < 0, -1, "i2c error when setting slave address: 0x%02x", addr);
	return 0;
}


uint8_t i2c_read_byte(struct i2c_context *i2c, int nack)
{
	return -1;
}


int i2c_write_byte(struct i2c_context *i2c, uint8_t byte)
{
	ERROR_IF_R(write(i2c->fd, &byte, 1) != 1, -1, "i2c write byte failed");
	return 0;
}


void i2c_stop(struct i2c_context *i2c)
{

}
