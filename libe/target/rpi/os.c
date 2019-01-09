/*
 * libe cross-platform library: operating system/target specific routines
 *
 * Authors:
 *  Antti Partanen <aehparta@iki.fi>
 */

#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <libe/os.h>
#include <libe/debug.h>


int os_gpio_enable(uint8_t pin, bool direction)
{
	char *buf;
	int fd;

	/* enable gpio pin */
	fd = open("/sys/class/gpio/export", O_WRONLY);
	CRIT_IF_R(fd < 0, -1, "unable to open gpio system for setting pin as gpio");
	CRIT_IF_R(asprintf(&buf, "%u", pin) < 1, -1, "asprintf() failed");
	WARN_IF(write(fd, buf, strlen(buf)) < 0, "could not write");
	close(fd);
	free(buf);

	/* set gpio direction as input/output */
	CRIT_IF_R(asprintf(&buf, "/sys/class/gpio/gpio%u/direction", pin) < 1, -1, "asprintf() failed");
	fd = open(buf, O_WRONLY);
	free(buf);
	buf = direction ? "out" : "in";
	CRIT_IF_R(fd < 0, -1, "unable to open gpio system for setting pin as %s", buf);
	WARN_IF(write(fd, buf, strlen(buf)) < 0, "could not write");
	close(fd);

	return 0;
}


int os_gpio_output(uint8_t pin)
{
	return os_gpio_enable(pin, OS_GPIO_OUTPUT);
}


int os_gpio_input(uint8_t pin)
{
	return os_gpio_enable(pin, OS_GPIO_INPUT);
}


int os_gpio_set(uint8_t pin, bool state)
{
	char *buf;
	int fd;
	CRIT_IF_R(asprintf(&buf, "/sys/class/gpio/gpio%u/value", pin) < 1, -1, "asprintf() failed");
	fd = open(buf, O_WRONLY);
	free(buf);
	CRIT_IF_R(fd < 0, -1, "unable to open gpio system for setting pin state, pin: %d", pin);
	WARN_IF(write(fd, state ? "1" : "0", 1) < 0, "could not write");
	close(fd);
	return 0;
}


int os_gpio_high(uint8_t pin)
{
	return os_gpio_set(pin, true);
}


int os_gpio_low(uint8_t pin)
{
	return os_gpio_set(pin, false);
}


const char *os_get_revision(void)
{
	static char *revision = NULL;
	FILE *f = NULL;
	size_t n = 0;
	int i;

	/* if revision has already been read */
	if (revision) {
		return (const char *)revision;
	}

	/* pre-allocate some buffer */
	revision = (char *)malloc(32);
	n = 32;
	memset(revision, 0, n);

	f = fopen("/proc/cpuinfo", "r");
	ERROR_IF_R(!f, revision, "unable to open /proc/cpuinfo");

	/* read until "Revision" is found */
	while (1) {
		/* if not found or some reading error */
		if (getline(&revision, &n, f) < 0) {
			/* return empty revision */
			WARN_MSG("revision could not be found");
			memset(revision, 0, n);
			return revision;
		}
		if (strncmp(revision, "Revision", strlen("Revision")) != 0) {
			continue;
		}

		/* trim */
		for (i = strlen(revision) - 1; ; i--) {
			if (isspace(revision[i])) {
				revision[i] = '\0';
			} else {
				break;
			}
		}

		/* parse to start of revision string */
		for ( ; ; i--) {
			if (isspace(revision[i])) {
				revision = &revision[i + 1];
				break;
			}
		}
		break;
	}

	return revision;
}
