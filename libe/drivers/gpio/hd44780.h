/*
 * gpio hd44780 driver
 *
 * Not the fastest implementation, since every gpio pin state is
 * set/read separately even if the display is connected to
 * same "port". But with hd44780 I don't see a real problem
 * here so far.
 * 
 * License: MIT
 * Authors: Antti Partanen <aehparta@iki.fi>
 */

#ifndef _DRIVERS_GPIO_HD44780_H_
#define _DRIVERS_GPIO_HD44780_H_

#include <stdint.h>

struct hd44780_device {
	uint8_t d4;
	uint8_t d5;
	uint8_t d6;
	uint8_t d7;
	uint8_t en;
	uint8_t rw;
	uint8_t rs;
	// uint8_t line_width;
	// uint8_t line_chars_written;
};

int hd44780_open(struct hd44780_device *display, uint8_t d4, uint8_t d5, uint8_t d6, uint8_t d7, uint8_t en, uint8_t rw, uint8_t rs);
void hd44780_close(struct hd44780_device *display);

int hd44780_cmd(struct hd44780_device *display, uint8_t command);
int hd44780_write_nibble(struct hd44780_device *display, int rs, uint8_t data);

int hd44780_write_data(struct hd44780_device *display, uint8_t data);
int hd44780_write_char(struct hd44780_device *display, char ch);
int hd44780_write_str(struct hd44780_device *display, const char *str);
int hd44780_goto_xy(struct hd44780_device *display, int x, int y);

// struct hd44780_context * ftdi_hd44780_init_simple(struct ftdi_bitbang_context *bb);
// void ftdi_hd44780_free(struct ftdi_hd44780_context *dev);

// int ftdi_hd44780_cmd(struct ftdi_hd44780_context *dev, uint8_t command);
// int ftdi_hd44780_write_data(struct ftdi_hd44780_context *dev, uint8_t data);
// int ftdi_hd44780_write_char(struct ftdi_hd44780_context *dev, char ch);
// int ftdi_hd44780_write_str(struct ftdi_hd44780_context *dev, char *str);

// int ftdi_hd44780_goto_xy(struct ftdi_hd44780_context *dev, int x, int y);
// int ftdi_hd44780_set_line_width(struct ftdi_hd44780_context *dev, int line_width);


#endif /* _DRIVERS_GPIO_HD44780_H_ */
