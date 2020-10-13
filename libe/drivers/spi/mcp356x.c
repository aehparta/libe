/*
 * libe cross-platform library
 *
 * Authors: Antti Partanen <aehparta@iki.fi>
 */

#ifdef USE_DRIVER_MCP356X /* because esp32 */

#include <libe/libe.h>


#define DRVBITS_ADDR    0

/* internals */
static int8_t mcp356x_opt(struct spi_device *device, uint8_t opt, void *value);
// static uint8_t mcp356x_rd_reg8(struct spi_device *device, uint8_t addr);
static void mcp356x_modify_reg8(struct spi_device *device, uint8_t addr, uint8_t value, uint8_t mask);


int8_t mcp356x_open(struct spi_device *device, struct spi_master *master, uint8_t ss)
{
	int8_t err = spi_open(device, master, ss);
	IF_R(err, err);

	/* default "address" */
	device->driver_bits[DRVBITS_ADDR] = MCP356X_DEFAULT_ADDR;
	/* opt callback */
	device->opt = mcp356x_opt;

	return 0;
}

void mcp356x_close(struct spi_device *device)
{
	spi_close(device);
}

int32_t mcp356x_rd(struct spi_device *device)
{
	uint8_t data[4];

	/* read */
	data[0] = device->driver_bits[DRVBITS_ADDR] | MCP356X_ADDR_ADCDATA | MCP356X_STATIC_READ;
	spi_transfer(device, data, 4);

	return (data[1] << 24) | (data[2] << 16) | (data[3] << 8);
}

int8_t mcp356x_ch(struct spi_device *device, int8_t channel)
{
	uint8_t data[2];
	data[0] = device->driver_bits[DRVBITS_ADDR] | MCP356X_ADDR_MUX | MCP356X_INC_WRITE;
	data[1] = channel;
	spi_transfer(device, data, 2);
	return channel;
}

uint8_t mcp356x_fast_command(struct spi_device *device, uint8_t command)
{
	command |= device->driver_bits[DRVBITS_ADDR];
	spi_transfer(device, &command, 1);
	return command;
}


/* internals */


static int8_t mcp356x_opt(struct spi_device *device, uint8_t opt, void *value)
{
	uint8_t *v8 = value;

	switch (opt) {
	case MCP356X_OPT_ADDR:
		device->driver_bits[DRVBITS_ADDR] = *v8 << 6;
		return 0;
	/* CONFIG0 */
	case MCP356X_OPT_CLK_SEL:
		mcp356x_modify_reg8(device, MCP356X_ADDR_CONFIG0, *v8 << 4, 0x30);
		return 0;
	case MCP356X_OPT_CS_SEL:
		mcp356x_modify_reg8(device, MCP356X_ADDR_CONFIG0, *v8 << 2, 0x0c);
		return 0;
	case MCP356X_OPT_ADC_MODE:
		mcp356x_modify_reg8(device, MCP356X_ADDR_CONFIG0, *v8, 0x03);
		return 0;
	/* CONFIG1 */
	case MCP356X_OPT_PRE:
		mcp356x_modify_reg8(device, MCP356X_ADDR_CONFIG1, *v8 << 6, 0xc0);
		return 0;
	case MCP356X_OPT_OSR:
		mcp356x_modify_reg8(device, MCP356X_ADDR_CONFIG1, *v8 << 2, 0x3c);
		return 0;
	/* CONFIG2 */
	case MCP356X_OPT_BOOST:
		mcp356x_modify_reg8(device, MCP356X_ADDR_CONFIG2, *v8 << 6, 0xc0);
		return 0;
	case MCP356X_OPT_GAIN:
		mcp356x_modify_reg8(device, MCP356X_ADDR_CONFIG2, *v8 << 3, 0x38);
		return 0;
	case MCP356X_OPT_AZ_MUX:
		mcp356x_modify_reg8(device, MCP356X_ADDR_CONFIG2, *v8 << 2, 0x04);
		return 0;
	/* CONFIG3 */
	case MCP356X_OPT_CONV_MODE:
		mcp356x_modify_reg8(device, MCP356X_ADDR_CONFIG3, *v8 << 6, 0xc0);
		return 0;
	/* IRQ */
	case MCP356X_OPT_IRQ_MODE:
		mcp356x_modify_reg8(device, MCP356X_ADDR_IRQ, *v8 << 2, 0x0c);
		return 0;
	}

	return -1;
}

// static uint8_t mcp356x_rd_reg8(struct spi_device *device, uint8_t addr)
// {
// 	uint8_t data[2] = { device->driver_bits[DRVBITS_ADDR] | addr | MCP356X_STATIC_READ, 0x00 };
// 	spi_transfer(device, data, 2);
// 	return data[1];
// }

static void mcp356x_modify_reg8(struct spi_device *device, uint8_t addr, uint8_t value, uint8_t mask)
{
	uint8_t data[2] = { device->driver_bits[DRVBITS_ADDR] | addr | MCP356X_STATIC_READ, 0x00 };
	spi_transfer(device, data, 2);
	data[0] = device->driver_bits[DRVBITS_ADDR] | addr | MCP356X_INC_WRITE;
	data[1] = (data[1] & ~mask) | (value & mask);
	spi_transfer(device, data, 2);
}

#endif
