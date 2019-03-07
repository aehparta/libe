/*
 * Scan i2c bus for devices.
 */

#include <libe/os.h>
#include <libe/log.h>
#include <libe/i2c.h>
#include <libe/drivers/i2c/mcp3221.h>
#ifdef TARGET_LINUX
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#endif


#ifdef TARGET_ESP32
int app_main(int argc, char *argv[])
#else
int main(int argc, char *argv[])
#endif
{
	void *context = NULL;
	struct i2c_master i2c;
	struct i2c_device dev;
	float vref = 0;

	/* base init */
	os_init();
	log_init(NULL, 0);

	/* check i2c device if using linux */
#ifdef TARGET_LINUX
	ERROR_IF_R(argc < 3, 1, "Give i2c device as first argument and reference (supply) voltage as second\nExample: ./i2c_mcp3221-x86.elf /dev/i2c-3 3.33");
	context = argv[1];
	vref = atof(argv[2]);
#endif

	/* open i2c */
	ERROR_IF_R(i2c_master_open(&i2c, context), 1, "unable to open i2c device");

	/* open mcp3221 */
	ERROR_IF_R(mcp3221_open(&dev, &i2c, MCP3221_ADDR_A2), 1, "unable to find mcp3221");

	/* read mcp3221 */
	while (1) {
		int16_t data = mcp3221_read(&dev);
		if (data < 0) {
			ERROR_MSG("failed reading mcp3221");
		} else {
			INFO_MSG("%.3f", (float)data * vref / 4096.0);
		}
		os_sleepf(0.2);
	}

	/* close mcp3221 */
	mcp3221_close(&dev);

	/* close i2c */
	i2c_master_close(&i2c);

	return 0;
}
