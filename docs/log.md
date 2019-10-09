
# Logging

## Functions

### `int log_init(void)`

Initialize logging. Should be called right after `os_init()`.

### `void log_quit(void)`

Free logging related resources.

## Defines

### `LOG_BAUD = 9600`

Log baud rate for devices using UART.

## Target specific

### pic8

#### Defines

##### `PIC8_LOG_BAUD`

8-bit PIC specific baud rate. Default used if not defined.

##### `PIC8_LOG_TX_PPS = RC6PPS`

If device supports peripheral pin select, then `PIC8_LOG_TX_PPS` is used to determine the pin
used for transmitting.
This must be set to `RxyPPS` where `x` is port and `y` is pin.

