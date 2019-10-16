
GPIO
########################################

Generic interface for GPIO which requires only pin number instead of port&pin-combo used in many smaller devices like AVR and PIC.
This is achieved by mapping PORTB in AVR to pin numbers 8-15, PORTC to 16-23 and so on.
Same for PIC, MSP430 and others that do this kind of thing instead of having continuous GPIO pin numbers.

For convenience ``GPIOxy`` constants have been defined where ``x`` is the letter of an 8-bit port and ``y`` is the bit number.
These constants cover ports from A to H.

GPIO implemented this way is more and less complex thing.
On most targets, like AVR, GPIO functions are inline and will often optimize as well as directly written native code.

Configuration
****************************************

Enable GPIO module
========================================

To enable GPIO, flag ``USE_GPIO`` must be defined globally.

In Makefile:

.. code-block:: makefile

	USE += GPIO

From command line when compiling::
	
	make use=gpio

Functions
****************************************

.. c:function:: void gpio_enable(uint8_t pin, bool direction)

	:param uint8_t pin: pin to be enabled
	:param bool direction:

		* 0 is input
		* 1 is output

	Enable specific pin using it as an input or output.

.. c:function:: void gpio_set(uint8_t pin, uint8_t state)

	:param uint8_t pin: pin to be set
	:param uint8_t state:

		* 0 is low
		* any other value is high

.. c:function:: uint8_t gpio_read(uint8_t pin)

	:param uint8_t pin: pin to be read
	:return: 0 if pin is low, non-zero if high

Macros
****************************************

.. c:function:: gpio_input(pin)

	Calls ``gpio_enable(pin, 0)``.

.. c:function:: gpio_output(pin)

	Calls ``gpio_enable(pin, 1)``.

.. c:function:: gpio_low(pin)

	Calls ``gpio_set(pin, 0)``.

.. c:function:: gpio_high(pin)

	Calls ``gpio_set(pin, 1)``.

Defines
****************************************

.. cpp:enumerator:: GPIOA0 = 0
.. cpp:enumerator:: GPIOA1 = 1
.. cpp:enumerator:: GPIOA2 = 2
.. cpp:enumerator:: GPIOA3 = 3
.. cpp:enumerator:: GPIOA4 = 4
.. cpp:enumerator:: GPIOA5 = 5
.. cpp:enumerator:: GPIOA6 = 6
.. cpp:enumerator:: GPIOA7 = 7

.. cpp:enumerator:: GPIOB0 = 8
.. cpp:enumerator:: GPIOB1 = 9
.. cpp:enumerator:: GPIOB2 = 10
.. cpp:enumerator:: GPIOB3 = 11
.. cpp:enumerator:: GPIOB4 = 12
.. cpp:enumerator:: GPIOB5 = 13
.. cpp:enumerator:: GPIOB6 = 14
.. cpp:enumerator:: GPIOB7 = 15

.. cpp:enumerator:: GPIOC0 = 16
.. cpp:enumerator:: GPIOC1 = 17
.. cpp:enumerator:: GPIOC2 = 18
.. cpp:enumerator:: GPIOC3 = 19
.. cpp:enumerator:: GPIOC4 = 20
.. cpp:enumerator:: GPIOC5 = 21
.. cpp:enumerator:: GPIOC6 = 22
.. cpp:enumerator:: GPIOC7 = 23

.. cpp:enumerator:: GPIOD0 = 24
.. cpp:enumerator:: GPIOD1 = 25
.. cpp:enumerator:: GPIOD2 = 26
.. cpp:enumerator:: GPIOD3 = 27
.. cpp:enumerator:: GPIOD4 = 28
.. cpp:enumerator:: GPIOD5 = 29
.. cpp:enumerator:: GPIOD6 = 30
.. cpp:enumerator:: GPIOD7 = 31

.. cpp:enumerator:: GPIOE0 = 32
.. cpp:enumerator:: GPIOE1 = 33
.. cpp:enumerator:: GPIOE2 = 34
.. cpp:enumerator:: GPIOE3 = 35
.. cpp:enumerator:: GPIOE4 = 36
.. cpp:enumerator:: GPIOE5 = 37
.. cpp:enumerator:: GPIOE6 = 38
.. cpp:enumerator:: GPIOE7 = 39

.. cpp:enumerator:: GPIOF0 = 40
.. cpp:enumerator:: GPIOF1 = 41
.. cpp:enumerator:: GPIOF2 = 42
.. cpp:enumerator:: GPIOF3 = 43
.. cpp:enumerator:: GPIOF4 = 44
.. cpp:enumerator:: GPIOF5 = 45
.. cpp:enumerator:: GPIOF6 = 46
.. cpp:enumerator:: GPIOF7 = 47

.. cpp:enumerator:: GPIOG0 = 48
.. cpp:enumerator:: GPIOG1 = 49
.. cpp:enumerator:: GPIOG2 = 50
.. cpp:enumerator:: GPIOG3 = 51
.. cpp:enumerator:: GPIOG4 = 52
.. cpp:enumerator:: GPIOG5 = 53
.. cpp:enumerator:: GPIOG6 = 54
.. cpp:enumerator:: GPIOG7 = 55

.. cpp:enumerator:: GPIOH0 = 56
.. cpp:enumerator:: GPIOH1 = 57
.. cpp:enumerator:: GPIOH2 = 58
.. cpp:enumerator:: GPIOH3 = 59
.. cpp:enumerator:: GPIOH4 = 60
.. cpp:enumerator:: GPIOH5 = 61
.. cpp:enumerator:: GPIOH6 = 62
.. cpp:enumerator:: GPIOH7 = 63
