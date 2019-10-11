
# System related (low level)

## Types

### os_time_t

Floating point time representation type. `long double` as default.

##### avr

This is `double` in avr.

##### pic8

This is `double` in pic8.

## Functions

### int os_init(void)
* ⤶ `int` It is highly unlikely that this function returns anything else than zero which means ok

Low level system setup. Should be always called first at start of application main entry point.
Generally will at least set all ports as inputs.

##### pic8

Will set all ports as digital using `ANSELx = 0x00` if analog port selection registers are available.

### void os_quit(void)

Will de-initialize system. On many smaller platforms this is just an empty define and does nothing.

### time_t os_timei(void)
* ⤶ `time_t` Current time as integer number

### os_time_t os_timef(void)
* ⤶ `time_t` Current time as floating point number with as much precision as the current platform is able to produce

### void os_sleepi(time_t t)
* ⇒ `time_t t` Sleep given amount of seconds

### void os_sleepf(os_time_t t)
* ⇒ `os_time_t t` Sleep given amount of seconds and fractions of second

### void os_delay_us(t_us)
* ⇒ `t_us` Sleep given amount of microseconds

This usually calls platform specific macro, like
[`_delay_us()`](https://www.nongnu.org/avr-libc/user-manual/group__util__delay.html)
in avr and `__delay_us()` in pic8.

### void os_delay_ms(t_us)
* ⇒ `t_us` Sleep given amount of milliseconds

This usually calls platform specific macro, like
[`_delay_ms()`](https://www.nongnu.org/avr-libc/user-manual/group__util__delay.html)
in avr and `__delay_ms()` in pic8.

