#include <i2c.h>

I2CTransaction *curr_transaction;

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
    // Enable I2C altogether (Pg 243)
    I2C0CONSET = BIT6;
}

void I2CStop() {
    // Stop signal (Pg 243)
    I2C0CONSET = BIT4;
}

void I2CStart() {
    // Stop signal (Pg 243)
    I2C0CONSET = BIT5;
}

int  I2CTransact(I2CTransaction *transact) {
	unsigned t_out=0xfffff;
    // Why do this? It makes sure that only one I2C transaction happens at a time
    //  because we are using a global pointer for it. Probably bad practice but I
    //  am OK with it for right now
	curr_transaction = transact;
	curr_transaction->idx=0;
	curr_transaction->status=0;
    I2CStart();

	while ((curr_transaction->status ==  0) && (t_out--)){
        // Wait for transaction to complete.
    }

	if (curr_transaction->status) {
		return -1;  // something went wrong maybe.
    } else {
		return 0; 	//  Not clear about status values and success - come back here.
    }
}
