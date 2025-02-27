/*
 * Scan i2c bus for devices.
 */

#include <libe/libe.h>
#ifdef TARGET_LINUX
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#endif

#ifndef I2C_CONTEXT
#define I2C_CONTEXT NULL
#define I2C_SDA     GPIOC6
#define I2C_SCL     GPIOC7
#endif


#ifdef TARGET_ESP32
int app_main(int argc, char *argv[])
#else
int main(int argc, char *argv[])
#endif
{
    void *context = I2C_CONTEXT;
    struct i2c_master i2c;
    struct i2c_device dev;

    /* base init */
    os_init();
    log_init();

    /* check i2c device if using linux */
#ifdef USE_FTDI
    ERROR_IF_R(os_ftdi_use(OS_FTDI_GPIO_0_TO_63, FTDI_VID, FTDI_PID, FTDI_DESC, FTDI_SERIAL), 1, "unable to open ftdi device for gpio 0-63");
#endif
#if defined(TARGET_LINUX) && !defined(USE_I2C_BITBANG)
    ERROR_IF_R(argc < 2, 1, "give i2c device as first argument");
    context = argv[1];
#endif

    /* open i2c */
    ERROR_IF_R(i2c_master_open(&i2c, context, 0, I2C_SCL, I2C_SDA), 1, "unable to open i2c device");

    while (1) {
        /* scan i2c bus */
        printf("Scanning I2C bus...\r\n");
        int found = 0;
        for (uint8_t a = 0; a < 128; a++) {
            os_wdt_reset();
            if (i2c_open(&dev, &i2c, a) == 0) {
                printf("Device found at address %02x\r\n", a);
                found++;
            }
            i2c_close(&dev);
        }
        printf("Found %d devices.\r\n", found);
        /* re-scan after timeout */
        os_delay_ms(1000);
    }

    /* close i2c */
    i2c_master_close(&i2c);

    log_quit();
    os_quit();

    return 0;
}
