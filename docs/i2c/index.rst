
I2C
########################################

Only a single I2C driver can be used for the time being.
If you use the bitbang driver then you cannot use target builtin I2C driver.
Nor can you use two different builtin drivers.

Configuration
****************************************

Enable I2C module
========================================

To enable I2C, flag ``USE_I2C`` must be defined globally.

In Makefile:

.. code-block:: makefile

	USE += I2C

From command line when compiling::
	
	make use=i2c

Master drivers
****************************************

X86, RPI and other linux based systems
========================================

Linux based systems use ``/dev/i2c`` interface:

* ``context`` for :c:func:`i2c_master_open` must be an absolute path, i.e. ``"/dev/i2c-13"``.
* rest of the parameters after ``context`` do not matter

Opening multiple masters is supported.

AVR
========================================

AVR's have only single I2C interface as far as I know so:

* ``frequency`` must be valid for :c:func:`i2c_master_open` or use 0 to fallback to default 100 kHz

	* closest available bitrate register value will then be calculated using :math:`(F\_CPU / frequency - 16) / 2`
	* negative results are rounded up to zero

* rest of the parameters do not matter, even ``master`` can be ``NULL``

Bitbang driver
========================================

Bitbang driver is available on all targets that are GPIO capable.

Static mode
----------------------------------------

Mostly for optimizations sake on small targets,
the bitbang driver requires clock and data pins to be defined in compile time as defines.

Because of this none of the parameters to :c:func:`i2c_master_open` matter.

To enable static bitbang driver with I2C, following conditions must be met:

* enable bitbang driver by defining ``USE_I2C_BITBANG`` globally with ``USE_I2C`` and ``USE_GPIO``
* define I2C clock and data pins using ``I2C_BITBANG_SCL`` and ``I2C_BITBANG_SDA``

In Makefile:

.. code-block:: makefile

	USE += GPIO I2C I2C_BITBANG
	# define target specific pins
	DEFINES_AVR += I2C_BITBANG_SCL=<PIN> I2C_BITBANG_SDA=<PIN>
	DEFINES_PIC8 += I2C_BITBANG_SCL=<PIN> I2C_BITBANG_SDA=<PIN>

From command line when compiling::
	
	make use="gpio i2c i2c_bitbang" defines="I2C_BITBANG_SCL=<PIN> I2C_BITBANG_SDA=<PIN>"

Dynamic mode
----------------------------------------

I2C bitbang driver can also be configured to use dynamic run-time values
for clock and data pins.

This mode should not be used on smaller targets.
It is slow and compiles ineffectively as a large piece of code example on targets AVR and PIC8.

On the other hand, you can open multiple masters using different GPIO pins in this mode.

To enable dynamic bitbang driver with I2C, following conditions must be met:

* enable bitbang driver by defining ``USE_I2C_BITBANG`` globally with ``USE_I2C`` and ``USE_GPIO``
* set bitbang driver into dynamic mode with ``USE_I2C_BITBANG_DYNAMIC``
* call :c:func:`i2c_master_open` with valid values for ``master``, ``frequency``, ``scl_pin`` and ``sda_pin``

In Makefile:

.. code-block:: makefile

	USE += GPIO I2C I2C_BITBANG USE_I2C_BITBANG_DYNAMIC

Frequency
----------------------------------------

As default the frequency will be set using a delay according to following rules:

* If dynamic mode is enabled using ``USE_I2C_BITBANG_DYNAMIC``, then delay is frequency given to :c:func:`i2c_master_open` and implemented as ``os_sleepf(1 / master->frequency)`` which is very inaccurate on smaller targets
* If target is AVR then ``_delay_loop1(F_CPU / 200000 / 3)`` is used to achieve an inaccurate approximate of 100 kHz
* On other targets ``os_delay_us(4)`` is used to achieve an inaccurate approximate of 100 kHz

Bitbang driver also supports custom delay. This can be defined using ``I2C_BITBANG_DELAY_FUNC`` macro.

In Makefile:

.. code-block:: makefile

	DEFINES += I2C_BITBANG_DELAY_FUNC=your_delay_func_or_macro()

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