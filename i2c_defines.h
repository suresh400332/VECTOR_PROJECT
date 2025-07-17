#ifndef I2C_DEFINES_H
#define I2C_DEFINES_H

// --- Pin Select ---
#define SCL_EN  (1 << 4)   // P0.2 -> SCL
#define SDA_EN  (1 << 6)   // P0.3 -> SDA

// --- I2C Bit Positions ---
#define I2EN_BIT   6
#define STA_BIT    5
#define STO_BIT    4
#define SI_BIT     3
#define AA_BIT     2

// --- Bit Masks ---
#define I2C_EN   (1 << I2EN_BIT)
#define I2C_STA  (1 << STA_BIT)
#define I2C_STO  (1 << STO_BIT)
#define I2C_SI   (1 << SI_BIT)
#define I2C_ACK  (1 << AA_BIT)

// --- I2C Speed (adjust for ~100kHz or ~400kHz as needed) ---
#define LOADVAL  60    // Adjust this based on PCLK (e.g. PCLK = 15MHz)

#endif
