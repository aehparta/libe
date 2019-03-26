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
#include <linux/i2c-dev.h>
#include <linux/i2c.h>
#include <libe/os.h>
#include <libe/log.h>
#include <libe/i2c.h>


int i2c_master_open(struct i2c_master *master, void *context, uint32_t frequency, uint8_t scl, uint8_t sda)
{
	struct stat st;

	/* check that i2c device is valid character device */
	IF_R(!context, -1);
	IF_R(stat(context, &st), -1);
	IF_R((st.st_mode & S_IFMT) != S_IFCHR, -1);

	/* open device */
	master->fd = open(context, O_RDWR);
	IF_R(master->fd < 0, -1);

	return 0;
}

void i2c_master_close(struct i2c_master *master)
{
	if (master->fd > -1) {
		close(master->fd);
	}
}

int i2c_open(struct i2c_device *dev, struct i2c_master *master, uint8_t address)
{
	dev->master = master;
	dev->address = address;
	/* do a write without any data to detect if there is a device at this address */
	return i2c_write(dev, NULL, 0);
}

void i2c_close(struct i2c_device *dev)
{

}

int i2c_read(struct i2c_device *dev, void *data, size_t size)
{
    struct i2c_msg msgs[1];
    struct i2c_rdwr_ioctl_data msgset[1];

    msgs[0].addr = dev->address;
    msgs[0].flags = I2C_M_RD;
    msgs[0].len = size;
    msgs[0].buf = data;

    msgset[0].msgs = msgs;
    msgset[0].nmsgs = 1;

    IF_R(ioctl(dev->master->fd, I2C_RDWR, &msgset) < 0, -1);

    return 0;
}

int i2c_write(struct i2c_device *dev, void *data, size_t size)
{
    struct i2c_msg msgs[1];
    struct i2c_rdwr_ioctl_data msgset[1];

    msgs[0].addr = dev->address;
    msgs[0].flags = 0;
    msgs[0].len = size;
    msgs[0].buf = data;

    msgset[0].msgs = msgs;
    msgset[0].nmsgs = 1;

    IF_R(ioctl(dev->master->fd, I2C_RDWR, &msgset) < 0, -1);

    return 0;
}

