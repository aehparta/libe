/*
 * Random testing area for development.
 */

#include <getopt.h>

#define tool_i2c_chips
#include <libe/libe.h>

#ifndef TARGET_LINUX
#error "this tool is not meant to be compiled on other platforms than linux"
#endif

const char opts[] = "hd:c:a:";
struct option longopts[] = {
	{ "help", no_argument, NULL, 'h' },
	{ "device", required_argument, NULL, 'd' },
	{ "chip", required_argument, NULL, 'c' },
	{ "address", required_argument, NULL, 'a' },
	{ 0, 0, 0, 0 },
};

char *device = NULL;
char *chip_name = NULL;
long int address = 0;

struct chip {
	char name[16];
	char description[256];
	void (*help)(void);
	int (*exec)(struct i2c_master *master, uint8_t address, char *command, int argc, char *argv[]);
};

struct chip chips[] = {
	tool_i2c_chips
};

int chip_sort(const void *p1, const void *p2)
{
	struct chip *c1 = (struct chip *)p1, *c2 = (struct chip *)p2;
	return strcmp(c1->name, c2->name);
}

struct chip *chip_find(char *name)
{
	if (!name) {
		return NULL;
	}
	for (int i = 0; i < (sizeof(chips) / sizeof(struct chip)); i++) {
		if (strcmp(chips[i].name, name) == 0) {
			return &chips[i];
		}
	}
	return NULL;
}

void p_help(int argc, char *argv[])
{
	printf("Usage: %s [options] [command]\n", basename(argv[0]));
	printf(
	    "Execute command on an I2C slave attached to given bus.\n"
	    "\n"
	    "Use \"-c <CHIP> -h\" to show chip specific help.\n"
	    "\n");
	printf("Options:\n"
	       "  -c, --chip=<CHIP>            chip (mandatory)\n"
	       "  -d, --device=<DEVICE>        I2C device (mandatory)\n"
	       "  -a, --address=<HEX>          Optional I2C device address if selected chip can have multiple addresses\n"
	       "\n"
	      );
	printf("Chip can be one of the following:\n");
	qsort(chips, sizeof(chips) / sizeof(struct chip), sizeof(struct chip), chip_sort);
	for (int i = 0; i < (sizeof(chips) / sizeof(struct chip)); i++) {
		printf("%-32s%s\n", chips[i].name, chips[i].description);
	}
	printf("\n");
	printf("Examples\n"
	       "Show chip specific help:\n  %s -c hdc1080 -h\n"
	       "Read HDC1080 temperature and humidity:\n  %s -c hdc1080 -d /dev/i2c-13 read\n"
	       , basename(argv[0]), basename(argv[0]));
}

int p_options(int argc, char *argv[])
{
	int err = 0;
	int longindex = 0, c;

	while ((c = getopt_long(argc, argv, opts, longopts, &longindex)) > -1) {
		switch (c) {
		case 'd':
			device = optarg;
			break;
		case 'c':
			chip_name = optarg;
			break;
		case 'a':
			address = strtol(optarg, NULL, 16);
			if (address < 1 || address > 0x7f) {
				fprintf(stderr, "Address out of range.\n");
				return -1;
			}
			break;
		case 'h':
			err = 1;
			break;
		default:
			return -1;
		}
	}

	return err;
}

int main(int argc, char *argv[])
{
	/* parse command line options */
	int err = p_options(argc, argv);
	if (err) {
		if (err < 0) {
			fprintf(stderr, "Invalid arguments.\n");
			exit(EXIT_FAILURE);
		} else {
			struct chip *chip = chip_find(chip_name);
			if (chip) {
				printf("%s specific usage.\n\n", chip_name);
				chip->help();
			} else {
				p_help(argc, argv);
			}
		}
		exit(EXIT_SUCCESS);
	}
	/* check that all required arguments were given */
	if (!chip_name || !device || optind >= argc) {
		fprintf(stderr, "Chip, device and command must be specified.\n");
		exit(EXIT_FAILURE);
	}
	/* check that chip is a valid one */
	struct chip *chip = chip_find(chip_name);
	if (!chip) {
		fprintf(stderr, "Unsupported chip.\n");
		exit(EXIT_FAILURE);
	}
	/* low level initialization */
	os_init();
	log_init();
	err = EXIT_FAILURE;
	/* open i2c */
	struct i2c_master i2c;
	if (i2c_master_open(&i2c, device, 100000, 0, 0)) {
		fprintf(stderr, "Invalid I2C device, reason: %s\n", error_last);
	} else {
		/* execute command */
		err = chip->exec(&i2c, address, argv[optind], argc - optind, &argv[optind]) ? EXIT_FAILURE : EXIT_SUCCESS;
		i2c_master_close(&i2c);
	}

	log_quit();
	os_quit();
	return err;


	// if (argc < 2) {
	// 	p_help(argc, argv);
	// 	fprintf(stderr, "\nAt least chip must be specified.\n");
	// } else {
	// 	struct chip *chip = NULL;
	// 	for (int i = 0; i < (sizeof(chips) / sizeof(struct chip)); i++) {
	// 		if (strcmp(chips[i].name, argv[1]) == 0) {
	// 			chip = &chips[i];
	// 			break;
	// 		}
	// 	}
	// 	if (chip && argc == 2) {
	// 		chip->help();
	// 	} else if (chip && argc >= 4) {
	// 		if (i2c_master_open(&i2c, argv[2], 100000, 0, 0)) {
	// 			fprintf(stderr, "Invalid I2C device.\n");
	// 		} else {
	// 			chip->exec(&i2c, argv[3], argc - 3, &argv[3]);
	// 			i2c_master_close(&i2c);
	// 		}
	// 	} else if (!chip) {
	// 		fprintf(stderr, "\nUnsupported chip.\n");
	// 	} else {
	// 		fprintf(stderr, "\nInvalid arguments.\n");
	// 	}
	// }

	// log_quit();
	// os_quit();
	// return 0;
}
