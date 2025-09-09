/*
 * Display driver: SSD1681 (e-paper)
 */

#ifndef _DISPLAY_SSD1681_H_
#define _DISPLAY_SSD1681_H_

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>

/* SSD1681 commands (subset) */
#define SSD1681_DRIVER_OUTPUT_CONTROL		0x01
#define SSD1681_GATE_DRIVING_VOLTAGE		0x03
#define SSD1681_SOURCE_DRIVING_VOLTAGE		0x04
#define SSD1681_BOOSTER_SOFT_START		0x0C
#define SSD1681_DEEP_SLEEP_MODE		0x10
#define SSD1681_DATA_ENTRY_MODE_SETTING	0x11
#define SSD1681_SW_RESET			0x12
#define SSD1681_TEMPERATURE_SENSOR_CONTROL	0x1A
#define SSD1681_MASTER_ACTIVATION		0x20
#define SSD1681_DISPLAY_UPDATE_CONTROL_1	0x21
#define SSD1681_DISPLAY_UPDATE_CONTROL_2	0x22
#define SSD1681_WRITE_RAM			0x24
#define SSD1681_WRITE_VCOM_REGISTER		0x2C
#define SSD1681_WRITE_LUT_REGISTER		0x32
#define SSD1681_SET_RAM_X_ADDRESS_START_END	0x44
#define SSD1681_SET_RAM_Y_ADDRESS_START_END	0x45
#define SSD1681_SET_RAM_X_ADDRESS_COUNTER	0x4E
#define SSD1681_SET_RAM_Y_ADDRESS_COUNTER	0x4F

/* Data entry mode bits */
#define SSD1681_DATA_ENTRY_X_INC		0x01
#define SSD1681_DATA_ENTRY_Y_INC		0x02

/*
 * SPI open helper
 *
 * dc: data/command pin
 * reset: reset pin (active low)
 * busy: busy pin (high = busy on many panels). If not available, pass 0xFF.
 */
int8_t ssd1681_open(struct display *display, struct spi_master *master, uint8_t ce, uint8_t dc, uint8_t reset, uint8_t busy, int16_t w, int16_t h);
int8_t ssd1681_opt(struct display *display, uint8_t opt, void *value);
void ssd1681_pixel(struct display *display, int16_t x, int16_t y, uint32_t color);
void ssd1681_update(struct display *display);

#ifdef __cplusplus
}
#endif

#endif /* _DISPLAY_SSD1681_H_ */


