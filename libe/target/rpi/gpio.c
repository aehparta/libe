/*
 * raspberry pi specific gpio.
 */

#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <libe/libe.h>


void gpio_enable(uint8_t pin, bool direction)
{
	char *buf;
	int fd, err;
	struct stat st;

	error_if(asprintf(&buf, "/sys/class/gpio/gpio%u/direction", pin) < 1, , "asprintf() failed");

	/* if gpio has not been enabled yet at all */
	if (stat(buf, &st)) {
		char *buf2;
		fd = open("/sys/class/gpio/export", O_WRONLY);
		error_if_errno(fd < 0,);
		error_if(asprintf(&buf2, "%u", pin) < 1, , "asprintf() failed");
		err = write(fd, buf2, strlen(buf2));
		close(fd);
		free(buf2);
		error_if(err < 0, , "write() failed when exporting gpio pin");

		/* need to let the change sink in, otherwise direction set will fail with "Permission denied"
		 *
		 * this problems has been produced at least on:
		 *  - Raspberry Pi 2 model B V1.2 using Rasbian Buster
		 *
		 * @todo this is not the right way, too slow etc, FIX!!!
		 */
		os_sleepf(0.05);
	}

	/* set gpio direction as input/output */
	fd = open(buf, O_WRONLY);
	free(buf);
	error_if_errno(fd < 0,);
	buf = direction ? "out" : "in";
	err = write(fd, buf, strlen(buf));
	close(fd);
	error_if(err < 0, , "write() failed when setting gpio pin direction");

	error_clear();
}

void gpio_set(uint8_t pin, uint8_t state)
{
	char *buf;
	int fd, err;

	error_if(asprintf(&buf, "/sys/class/gpio/gpio%u/value", pin) < 1, , "asprintf() failed");
	fd = open(buf, O_WRONLY);
	free(buf);
	error_if(fd < 0, , "unable to open gpio /sys/class/gpio/gpioX/value for writing");
	err = write(fd, state ? "1" : "0", 1);
	close(fd);
	error_if(err < 0, , "write() failed when setting gpio pin value");

	error_clear();
}

uint8_t gpio_read(uint8_t pin)
{
	char *buf, value[8];
	int fd, err;

	error_if(asprintf(&buf, "/sys/class/gpio/gpio%u/value", pin) < 1, 0, "asprintf() failed");
	fd = open(buf, O_RDONLY);
	free(buf);
	error_if(fd < 0, 0, "unable to open gpio /sys/class/gpio/gpioX/value for reading");

	err = read(fd, value, sizeof(value));
	close(fd);
	error_if(err < 0, 0, "read() failed when getting gpio pin value");

	error_clear();
	return atoi(value);
}
