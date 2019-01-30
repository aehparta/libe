/*
 * gpio hd44780 driver
 *
 * License: MIT
 * Authors: Antti Partanen <aehparta@iki.fi>
 */

#include <string.h>
#include <libe/os.h>
#include "hd44780.h"


int hd44780_open(struct hd44780_device *display, uint8_t d4, uint8_t d5, uint8_t d6, uint8_t d7, uint8_t en, uint8_t rw, uint8_t rs)
{
	display->d4 = d4;
	display->d5 = d5;
	display->d6 = d6;
	display->d7 = d7;

	os_gpio_output(display->d4);
	os_gpio_output(display->d5);
	os_gpio_output(display->d6);
	os_gpio_output(display->d7);

	display->en = en;
	display->rw = rw;
	display->rs = rs;

	os_gpio_output(display->en);
	os_gpio_output(display->rw);
	os_gpio_output(display->rs);

	hd44780_write_nibble(display, 0, 0x3);
	os_delay_ms(5);
	hd44780_write_nibble(display, 0, 0x3);
	os_delay_ms(5);
	hd44780_write_nibble(display, 0, 0x3);
	os_delay_ms(5);
	hd44780_write_nibble(display, 0, 0x2);
	os_delay_ms(5);

	/* entry mode: move cursor right */
	hd44780_cmd(display, 0x06);
	/* display on */
	hd44780_cmd(display, 0x0c);
	/* cursor/shift */
	hd44780_cmd(display, 0x10);
	/* clear display, set cursor home */
	hd44780_cmd(display, 0x01);

	return 0;
}

void hd44780_close(struct hd44780_device *display)
{
	os_gpio_input(display->d4);
	os_gpio_input(display->d5);
	os_gpio_input(display->d6);
	os_gpio_input(display->d7);
	os_gpio_input(display->en);
	os_gpio_input(display->rw);
	os_gpio_input(display->rs);
}

int hd44780_cmd(struct hd44780_device *display, uint8_t command)
{
	hd44780_write_nibble(display, 0, command >> 4);
	hd44780_write_nibble(display, 0, command);
	os_delay_ms(2);
	return 0;
}

int hd44780_write_nibble(struct hd44780_device *display, int rs, uint8_t data)
{
	os_gpio_set(display->d4, data & 0x1);
	os_gpio_set(display->d5, data & 0x2);
	os_gpio_set(display->d6, data & 0x4);
	os_gpio_set(display->d7, data & 0x8);
	
	os_gpio_set(display->rw, 0);
	os_gpio_set(display->rs, rs);
	
	os_gpio_set(display->en, 1);

	/* sleep here but how much? */
	os_delay_us(1);

	os_gpio_set(display->en, 0);

	return 0;
}

int hd44780_write_data(struct hd44780_device *display, uint8_t data)
{
	hd44780_write_nibble(display, 1, data >> 4);
	hd44780_write_nibble(display, 1, data);
	os_delay_us(37);
	return 0;
}

int hd44780_write_char(struct hd44780_device *display, char ch)
{
	return hd44780_write_data(display, (uint8_t)ch);
}

int hd44780_write_str(struct hd44780_device *display, const char *str)
{
	for (int i = 0; i < strlen(str); i++) {
		hd44780_write_char(display, str[i]);
	}
	return 0;
}

int hd44780_goto_xy(struct hd44780_device *display, int x, int y)
{
	if (x < 0 || x > 39 || y < 0 || y > 3) {
		return -1;
	}
	hd44780_cmd(display, 0x80 | (y * 40 + x));
	return 0;
}