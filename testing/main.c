/*
 * Random testing area for development.
 */

#include <string.h>
#include <libe/libe.h>


// struct optt {
// 	int8_t (*opt)(struct optt *optt, uint8_t opt, void *value, uint8_t flags);
// };

// int8_t o(struct optt *optt, uint8_t opt, void *value, uint8_t flags)
// {
// 	printf("%s opt %u\n", (flags & OPT_FLAG_MASK_SET_GET) == OPT_FLAG_GET ? "get" : "set", opt);
// 	return -1;
// }


// #ifdef TARGET_ESP32
// int app_main(int argc, char *argv[])
// #else
// int main(void)
// #endif
// {
// 	struct optt x;
// 	float f = 32;

// 	memset(&x, 0, sizeof(x));

// 	opt_set(&x, 8, &f);

// 	x.opt = o;

// 	opt_set(&x, 9, &f);
// 	opt_set_u8(&x, 10, 1);

// 	opt_get(&x, 17, &f);

// 	return 0;
// }


// #define VREF_VCC

#ifdef VREF_VCC
#define VREF                        3.3
#define OFFSET                      885
#else
#define VREF                        2.555
#define OFFSET                      1010
#endif


int main(void)
{
	struct spi_master master;
	struct spi_device device;
	uint8_t data[32];

	os_init();
	log_init();

	/* open ftdi in mpsse mode for spi */
#ifdef USE_FTDI
	ERROR_IF_R(os_ftdi_use(OS_FTDI_GPIO_0_TO_63, 0, 0, NULL, NULL), -1, "unable to open ftdi device for gpio 0-63");
	os_ftdi_set_mpsse(0);
#endif

	/* irq gpio */
	gpio_input(19);

	/* open ft232h type device and try to see if it has a nrf24l01+ connected to it through mpsse-spi */
	ERROR_IF_R(spi_master_open(
	               &master, /* must give pre-allocated spi master as pointer */
	               NULL, /* context depends on platform */
	               10e6,
	               2,
	               1,
	               0
	           ), -1, "failed to open spi master");
	mcp356x_open(&device, &master, 3);

	/* reset */
	mcp356x_fast_command(&device, MCP356X_CMD_RESET);
	os_sleepf(0.3);

	/* read configuration */
	memset(data, 0, sizeof(data));
	data[0] = 0x47;
	spi_transfer(&device, data, 7);
	HEX_DUMP(data, 7, 1);

	/* configure */
	optwr_u8(&device, MCP356X_OPT_CLK_SEL, 0x0);
	// optwr_u8(&device, MCP356X_OPT_CLK_SEL, 0x0);
	optwr_u8(&device, MCP356X_OPT_ADC_MODE, 0x3);
	optwr_u8(&device, MCP356X_OPT_PRE, 0x3);

	/* OSR */
	optwr_u8(&device, MCP356X_OPT_OSR, MCP356X_OSR_16384);
	// optwr_u8(&device, MCP356X_OPT_OSR, MCP356X_OSR_1024);

	optwr_u8(&device, MCP356X_OPT_BOOST, 0x3);
	optwr_u8(&device, MCP356X_OPT_GAIN, MCP356X_GAIN_X16);
	optwr_u8(&device, MCP356X_OPT_CONV_MODE, 0x3);
	optwr_u8(&device, MCP356X_OPT_IRQ_MODE, 0x1);

	// data[0] = MCP356X_DEFAULT_ADDR | MCP356X_INC_WRITE | MCP356X_ADDR_CONFIG0; /* incremental write starting from register address 1 (CONFIG0) */
	// data[1] = 0xf2; /* CONFIG0: internal clock, standby mode */
	// data[2] = 0x00 | (0xf << 2); /* CONFIG1 */
	// data[3] = 0xc3 | (0x5 << 3); /* CONFIG2 */
	// data[4] = 0x80; /* CONFIG3: one-shot conversion with standby */
	// data[5] = 0x07;
	// spi_transfer(&device, data, 6);
	// os_delay_ms(100);

	/* read configuration */
	memset(data, 0, sizeof(data));
	data[0] = 0x47;
	spi_transfer(&device, data, 7);
	HEX_DUMP(data, 7, 1);

	os_sleepf(0.3);

	/* first read */
	mcp356x_ch(&device, MCP356X_CH_01);
	for (int i = 0; i < 10; i++) {
		while (gpio_read(19));
		mcp356x_rd(&device);
	}
	os_sleepf(1.0);

	/* read */
	int32_t min = -1e6, max = -1e6;
	double sum = 0;
	int count = 0;
	while (1) {
		while (gpio_read(19));

		int32_t x = mcp356x_rd(&device);

		x /= 256;
		x += OFFSET;
		min = min <= -1e6 ? x : min;
		max = max <= -1e6 ? x : max;
		min = x < min ? x : min;
		max = x > max ? x : max;

		sum += (double)x / (double)0x7fffff * VREF / 16.0 * 1000000.0;
		count++;
		if (count >= 10) {
			sum /= count;
			printf("%+12.1lf %+12d %+12d %+12d %d\n", sum, x, min, max, max - min);
			sum = 0;
			count = 0;
		}
	}

	/* free */
	spi_master_close(&master);
	log_quit();
	os_quit();
	return 0;
}
