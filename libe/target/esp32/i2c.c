/*
 * I2C ESP32 drivers
 *
 * Authors:
 *  Antti Partanen <aehparta@iki.fi>
 */

#if defined(USE_I2C) && !defined(USE_I2C_BITBANG)

#include <libe/libe.h>


int8_t i2c_master_open(struct i2c_master *master, void *context, uint32_t frequency, uint8_t scl, uint8_t sda)
{
	/* default to 100 kHz */
	if (frequency < 1) {
		frequency = 100000;
	}

	master->port = (intptr_t)context;

	i2c_config_t conf;
	conf.mode = I2C_MODE_MASTER;
	conf.sda_io_num = sda;
	conf.sda_pullup_en = GPIO_PULLUP_DISABLE;
	conf.scl_io_num = scl;
	conf.scl_pullup_en = GPIO_PULLUP_DISABLE;
	conf.master.clk_speed = frequency;
	i2c_param_config(master->port, &conf);

	return i2c_driver_install(master->port, conf.mode, 0, 0, 0);
}

void i2c_master_close(struct i2c_master *master)
{
	i2c_driver_delete(master->port);
}

int8_t i2c_open(struct i2c_device *dev, struct i2c_master *master, uint8_t address)
{
	dev->master = master;
	dev->address = address << 1;
	return i2c_write(dev, NULL, 0);
}

void i2c_close(struct i2c_device *dev)
{
}

int8_t i2c_read(struct i2c_device *dev, void *data, uint8_t size)
{
	esp_err_t err;
	i2c_cmd_handle_t cmd = i2c_cmd_link_create();
	i2c_master_start(cmd);
	i2c_master_write_byte(cmd, dev->address | 1, true);
	if (size > 0) {
		if (size > 1) {
			i2c_master_read(cmd, data, size - 1, I2C_MASTER_ACK);
		}
		i2c_master_read_byte(cmd, data + size - 1, I2C_MASTER_NACK);
	}
	i2c_master_stop(cmd);
	err = i2c_master_cmd_begin(dev->master->port, cmd, 1000 / portTICK_RATE_MS);
	i2c_cmd_link_delete(cmd);
	return err != ESP_OK ? -1 : 0;
}

int8_t i2c_write(struct i2c_device *dev, void *data, uint8_t size)
{
	esp_err_t err;
	i2c_cmd_handle_t cmd = i2c_cmd_link_create();
	i2c_master_start(cmd);
	i2c_master_write_byte(cmd, dev->address, true);
	if (size > 0) {
		i2c_master_write(cmd, data, size, true);
	}
	i2c_master_stop(cmd);
	err = i2c_master_cmd_begin(dev->master->port, cmd, 1000 / portTICK_RATE_MS);
	i2c_cmd_link_delete(cmd);
	return err != ESP_OK ? -1 : 0;
}

#endif
