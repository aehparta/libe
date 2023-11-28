/*
 * Scan i2c bus for devices.
 */

#include <libe/libe.h>
#ifdef TARGET_LINUX
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#endif
#include "../config.h"


#ifdef TARGET_ESP32
int app_main(int argc, char *argv[])
#else
int main(int argc, char *argv[])
#endif
{
	void *context = CFG_I2C_CONTEXT;
	struct i2c_master i2c;
	struct i2c_device dev1, dev2;
	float vref = 5.0;

	/* base init */
	os_init();
	log_init();

	/* check i2c device if using linux */
#ifdef USE_FTDI
	ERROR_IF_R(os_ftdi_use(OS_FTDI_GPIO_0_TO_63, CFG_FTDI_VID, CFG_FTDI_PID, CFG_FTDI_DESC, CFG_FTDI_SERIAL), 1, "unable to open ftdi device for gpio 0-63");
#endif
#ifdef TARGET_LINUX
	ERROR_IF_R(argc < 3, 1, "Give i2c device as first argument and reference (supply) voltage as second\nExample: ./i2c_mcp3221-x86.elf /dev/i2c-3 3.33");
	context = argv[1];
	vref = atof(argv[2]);
#endif

	/* open i2c */
	ERROR_IF_R(i2c_master_open(&i2c, context, CFG_I2C_FREQUENCY, CFG_I2C_SCL, CFG_I2C_SDA), 1, "unable to open i2c device");

	/* open mcp3221 */
	ERROR_IF_R(mcp3221_open(&dev1, &i2c, MCP3221_ADDR_A2), 1, "unable to find mcp3221-a2");
	ERROR_IF_R(mcp3221_open(&dev2, &i2c, MCP3221_ADDR_A5), 1, "unable to find mcp3221-a5");

	/* read mcp3221 */
	while (1) {
		int32_t v1, v2;
		v1 = mcp3221_rd(&dev1, 0);
		v2 = mcp3221_rd(&dev2, 0);
		ERROR_IF(v1 < 0, "failed reading mcp3221-a2");
		ERROR_IF(v2 < 0, "failed reading mcp3221-a5");

		INFO_MSG("A2: %.3fV (%04d), A5: %.3fV (%04d)", (float)v1 * vref / 4096.0, (int)v1, (float)v2 * vref / 4096.0, (int)v2);
		os_sleepf(0.2);
	}

	/* close mcp3221 */
	mcp3221_close(&dev1);
	mcp3221_close(&dev2);

	/* close i2c */
	i2c_master_close(&i2c);

	return 0;
}
