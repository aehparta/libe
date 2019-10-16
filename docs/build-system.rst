
Build system
########################################

Command line options for make
****************************************

All option names must be lower case for not to override internals.

.. describe:: target=<target>

.. describe:: mcu=<mcu>

.. describe:: f_cpu=<frequency>

.. describe:: use="<use1> <use2>"

.. describe:: defines="<define1> <define2>"

.. describe:: cflags="<flag1> <flag2>"
	
	Directly add these to ``CFLAGS``.

.. describe:: ldflags="<flag1> <flag2>"

	Directly add these to ``LDFLAGS``.

Makefile options
****************************************

.. describe:: TARGET = <TARGET>

	In Makefile you **MUST** use upper case for target value. In command line you can also use lower case.

	Define default target in Makefile:

	.. code-block:: makefile

		TARGET = X86

	Then compile using another target::

		make target=avr


.. describe:: MCU_<TARGET> = <MCU>

	Target specific default MCU. Could be ATmega8 for AVR targets or 16F87 for 8-bit PIC targets.

	Define default MCU in Makefile:

	.. code-block:: makefile

		TARGET = X86
		MCU_AVR = atmega328p
		MCU_PIC8 = 16f84a

	Then compile using another MCU::

		make target=avr mcu=atmega8
		make target=pic8 mcu=16f87

.. describe:: F_CPU_<TARGET> = <FREQUENCY>

	Target specific CPU clock speed needed by some platforms like AVR and 8-bit PIC.

	Define in Makefile:

	.. code-block:: makefile

		F_CPU_AVR = 1000000

	Then override from command line::

		make f_cpu=8000000

.. describe:: F_CPU_<MCU> = <FREQUENCY>

	Same as ``F_CPU_<TARGET>`` but ``MCU`` specific.

.. describe:: USE += ...

	Add used components. Normally you want some since almost nothing is included as default.

	Makefile with I2C and logging enabled:

	.. code-block:: makefile

		USE += I2C LOG

	Then compile by adding GPIO and using I2C in bitbang mode (which needs GPIO)::

		make use="gpio i2c_bitbang"

.. describe:: USE_<TARGET> += ...

	Same as ``USE`` but ``TARGET`` specific.

.. describe:: USE_<MCU> += ...

	Same as ``USE`` but ``MCU`` specific.

.. describe:: DEFINES += ...

	Add defines. This basically is a shorthand for ``CFLAGS += -D<DEFINE>``.

	Usage in Makefile:

	.. code-block:: makefile

		DEFINES += MY_DEFINE OTHER_DEFINE
		DEFINES += MY_PI=3.1415

	From command line::

		make defines="CMD_DEFINE1 CMD_DEFINE2=123"

.. describe:: DEFINES_<TARGET> += ...

	Same as ``DEFINES`` but ``TARGET`` specific.

.. describe:: DEFINES_<MCU> += ...

	Same as ``DEFINES`` but ``MCU`` specific.

.. describe:: CFLAGS += ...

	Compiler flags.

.. describe:: CFLAGS_<TARGET> += ...

	Compiler flags.

.. describe:: CFLAGS_<MCU> += ...

	Compiler flags.

.. describe:: LDFLAGS += ...

	Linker flags.

.. describe:: LDFLAGS_<TARGET> += ...

	Linker flags.

.. describe:: LDFLAGS_<MCU> += ...

	Linker flags.
