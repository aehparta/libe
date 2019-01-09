/*
 * libe cross-platform library
 *
 * Authors:
 *  Antti Partanen <aehparta@iki.fi>
 */

#include <string.h>
#include <libe/debug.h>
#include <libe/os.h>
#include "ftdi.h"


struct os_ftdi {
	struct ftdi_context *ftdi;
	uint8_t mode;
	uint8_t pins[2];
	uint8_t dirs[2];
};

static struct os_ftdi fdevs[16];


void os_ftdi_init(void)
{
	memset(fdevs, 0, sizeof(fdevs));
}

void os_ftdi_quit(void)
{
	for (int i = 0; i < 16; i++) {
		if (!fdevs[i].ftdi) {
			continue;
		}
		DEBUG_MSG("free interface %d", i);
		if ((i % 4) == 0) {
			ftdi_set_bitmode(fdevs[i].ftdi, 0, BITMODE_RESET);
		}
		ftdi_free(fdevs[i].ftdi);
	}
}

int os_ftdi_use(int pin_range, uint16_t vid, uint16_t pid, const char *description, const char *serial)
{
	struct ftdi_context *ftdi;
	struct ftdi_device_list *list, *list_first;
	int i, n;
	uint8_t b = 0;

	/* initialize ftdi */
	ftdi = ftdi_new();
	WARN_IF_R(!ftdi, -1, "ftdi_new() failed");
	ftdi_set_interface(ftdi, INTERFACE_A);

	/* list all somewhat matching devices */
	n = ftdi_usb_find_all(ftdi, &list, vid, pid);
	if (n < 1) {
		WARN_MSG("unable to find any matching device");
		ftdi_free(ftdi);
		return -1;
	}

	/* save list start for freeing whole list later */
	list_first = list;

	/* try to find matching device if more precise description or serial was given */
	if (description || serial) {
		for (i = 0; i < n; i++) {
			char m[1024], d[1024], s[1024];
			memset(m, 0, 1024);
			memset(d, 0, 1024);
			memset(s, 0, 1024);
			ftdi_usb_get_strings(ftdi, list->dev, m, 1024, d, 1024, s, 1024);
			if (description) {
				if (strcmp(description, d) != 0) {
					list = list->next;
					continue;
				}
			}
			if (serial) {
				if (strcmp(serial, s) != 0) {
					list = list->next;
					continue;
				}
			}
			break;
		}
		if (i >= n) {
			WARN_MSG("unable to find any matching device");
			ftdi_list_free(&list_first);
			ftdi_free(ftdi);
			return -1;
		}
	}

	/* set interface and try to open */
	if (ftdi_usb_open_dev(ftdi, list->dev)) {
		WARN_MSG("unable to open device");
		ftdi_list_free(&list_first);
		ftdi_free(ftdi);
		return -1;
	}
	ftdi_set_latency_timer(ftdi, 1);
	ftdi_usb_purge_buffers(ftdi);
	ftdi_set_bitmode(ftdi, 0x00, BITMODE_BITBANG);
	ftdi_write_data(ftdi, &b, 1);
	fdevs[pin_range * 4].ftdi = ftdi;
	fdevs[pin_range * 4].mode = BITMODE_BITBANG;

	/* open rest of the interfaces that might be there */
	for (i = 1; i < 4; i++) {
		ftdi = ftdi_new();
		ftdi_set_interface(ftdi, INTERFACE_A + i);
		if (ftdi_usb_open_dev(ftdi, list->dev)) {
			ftdi_free(ftdi);
			break;
		}
		ftdi_set_latency_timer(ftdi, 1);
		ftdi_usb_purge_buffers(ftdi);
		ftdi_set_bitmode(ftdi, 0x00, BITMODE_BITBANG);
		ftdi_write_data(ftdi, &b, 1);
		fdevs[pin_range * 4 + i].ftdi = ftdi;
		fdevs[pin_range * 4 + i].mode = BITMODE_BITBANG;
	}
	DEBUG_MSG("device has %d interfaces", i);

	ftdi_list_free(&list_first);

	return 0;
}

struct ftdi_context *os_ftdi_direct_open(uint16_t vid, uint16_t pid, int interface, const char *description, const char *serial, int reset)
{
	int err, i, n;
	struct ftdi_context *ftdi;
	struct ftdi_device_list *list, *list_first;

	/* initialize ftdi */
	ftdi = ftdi_new();
	if (!ftdi) {
		ERROR_MSG("ftdi_new() failed");
		return NULL;
	}
	err = ftdi_set_interface(ftdi, interface);
	if (err < 0) {
		ERROR_MSG("unable to set selected interface on ftdi device: %d (%s)", err, ftdi_get_error_string(ftdi));
		ftdi_free(ftdi);
		return NULL;
	}

	/* list all somewhat matching devices */
	n = ftdi_usb_find_all(ftdi, &list, vid, pid);
	if (n < 1) {
		DEBUG_MSG("unable to find any matching device");
		ftdi_free(ftdi);
		return NULL;
	}

	/* save list start for freeing whole list later */
	list_first = list;

	/* try to find matching device if more precise description or serial was given */
	if (description || serial) {
		for (i = 0; i < n; i++) {
			char m[1024], d[1024], s[1024];
			memset(m, 0, 1024);
			memset(d, 0, 1024);
			memset(s, 0, 1024);
			ftdi_usb_get_strings(ftdi, list->dev, m, 1024, d, 1024, s, 1024);
			if (description) {
				if (strcmp(description, d) != 0) {
					list = list->next;
					continue;
				}
			}
			if (serial) {
				if (strcmp(serial, s) != 0) {
					list = list->next;
					continue;
				}
			}
			break;
		}
		if (i >= n) {
			DEBUG_MSG("unable to find any matching device");
			ftdi_list_free(&list);
			ftdi_free(ftdi);
			return NULL;
		}
	}

	err = ftdi_usb_open_dev(ftdi, list->dev);
	ftdi_list_free(&list_first);
	if (err < 0) {
		DEBUG_MSG("unable to open ftdi device: %s", ftdi_get_error_string(ftdi));
		ftdi_free(ftdi);
		return NULL;
	}

	/* reset chip */
	if (reset) {
		if (ftdi_usb_reset(ftdi)) {
			DEBUG_MSG("failed to reset device: %s", ftdi_get_error_string(ftdi));
			ftdi_free(ftdi);
			return NULL;
		}
	}

	return ftdi;
}

int os_ftdi_gpio_enable(uint8_t pin, bool direction)
{
	uint8_t pin_range = pin >> 4;
	uint8_t ud = (pin & 0x08) ? 1 : 0;
	uint8_t p = (1 << (pin & 0x07));

	ERROR_IF_R(!fdevs[pin_range].ftdi, -1, "ftdi gpio %d is not available", pin);
	ERROR_IF_R(fdevs[pin_range].mode != BITMODE_MPSSE && ud, -1, "ftdi gpio %d is not available in bitbang mode", pin);

	if ((fdevs[pin_range].dirs[ud] & p) == (direction == OS_GPIO_OUTPUT ? p : 0)) {
		/* no changes, just return */
		return 0;
	}

	if (direction == OS_GPIO_OUTPUT) {
		fdevs[pin_range].dirs[ud] |= p;
	} else {
		fdevs[pin_range].dirs[ud] &= ~p;
	}

	if (fdevs[pin_range].mode == BITMODE_BITBANG) {
		ftdi_set_bitmode(fdevs[pin_range].ftdi, fdevs[pin_range].dirs[ud], BITMODE_BITBANG);
		ftdi_write_data(fdevs[pin_range].ftdi, &fdevs[pin_range].pins[ud], 1);
	}

	/*
		master->state |= pins;
		uint8_t cmd[] = {
			SET_BITS_LOW, master->state, master->dir
		};
		if (ftdi_write_data(master->ftdi, cmd, sizeof(cmd)) != sizeof(cmd)) {
			return -1;
		}
	*/
	return 0;
}

int os_ftdi_gpio_set(uint8_t pin, bool state)
{
	uint8_t pin_range = pin >> 4;
	uint8_t ud = (pin & 0x08) ? 1 : 0;
	uint8_t p = (1 << (pin & 0x07));

	ERROR_IF_R(!fdevs[pin_range].ftdi, -1, "ftdi gpio %d is not available", pin);
	ERROR_IF_R(fdevs[pin_range].mode != BITMODE_MPSSE && ud, -1, "ftdi gpio %d is not available in bitbang mode", pin);

	if ((fdevs[pin_range].pins[ud] & p) == (state ? p : 0)) {
		/* no changes, just return */
		return 0;
	}

	if (state) {
		fdevs[pin_range].pins[ud] |= p;
	} else {
		fdevs[pin_range].pins[ud] &= ~p;
	}

	if (fdevs[pin_range].mode == BITMODE_BITBANG) {
		ftdi_write_data(fdevs[pin_range].ftdi, &fdevs[pin_range].pins[ud], 1);
	}

	/*
		master->state &= ~pins;
		uint8_t cmd[] = {
			SET_BITS_LOW, master->state, master->dir
		};
		if (ftdi_write_data(master->ftdi, cmd, sizeof(cmd)) != sizeof(cmd)) {
			return -1;
		}
	*/
	return 0;
}

int os_ftdi_gpio_read(uint8_t pin)
{
	return -1;
}
