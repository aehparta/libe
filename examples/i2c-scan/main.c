/*
 * Scan i2c bus for devices.
 */

#include <libe/os.h>
#include <libe/log.h>
#include <libe/i2c.h>
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

	/* base init */
	os_init();
	log_init(NULL, 0);

	/* check i2c device if using linux */
#ifdef TARGET_LINUX
	ERROR_IF_R(argc < 2, 1, "give i2c device as first argument");
	context = argv[1];
#endif

	/* open i2c */
	ERROR_IF_R(i2c_master_open(&i2c, context), 1, "unable to open i2c device");

	/* scan i2c bus */
	int found = 0;
	for (int a = 0; a < 128; a++) {
		if (i2c_open(&dev, &i2c, a) == 0) {
			INFO_MSG("Device found at address %02x", a);
			found++;
		}
		i2c_close(&dev);
	}
	INFO_MSG("Found %d devices.", found);

	/* close i2c */
	i2c_master_close(&i2c);

	return 0;
}
