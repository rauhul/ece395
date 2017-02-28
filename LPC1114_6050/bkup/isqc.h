#ifndef I2C_H
#define I2C_H

#include <stdint.h>
#include "lcp111x.h"

#define MAX_I2C_DATA 64

typedef struct {
    int mode;
    uint8_t s_addr;
    unsigned idx;
    unsigned cnt;
    int status;
    uint8_t data[MAX_I2C_DATA];
} I2CTransaction;

void I2CInit();
void I2CStop();
void I2CStart();
int  I2CTransact(I2CTransaction *transact);

#endif /* I2C_H */
