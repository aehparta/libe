/*
 * Random testing area for development.
 */

#include <libe/os.h>
#include <libe/log.h>
#include <libe/i2c.h>

#include <libe/drivers/i2c/hdc1080.h>

#ifndef TARGET_LINUX
#error "this tool is not meant to be compiled on other platforms than linux"
#endif

struct chip {
	char name[16];
	char description[256];
};

struct chip chips[] = {
	{ "HDC1080", "Ti temperature and humidity sensor" },
	{ "MCP3221", "Microchip 12-bit ADC" },
	{ "CAP1293", "Microchip touch sensor" },
	{ "FAN5702", "Fairchild LED driver" },
	{ "SHT21", "Sensirion temperature and humidity sensor" },
	{ "MCP39F521", "Microchip power monitor" },
};

int chip_sort(const void *p1, const void *p2)
{
	struct chip *c1 = (struct chip *)p1, *c2 = (struct chip *)p2;
	return strcmp(c1->name, c2->name);
}

void p_help(int argc, char *argv[])
{
	printf(
	    "\n"
	    "Usage:\n"
	    " %s <device> <chip> [command]\n"
	    "\n"
	    , basename(argv[0]));
	printf(
	    "Execute command on an I2C slave attached to given bus.\n"
	    "\n");
	printf(
	    "Device must be /dev/i2c-<#>. Example /dev/i2c-11\n"
	    "\n");

	qsort(chips, sizeof(chips) / sizeof(struct chip), sizeof(struct chip), chip_sort);
	printf("Chip can be one of the following:\n");
	for (int i = 0; i < (sizeof(chips) / sizeof(struct chip)); i++) {
		printf("%-16s - %s\n", chips[i].name, chips[i].description);
	}
	printf("\n");
}

int main(int argc, char *argv[])
{
	struct i2c_master i2c;
	struct i2c_device dev;

	os_init();
	log_init(NULL, 0);

	p_help(argc, argv);

	// ERROR_IF_R(argc < 2, 1, "give i2c device as first argument");

	// i2c_master_open(&i2c, argv[1], 100000, 0, 0);

	// i2c_master_close(&i2c);
	log_quit();
	os_quit();
	return 0;
}
