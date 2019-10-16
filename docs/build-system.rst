
Build system
########################################

To be written.

Configuration
****************************************

.. envvar:: TARGET = <TARGET>

	Only required configuration value.

.. envvar:: MCU_<TARGET> = <MCU>

	Target specific MCU. Could be ATMEGA8 for AVR targets or 16F87 for 8-bit PIC targets for example.

.. envvar:: F_CPU_<TARGET> = <FREQUENCY>

	Target specific CPU clock speed needed by some platforms like AVR and 8-bit PIC.

.. envvar:: F_CPU_<MCU> = <FREQUENCY>

	Same as ``F_CPU_<TARGET>`` but ``MCU`` specific.

.. envvar:: USE += ...
.. envvar:: USE_<TARGET> += ...
.. envvar:: USE_<MCU> += ...

.. envvar:: DEFINES += ...

.. envvar:: DEFINES_<TARGET> += ...

	Same as ``DEFINES`` but ``TARGET`` specific.

.. envvar:: DEFINES_<MCU> += ...

	Same as ``DEFINES`` but ``MCU`` specific.


