
I2C
########################################


Functions
****************************************

.. c:function:: int8_t i2c_master_open(struct i2c_master *master, void *context, uint32_t frequency, uint8_t scl_pin, uint8_t sda_pin)

	:param master: pointer to preallocated memory
	:type master: struct i2c_master *
	:param context: driver specific context
	:type context: void *
	:param uint32_t frequency: I2C frequency, used only if driver supports setting frequency
	:param uint8_t scl_pin: I2C clock GPIO, used only if driver supports changing scl
	:param uint8_t sda_pin: I2C data GPIO, used only if driver supports changing sda
	:return: 0 on success, negative value on errors

	Open I2C driver in master mode.

.. c:function:: void i2c_master_close(struct i2c_master *master)

	:param master: previously opened master
	:type master: struct i2c_master *

	Close I2C driver.

.. c:function:: int8_t i2c_open(struct i2c_device *dev, struct i2c_master *master, uint8_t address)

	:param dev: pointer to preallocated memory
	:type dev: struct i2c_device *
	:param master: previously opened master
	:type master: struct i2c_master *
	:param uint8_t address: device address
	:return:

		* 0 on success
		* -1 if device did not respond

	Try to open I2C device connected to master.

.. c:function:: void i2c_close(struct i2c_device *dev)

	:param dev: previously opened i2c device
	:type dev: struct i2c_device *

	Close I2C device.

.. c:function:: int8_t i2c_read(struct i2c_device *dev, void *data, int8_t size)

	:param dev: previously opened i2c device
	:type dev: struct i2c_device *
	:param data: pointer to a buffer where to save read data
	:type data: void *
	:param int8_t size: count of bytes to read
	:return:

		* ``size`` on success
		* less than ``size`` if only some of the data was read
		* 0 if ack was received after address but no data was read succesfully
		* -1 if device did not respond

	Read data from I2C device.

.. c:function:: int8_t i2c_write(struct i2c_device *dev, void *data, int8_t size)

	:param dev: previously opened i2c device
	:type dev: struct i2c_device *
	:param data: pointer to data to be written
	:type data: void *
	:param int8_t size: count of bytes to write
	:return:

		* ``size`` on success
		* less than ``size`` if only some of the data was written
		* 0 if ack was received after address but no data was written succesfully
		* -1 if device did not respond

	Write data to I2C device.
	Setting ``data = NULL`` and ``size = 0`` can be used to detect if there is a device connected to
	the address used by device. This is done automatically when calling :c:func:`i2c_open`.

.. c:function:: int8_t i2c_write_byte(struct i2c_device *dev, uint8_t value)

	:param dev: previously opened i2c device
	:type dev: struct i2c_device *
	:param uint8_t value: value to write
	:return: 1 on success

	Write single byte to I2C device.

.. c:function:: int8_t i2c_write_reg_byte(struct i2c_device *dev, uint8_t reg, uint8_t value)

	:param dev: previously opened i2c device
	:type dev: struct i2c_device *
	:param uint8_t reg: register to write
	:param uint8_t value: value to write
	:return: 2 on success

	Write register value to I2C device.
	This is same as writing two bytes to I2C device, in this case ``reg`` followed by ``value``.

.. c:function::  uint8_t i2c_read_reg_byte(struct i2c_device *dev, uint8_t reg)

	:param dev: previously opened i2c device
	:type dev: struct i2c_device *
	:param uint8_t reg: register to read
	:return:

		* register value on success
		* 255 on errors, which can be also a valid value read from a register

	Read register from I2C device.
	This is same as writing a single byte to device and then reading a single byte from it.