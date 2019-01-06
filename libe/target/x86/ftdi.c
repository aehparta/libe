/*
 * libe cross-platform library
 *
 * Authors:
 *  Antti Partanen <aehparta@iki.fi>
 */

#include <string.h>
#include <libe/debug.h>
#include "ftdi.h"


struct ftdi_context *ftdi_open(uint16_t vid, uint16_t pid, int interface, const char *description, const char *serial, int reset)
{
	int err, i, n;
	struct ftdi_context *ftdi;
	struct ftdi_device_list *list;
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
	/* find first device if vid or pid is zero */
	n = ftdi_usb_find_all(ftdi, &list, vid, pid);
	if (n < 1) {
		WARN_MSG("unable to find any matching device");
		ftdi_free(ftdi);
		return NULL;
	}
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
			ftdi_list_free(&list);
			ftdi_free(ftdi);
			return NULL;
		}
	}
	err = ftdi_usb_open_dev(ftdi, list->dev);
	ftdi_list_free(&list);
	if (err < 0) {
		ERROR_MSG("unable to open ftdi device: %s", ftdi_get_error_string(ftdi));
		ftdi_free(ftdi);
		return NULL;
	}
	/* reset chip */
	if (reset) {
		if (ftdi_usb_reset(ftdi)) {
			ERROR_MSG("failed to reset device: %s", ftdi_get_error_string(ftdi));
			ftdi_free(ftdi);
			return NULL;
		}
	}

	return ftdi;
}
