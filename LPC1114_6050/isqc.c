#include <stdint.h>
#include <i2c.h>

void I2CInit() {
    unsigned div;
    // Enable IOCON and I2C (Page 34 Manual)
    SYSAHBCLKCTRL |= BIT5 | BIT16;
    // Select SDA mode for PIO_5, standard I2C (Pg 74, 85)
    IOCON_PIO0_5 = 1;
    // Select SCL mode for PI0_4, standard I2C (Pg 84)
    IOCON_PIO0_4 = 1;
    // De-init reset on I2C (Pg 240)
    PRESETCTRL = 1;
    // Set I2C clock rate (https://www.i2c-bus.org/speed/  100kHz standard) (Pg 242)
    div = 48000000 / (2 * 100000);
    I2C0SCLH = (div >> 16);
	I2C0SCLL = (div & 0xffff);
    // Enable I2C interrupts (Pg 507)
    ISER = 1;
    // Enable I2C altogether
    I2C0CONSET = BIT6;
}
