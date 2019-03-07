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
#include <libe/log.h>
#include <libe/i2c.h>


int i2c_open(struct i2c_context *i2c, void *context)
{
	IF_R(!context, -1);
	i2c->fd = open(context, O_RDWR);
	IF_R(i2c->fd < 0, -1);
	return 0;
}


void i2c_close(struct i2c_context *i2c)
{
	if (i2c->fd > -1) {
		close(i2c->fd);
	}
}


int i2c_addr7(struct i2c_context *i2c, uint8_t addr, uint8_t rw)
{
	int err = ioctl(i2c->fd, I2C_SLAVE, addr);
	ERROR_IF_R(err < 0, -1, "i2c error when setting slave address: 0x%02x", addr);
	return err;
}


int i2c_read_byte(struct i2c_context *i2c, int nack)
{
	uint8_t byte;
	IF_R(read(i2c->fd, &byte, 1) != 1, -1);
	return byte;
}


int i2c_write_byte(struct i2c_context *i2c, uint8_t byte)
{
	IF_R(write(i2c->fd, &byte, 1) != 1, -1);
	return 0;
}


void i2c_stop(struct i2c_context *i2c)
{

}
