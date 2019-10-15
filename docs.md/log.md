
# Logging

## Functions

### `int log_init(void)`
* ⤶ `int` It is highly unlikely that this function returns anything else than zero which means ok

Initialize logging. Should be called right after `os_init()`.
Will calculate platform specific UART settings according to baud rate.

First UART is used as default on platforms having an UART.
This can be overridden using platform specific register defines
documented below.


### `void log_quit(void)`

Free logging related resources.

## Macros

All macros that end `...` use `printf()` style formatting.

### `DEBUG_MSG(message, ...)`
### `INFO_MSG(message, ...)`
### `NOTICE_MSG(message, ...)`
### `WARN_MSG(message, ...)`
### `ERROR_MSG(message, ...)`
### `CRIT_MSG(message, ...)`

Print given message.

### `DEBUG_IF(condition, message, ...)`
### `INFO_IF(condition, message, ...)`
### `NOTICE_IF(condition, message, ...)`
### `WARN_IF(condition, message, ...)`
### `ERROR_IF(condition, message, ...)`
### `CRIT_IF(condition, message, ...)`

Print given message only if condition is true.

### `DEBUG_IF_R(condition, ret_value, message, ...)`
### `INFO_IF_R(condition, ret_value, message, ...)`
### `NOTICE_IF_R(condition, ret_value, message, ...)`
### `WARN_IF_R(condition, ret_value, message, ...)`
### `ERROR_IF_R(condition, ret_value, message, ...)`
### `CRIT_IF_R(condition, ret_value, message, ...)`

If condition is true, print given message and return from current
function with `ret_value`.

Example:
```c
int ret_int(int condition)
{
    DEBUG_IF_R(condition, -1, "debug error and return -1");
    return 0;
}

void ret_void(int condition)
{
    DEBUG_IF_R(condition,, "debug error and return with no value");
}
```


## Defines

To override these settings, define each setting you want to override at
global level, i.e. in Makefile.

Makefile example:
```makefile
ifeq ($(TARGET), avr)
    CFLAGS += -DLOG_BAUD=38400
else ifeq ($(TARGET), e8)
    # baud rate left as default 9600 here
    CFLAGS += -DPIC8_LOG_TX_PPS=RA0PPS
else
    CFLAGS += -DLOG_BAUD=115200
endif
```

### `LOG_BAUD = 9600`

Default log baud rate for devices using UART.

### Target specific

#### AVR

##### Defines

Default value is either the first or the second separated by `|`
depending on if the AVR has multiple UARTs.

###### `AVR_LOG_UCSRA = UCSRA|UCSR0A`
###### `AVR_LOG_UCSRB = UCSRB|UCSR0B`
###### `AVR_LOG_UCSRC = UCSRC|UCSR0C`
###### `AVR_LOG_UDRE = UDRE|UDRE0`
###### `AVR_LOG_UDR = UDR|UDR0`
###### `AVR_LOG_UBRR = UBRRL|UBRR0`
###### `AVR_LOG_TXEN = TXEN|TXEN0`
###### `AVR_LOG_RXEN = RXEN|RXEN0`
###### `AVR_LOG_UCSZ = UCSZ0|UCSZ00`

#### ESP32

ESP32
[internal logging system](https://docs.espressif.com/projects/esp-idf/en/latest/api-reference/system/log.html)
is used.

#### PIC, 8-bit

##### Defines

###### `PIC8_LOG_BAUD = LOG_BAUD`

PIC specific baud rate. Default used if not defined.

###### `PIC8_LOG_TX_PPS = RC6PPS`

If device supports peripheral pin select, then `PIC8_LOG_TX_PPS` is used to determine the pin
used for transmitting.
This must be set to `RxyPPS` where `x` is port and `y` is pin.

###### `PIC8_LOG_BAUDCON = BAUDCON1`

Only used if selected MCU has this register.

###### `PIC8_LOG_TXIF = TXIF`

###### `PIC8_LOG_TXREG = TXREG`

###### `PIC8_LOG_TXSTA = TXSTA`

###### `PIC8_LOG_RCSTA = RCSTA`

###### `PIC8_LOG_SPBRG = SPBRG`


