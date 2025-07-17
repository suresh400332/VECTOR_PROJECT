#ifndef I2C_H
#define I2C_H

#include "types.h"

// Core functions
void init_i2c(void);
void i2c_start(void);
void i2c_restart(void);
void i2c_write(u8 dat);
void i2c_stop(void);
u8 i2c_masterack(void);
u8 i2c_nack(void);
void init_MMA7660(void);
u8 read_MMA7660(u8 );

// Device-level helper
void i2c_device_write(u8 sa, u8 reg, u8 data);
u8 i2c_device_read(u8 sa, u8 reg);

#endif
