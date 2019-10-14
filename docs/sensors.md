
# Sensors

## Temperature and humidity

Temperature and humidity sensors are combined together under same API.
Even sensors that provide only one use the same API.

## Functions

Capitalized words:

* `SENSOR`: sensor name being used
* `DEV`
    * `struct i2c_device *` in case of sensor connected to I2C bus
    * `struct spi_device *` in case of sensor connected to SPI bus
* `MASTER`
    * `struct i2c_master *` in case of sensor connected to I2C bus
    * `struct spi_master *` in case of sensor connected to SPI bus

### `int8_t SENSOR_open(DEV, MASTER, uint8_t ref, int8_t res, int8_t h_res)`
* ⇒ `DEV`
* ⇒ `MASTER`
* ⇒ `uint8_t ref`
    * I2C: Optional address if device supports more than single address
        * Set to `0` to use default address
    * SPI: Slave select pin number
* ⇒ `uint8_t res` General resolution or temperature resolution if device supports either
    * Set to `0` to use highest possible resolution
    * If `-1`, then do not set at all, use whatever the chip is using currently
* ⇒ `uint8_t h_res` Humidity resolution if device supports setting it
    * Set to `0` to use highest possible resolution
    * If `-1`, then do not set at all, use whatever the chip is using currently
* ⤶ `int8_t`
    * `0` on success
    * `-1` if device not found
    * `-2` on other errors

### `int8_t SENSOR_heater(DEV, bool on)`
* ⇒ `DEV`
* ⇒ `bool on` Heater on or off
* ⤶ `int8_t`
    * `0` on success
    * `-1` if device has no heater
    * `-2` on other errors

Enable internal heater if device has such.
Usually heater will only apply heat when a measurement is requested.
So in order to actually heat the device, sensor should be read continuously.

### `int8_t SENSOR_read(DEV, float *t, float *h)`
* ⇒ `DEV`
* ⇒ `float *t` Save temperature here if not NULL
    * `-274.0`: If device does not support temperature reading 
    * `-275.0`: On other errors
* ⇒ `float *h` Save humidity here if not NULL
    * `-1.0`: If device does not support humidity reading 
    * `-2.0`: On other errors

Read device temperature and humidity values.

