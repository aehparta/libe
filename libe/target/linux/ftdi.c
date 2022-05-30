/*
 * libe cross-platform library
 *
 * Authors:
 *  Antti Partanen <aehparta@iki.fi>
 */

#ifdef USE_FTDI

#include <string.h>
#include <libe/libe.h>


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
		// DEBUG_MSG("free interface %d", i);
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
	uint8_t zero = 0;

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
	ftdi_usb_reset(ftdi);

	/* first interface is always there */
	ftdi_set_latency_timer(ftdi, 1);
	ftdi_write_data_set_chunksize(ftdi, 256);
	ftdi_read_data_set_chunksize(ftdi, 256);
	ftdi_set_bitmode(ftdi, 0, BITMODE_RESET);
	ftdi_set_bitmode(ftdi, 0, BITMODE_BITBANG);
	ftdi_set_baudrate(ftdi, 1e6);
	ftdi_write_data(ftdi, &zero, 1);
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
		ftdi_write_data_set_chunksize(ftdi, 256);
		ftdi_read_data_set_chunksize(ftdi, 256);
		ftdi_set_bitmode(ftdi, 0, BITMODE_RESET);
		ftdi_set_bitmode(ftdi, 0, BITMODE_BITBANG);
		ftdi_set_baudrate(ftdi, 1e6);
		ftdi_write_data(ftdi, &zero, 1);
		fdevs[pin_range * 4 + i].ftdi = ftdi;
		fdevs[pin_range * 4 + i].mode = BITMODE_BITBANG;
	}
	// DEBUG_MSG("device has %d interfaces", i);

	ftdi_list_free(&list_first);

	return 0;
}

int os_ftdi_set_mpsse(int pin)
{
	int i = pin >> 4;
	uint16_t divisor = 60e6 / 1e5 / 2 - 1;

	ERROR_IF_R(!fdevs[i].ftdi, -1, "cannot change mode to mpsse, interface is not open");

	/* setup mpsse */
	ftdi_set_bitmode(fdevs[i].ftdi, 0, BITMODE_RESET);
	ERROR_IF_R(ftdi_set_bitmode(fdevs[i].ftdi, 0, BITMODE_MPSSE), -1, "unable to enable mpsse bitmode");

	uint8_t cmd[] = {
		DIS_DIV_5,
		TCK_DIVISOR, divisor & 0xff, divisor >> 8,
		DIS_ADAPTIVE,
		DIS_3_PHASE,
		SET_BITS_LOW, fdevs[i].pins[0], fdevs[i].dirs[0],
		SET_BITS_HIGH, fdevs[i].pins[1], fdevs[i].dirs[1]
	};
	ERROR_IF_R(ftdi_write_data(fdevs[i].ftdi, cmd, sizeof(cmd)) != sizeof(cmd), -1, "mpsse setup failed: %s", ftdi_get_error_string(fdevs[i].ftdi));

	fdevs[i].mode = BITMODE_MPSSE;

	return 0;
}

int os_ftdi_get_mode(int pin)
{
	int i = pin >> 4;
	if (fdevs[i].ftdi) {
		return fdevs[i].mode;
	}
	return -1;
}

struct ftdi_context *os_ftdi_get_context(int pin)
{
	return fdevs[pin >> 4].ftdi;
}

int os_ftdi_has_pin(uint8_t pin)
{
	int pin_range = pin >> 4;
	int ud = (pin & 0x08) ? 1 : 0;

	ERROR_IF_R(!fdevs[pin_range].ftdi, -1, "ftdi gpio %d is not available", pin);
	ERROR_IF_R(fdevs[pin_range].mode != BITMODE_MPSSE && ud, -1, "ftdi gpio %d is not available in bitbang mode", pin);

	return 0;
}

#ifdef USE_GPIO

static int os_ftdi_gpio_check_pin(uint8_t pin, uint8_t *pin_range, uint8_t *ud, uint8_t *p)
{
	*pin_range = pin >> 4;
	*ud = (pin & 0x08) ? 1 : 0;
	*p = (1 << (pin & 0x07));

	ERROR_IF_R(!fdevs[*pin_range].ftdi, -1, "ftdi gpio %d is not available", pin);
	ERROR_IF_R(fdevs[*pin_range].mode != BITMODE_MPSSE && *ud, -1, "ftdi gpio %d is not available in bitbang mode", pin);

	return 0;
}

void os_ftdi_gpio_enable(uint8_t pin, bool direction)
{
	uint8_t pin_range, ud, p;

	IF_R(os_ftdi_gpio_check_pin(pin, &pin_range, &ud, &p),);

	if ((fdevs[pin_range].dirs[ud] & p) == (direction ? p : 0)) {
		/* no changes, just return */
		return;
	}

	if (direction) {
		fdevs[pin_range].dirs[ud] |= p;
	} else {
		fdevs[pin_range].dirs[ud] &= ~p;
	}

	if (fdevs[pin_range].mode == BITMODE_BITBANG) {
		ftdi_set_bitmode(fdevs[pin_range].ftdi, fdevs[pin_range].dirs[ud], BITMODE_BITBANG);
		ftdi_write_data(fdevs[pin_range].ftdi, &fdevs[pin_range].pins[ud], 1);
	} else if (fdevs[pin_range].mode == BITMODE_MPSSE) {
		uint8_t buf[3] = { ud ? SET_BITS_HIGH : SET_BITS_LOW, fdevs[pin_range].pins[ud], fdevs[pin_range].dirs[ud] };
		ftdi_write_data(fdevs[pin_range].ftdi, buf, 3);
	} else {
		ERROR_MSG("invalid mode for pin %d", pin);
	}
}

void os_ftdi_gpio_set(uint8_t pin, uint8_t state)
{
	uint8_t pin_range, ud, p;

	IF_R(os_ftdi_gpio_check_pin(pin, &pin_range, &ud, &p),);

	if ((fdevs[pin_range].pins[ud] & p) == (state ? p : 0)) {
		/* no changes, just return */
		return;
	}

	if (state) {
		fdevs[pin_range].pins[ud] |= p;
	} else {
		fdevs[pin_range].pins[ud] &= ~p;
	}

	if (fdevs[pin_range].mode == BITMODE_BITBANG) {
		ftdi_write_data(fdevs[pin_range].ftdi, &fdevs[pin_range].pins[ud], 1);
	} else if (fdevs[pin_range].mode == BITMODE_MPSSE) {
		uint8_t buf[3] = { ud ? SET_BITS_HIGH : SET_BITS_LOW, fdevs[pin_range].pins[ud], fdevs[pin_range].dirs[ud] };
		ftdi_write_data(fdevs[pin_range].ftdi, buf, 3);
	} else {
		ERROR_MSG("invalid mode for pin %d", pin);
	}
}

uint8_t os_ftdi_gpio_read(uint8_t pin)
{
	uint8_t pin_range, ud, p, pins;
	IF_R(os_ftdi_gpio_check_pin(pin, &pin_range, &ud, &p), 0);
	ftdi_read_pins(fdevs[pin_range].ftdi, &pins);
	return (pins & p) ? 1 : 0;
}

#endif

#endif
