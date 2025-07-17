#include <LPC21xx.h>
#include "types.h"
#include "delay.h"
#include "i2c_defines.h"
#include "i2c.h"

void init_i2c(void) {
    PINSEL0 |= SCL_EN | SDA_EN;       // Enable SCL/SDA on P0.2/P0.3
    I2SCLL = LOADVAL;
    I2SCLH = LOADVAL;
    I2CONSET = I2C_EN;                // Enable I2C interface
}

void i2c_start(void) {
    I2CONSET = I2C_STA;               // Set start condition
    while ((I2CONSET & I2C_SI) == 0);    // Wait for SI
    I2CONCLR = I2C_STA;               // Clear start bit
}

void i2c_restart(void) {
    I2CONSET = I2C_STA;               // Repeated start
    I2CONCLR = I2C_SI;                // Clear SI
    while ((I2CONSET & I2C_SI) == 0);    // Wait for SI
    I2CONCLR = I2C_STA;               // Clear start bit
}

void i2c_write(u8 dat) {
    I2DAT = dat;                      // Write data to data register
    I2CONCLR = I2C_SI;                // Clear SI to resume
    while ((I2CONSET & I2C_SI) == 0);    // Wait for SI
}

u8 i2c_masterack(void) {
    I2CONSET = I2C_ACK;               // Assert ACK
    I2CONCLR = I2C_SI;                // Clear SI
    while ((I2CONSET & I2C_SI) == 0);    // Wait for SI
    I2CONCLR = I2C_ACK;               // Clear ACK
    return I2DAT;
}

u8 i2c_nack(void) {
    I2CONCLR = I2C_ACK;               // NACK
    I2CONCLR = I2C_SI;                // Clear SI
    while ((I2CONSET & I2C_SI) == 0);    // Wait for SI
    return I2DAT;
}

void i2c_stop(void) {
    I2CONSET = I2C_STO;               // Send STOP condition
    I2CONCLR = I2C_SI;                // Clear SI
    // STOP is auto-cleared by hardware
}

void i2c_device_write(u8 sa, u8 reg, u8 data) {
    i2c_start();
    i2c_write(sa << 1);               // SLA+W
    i2c_write(reg);                   // Register
    i2c_write(data);                  // Data
    i2c_stop();
    delay_ms(5);                      // Delay for device internal write
}

u8 i2c_device_read(u8 sa, u8 reg) {
    u8 data;
    i2c_start();
    i2c_write(sa << 1);               // SLA+W
    i2c_write(reg);                   // Register
    i2c_restart();
    i2c_write((sa << 1) | 1);         // SLA+R
    data = i2c_nack();                // Read data with NACK
    i2c_stop();
    return data;
}
void init_MMA7660()
{
        i2c_start();
        i2c_write(0x98);
        i2c_write(0x07);
        i2c_write(0x01);
        i2c_stop();
}
u8 read_MMA7660(u8 reg)
{
        u8 data;
        i2c_start();
        i2c_write(0x98);
        i2c_write(reg);
        i2c_restart();
        i2c_write(0x99);
        data=i2c_nack();
        if(data>31)
        {
                data=data-64;
        }
        i2c_stop();
        delay_ms(10);
        return data;
}
