
System and time
########################################

Functions
****************************************

.. c:function:: int os_init(void)

	:return: 0 on success, -1 on errors

	Low-level system setup.
	Should be always called first at start of application main entry point.
	Generally will at least set all ports as inputs.

.. c:function:: void os_quit(void)

	Will de-initialize system. On many smaller targets this is just an empty define and does nothing.

.. c:function:: time_t os_timei(void)

	:return: seconds since some unspecified constant point in the past, usually system boot

.. c:function:: os_time_t os_timef(void)

	:return: seconds since some unspecified constant point in the past, usually system boot

.. c:function:: void os_sleepi(time_t t)

	:param time_t t: seconds to sleep

	Sleep given amount of seconds. On most targets this is stupid while-loop that simply blocks.

.. c:function:: void os_sleepf(os_time_t t)

	:param os_time_t t: seconds to sleep, floating point, with fractions of second included

	Sleep given amount of seconds. On most targets this is stupid while-loop that simply blocks.

Macros
****************************************

.. c:macro:: os_delay_us(delay)

	:param delay: microseconds to sleep

	This usually calls target specific macro, like
	`_delay_us() <https://www.nongnu.org/avr-libc/user-manual/group__util__delay.html>`_
	in AVR and ``__delay_us()`` in PIC.

.. c:macro:: os_delay_ms(delay)

	:param delay: milliseconds to sleep

	This usually calls target specific macro, like
	`_delay_ms() <https://www.nongnu.org/avr-libc/user-manual/group__util__delay.html>`_
	in AVR and ``__delay_ms()`` in PIC.

Types
****************************************

.. c:type:: os_time_t

Floating point time representation type:

* default: ``long double``
* AVR: ``double``
* PIC, 8-bit: ``double``
