/*
 * Random testing area for development.
 */

#include <libe/os.h>
#include <libe/log.h>

#define tool_i2c_chips
#include <libe/i2c.h>

#ifndef TARGET_LINUX
#error "this tool is not meant to be compiled on other platforms than linux"
#endif

struct chip {
	char name[16];
	char description[256];
	void (*help)(void);
	void (*exec)(char *command, int argc, char *argv[]);
};

struct chip chips[] = {
	tool_i2c_chips
};

int chip_sort(const void *p1, const void *p2)
{
	struct chip *c1 = (struct chip *)p1, *c2 = (struct chip *)p2;
	return strcmp(c1->name, c2->name);
}

void p_help(int argc, char *argv[])
{
	printf("Usage: %s <chip> [<device> <command>]\n", basename(argv[0]));
	printf(
	    "Execute command on an I2C slave attached to given bus.\n"
	    "Device must be /dev/i2c-#, example /dev/i2c-11.\n"
	    "If device and command are omitted, then chip specific help is printed.\n"
	    "\n");
	printf("Chip can be one of the following:\n");
	qsort(chips, sizeof(chips) / sizeof(struct chip), sizeof(struct chip), chip_sort);
	for (int i = 0; i < (sizeof(chips) / sizeof(struct chip)); i++) {
		printf("%-16s - %s\n", chips[i].name, chips[i].description);
	}
}

int main(int argc, char *argv[])
{
	struct i2c_master i2c;
	struct i2c_device dev;

	os_init();
	log_init(NULL, 0);

	if (argc < 2) {
		p_help(argc, argv);
		fprintf(stderr, "\nAt least chip must be specified.\n");
	} else {
		struct chip *chip = NULL;
		for (int i = 0; i < (sizeof(chips) / sizeof(struct chip)); i++) {
			if (strcmp(chips[i].name, argv[1]) == 0) {
				chip = &chips[i];
				break;
			}
		}
		if (chip && argc == 2) {
			chip->help();
		} else if (chip && argc >= 4) {
			if (i2c_master_open(&i2c, argv[2], 100000, 0, 0)) {
				fprintf(stderr, "Invalid I2C device.\n");
			} else {
				chip->exec(argv[3], argc - 3, &argv[3]);
				i2c_master_close(&i2c);
			}
		} else if (!chip) {
			fprintf(stderr, "\nUnsupported chip.\n");
		} else {
			fprintf(stderr, "\nInvalid arguments.\n");
		}
	}

	log_quit();
	os_quit();
	return 0;
}
