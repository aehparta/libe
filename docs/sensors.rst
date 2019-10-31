
Sensors
########################################

.. _sensors reference:

Reference
****************************************

Reference for following capitalized words in this page:

* ``SENSOR``
	* sensor name being used, *always lower case*
* ``DEV``
	* ``struct i2c_device *`` in case of sensor connected to I2C bus
	* ``struct spi_device *`` in case of sensor connected to SPI bus
	* other device structs depending on bus and so on
* ``MASTER``
	* ``struct i2c_master *`` in case of sensor connected to I2C bus
	* ``struct spi_master *`` in case of sensor connected to SPI bus
	* other master structs depending on bus and so on

Generic Functions
****************************************

.. c:function:: int8_t SENSOR_open(DEV, MASTER, int32_t ref, int32_t cfg)

	:param DEV: See :ref:`sensors reference`
	:param MASTER: See :ref:`sensors reference`
	:param int32_t ref: Reference information to driver

		* I2C: Optional address if device supports more than one, set to zero to use default
		* SPI: Slave select pin

	:param int32_t cfg: Driver specific configuration option

		* T&H sensors: Device specific measurement resolution or similar setting
		* 0: use default
		* -1: skip configuration and use what ever the chip is using currently

	:return:

		* 0 on success
		* -1 if device not found
		* -2 on other errors, e.g. configuration failed

.. c:function:: void SENSOR_close(DEV)

	:param DEV: See :ref:`sensors reference`

ADC
****************************************

Analog to digital converters.

Functions
========================================

.. c:function:: int8_t SENSOR_vref(DEV, float vref)

	:param DEV: See :ref:`sensors reference`
	:param float vref: Reference voltage to use when reading
	:return:

		* 0 on success
		* -1 if device not found
		* -2 on other errors

	Set reference voltage used by ADC. Default is 1.0 volts.

	See :c:func:`SENSOR_read`.

.. c:function:: int8_t SENSOR_zero(DEV, float zero)

	:param DEV: See :ref:`sensors reference`
	:param float zero: Zero offset to apply when reading
	:return: 0

	Set zero offset. Default is zero which means no effect.

	See :c:func:`SENSOR_read`.

.. c:function:: int8_t SENSOR_multiplier(DEV, float multiplier)

	:param DEV: See :ref:`sensors reference`
	:param float multiplier: Multiplier to apply when reading
	:return: 0

	Default is 1.0.

	See :c:func:`SENSOR_read`.

.. c:function:: int8_t SENSOR_read(DEV, int32_t *raw, float *value)

	:param DEV: See :ref:`sensors reference`
	:param int32_t \*raw: Save raw reading here if not NULL.

		Note that differential ADC readings can be negative. ``*raw`` is a 32 bit signed
		integer which means that example a 16 bit differential ADC reading
		is returned between -32768..32767 as a 32 bit signed integer,
		not 16 bits of raw data saved in 32 bit signed integer.

	:param float \*value: Save converted value here after applying formula shown in description. Can be NULL.
	:return:

		* 0 on success
		* -1 if device not found
		* -2 on other errors

	Read ADC value. Formula applied to raw value for converting it to ``*value``:

	:math:`(RAW\_VALUE / MAX\_VALUE * vref - zero) * multiplier`


ADC Drivers
========================================

None at the moment. Only documentation written to be used when writing actual drivers.

adc *(internal)*
----------------------------------------

Internal device ADC driver, if exists.


Temperature and Humidity
****************************************

Temperature and humidity sensors are combined together under same API.

.. note:: Even sensors that provide only one use the same API.

Functions
========================================

.. c:function:: int8_t SENSOR_heater(DEV, bool on)

	:param DEV: See :ref:`sensors reference`
	:param bool on: Heater on or off
	:return:

		* 0 on success
		* -1 if device not found
		* -2 on other errors
		* -3 if device has no heater

	Enable internal heater if device has such.
	Usually heater will only apply heat when a measurement is requested
	so in order to actually heat the device sensor should be read continuously.

.. c:function:: int8_t SENSOR_read(DEV, float *t, float *h)

	:param DEV: See :ref:`sensors reference`
	:param float \*t: Save temperature here if not NULL

	    * Set to -274 if device does not support temperature reading 
	    * Set to -275.0 on other errors, e.g. reading failed

	:param float \*h: Save humidity here if not NULL

	    * Set to -1 if device does not support humidity reading 
	    * Set to -2 on other errors, e.g. reading failed

	:return:

		* 0 on success
		* -1 if device not found
		* -2 on other errors

	Read device temperature and humidity values.

T&H Drivers
========================================

hdc1080
----------------------------------------

:c:func:`SENSOR_open`

	* ref is not used
	* cfg is resolution
			
		* 14 bits (default)
		* 11 bits

:c:func:`SENSOR_heater`

	* Has a heater

sht21
----------------------------------------

:c:func:`SENSOR_open`

	* ref is not used
	* cfg is resolution
		
		* 14: T = 14 bits, RH = 12 bits (default)
		* 13: T = 13 bits, RH = 10 bits
		* 12: T = 12 bits, RH = 8 bits
		* 11: T = 11 bits, RH = 11 bits

:c:func:`SENSOR_heater`

	* Has a heater

sht31
----------------------------------------

:c:func:`SENSOR_open`

	* ref is I2C address

		* 0x44 (default)
		* 0x45

	* cfg is repeatability

		.. cpp:enumerator:: SHT31_REPEATABILITY_HIGH = 0x00
		.. cpp:enumerator:: SHT31_REPEATABILITY_MEDIUM = 0x0b
		.. cpp:enumerator:: SHT31_REPEATABILITY_LOW = 0x16

:c:func:`SENSOR_heater`

	* Has a heater

