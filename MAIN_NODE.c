#include <lpc21xx.h>
#include "lcd_defines.h"
#include "types.h"
#include "lcd.h"
#include "delay.h"
#include "can.h"
#include "i2c.h"
#include "interrupt.h"
#include "can_defines.h"

// Define MMA7660 I2C address
#define MMA7660_ADDR 0x4C

// Define LED pins
#define rLED (1<<16) // P1.16
#define gLED (1<<17) // P1.17

// Define custom character codes
#define CHAR_BAR            0
#define CHAR_ARROW_LEFT     2
#define CHAR_ARROW_RIGHT    3

// Define threshold for accelerometer readings
#define THRESHOLD          19

// Global variables
u32 cnt;
u32 adcval;
volatile u32 flag;
CANF rxF, txF;
u8 current = 0;

// Custom character bitmaps for LCD
u8 cgramLUT[] = {
    0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,
    0x0e,0x11,0x11,0x11,0x11,0x1f,0x1f,0x1f,
    0x02,0x06,0x0E,0x1E,0x0E,0x06,0x02,0x00,
    0x08,0x0C,0x0E,0x0F,0x0E,0x0C,0x08,0x00,
    0x02,0x06,0x0a,0x12,0x0a,0x06,0x02,0x00,
    0x08,0x0c,0x0a,0x09,0x0a,0x0c,0x08,0x00
};

// External interrupt 0 handler
void eint0_isr(void)__irq {
    flag = 1; // Set flag to indicate interrupt
    EXTINT = 1 << 0; // Clear EINT0 interrupt
    VICVectAddr = 0; // Acknowledge interrupt
}

// External interrupt 1 handler
void eint1_isr(void)__irq {
    flag = 2; // Set flag to indicate interrupt
    EXTINT = 1 << 1; // Clear EINT1 interrupt
    VICVectAddr = 0; // Acknowledge interrupt
}

// Main program
int main() {
    u32 i, per;
    s8 x, y, z;

    // Initialize peripherals
    init_i2c();
    init_MMA7660();
    initlcd();
    Init_CAN1();
    Enable_EINT0();
    Enable_EINT1();
    BuildCGRAM(cgramLUT, 48);
    delay_ms(100);

    while(1) {
        // Read accelerometer data
        x = read_MMA7660(0x00);
        y = read_MMA7660(0x01);
        z = read_MMA7660(0x02);

        // Check if any axis exceeds threshold
        if(x > THRESHOLD || x < (-THRESHOLD) || y > THRESHOLD || y < (-THRESHOLD) || z > THRESHOLD || z < (-THRESHOLD)) {
            strlcd("     ");
            cmdlcd(GOTO_LINE2_POS0);
            strlcd("AirBagOpen");
        } else {
            cmdlcd(GOTO_LINE2_POS0);
            strlcd("          ");
            cmdlcd(GOTO_LINE2_POS0);
            strlcd("Safe");
        }

        // Check if CAN receive buffer is full
        if(((C1GSR >> RBS_BIT_READ) & 1) != 0) {
            CAN1_Rx(&rxF);
            if(rxF.ID == 2 && rxF.vbf.DLC == 1) {
                per = rxF.Data1;
                cmdlcd(GOTO_LINE1_POS0);
                charlcd(1);
                for(i = 2; i < 7; i++) {
                    cmdlcd(GOTO_LINE1_POS0 + i);
                    charlcd(CHAR_BAR);
                }

                // Display percentage on LCD
                if(per > 80 && per <= 100) {
                    cmdlcd(GOTO_LINE1_POS0 + 6);
                } else if(per > 100) {
                    per = 100;
                    cmdlcd(GOTO_LINE1_POS0 + 6);
                } else if(per > 60 && per <= 80) {
                    cmdlcd(GOTO_LINE1_POS0 + 6);
                    strlcd(" ");
                } else if(per > 40 && per <= 60) {
                    cmdlcd(GOTO_LINE1_POS0 + 5);
                    strlcd("  ");
                } else if(per > 20 && per <= 40) {
                    cmdlcd(GOTO_LINE1_POS0 + 4);
                    strlcd("   ");
                } else if(per > 0 && per <= 20) {
                    cmdlcd(GOTO_LINE1_POS0 + 3);
                    strlcd("    ");
                } else {
                    cmdlcd(GOTO_LINE1_POS0 + 2);
                    strlcd("     ");
                }

                cmdlcd(GOTO_LINE1_POS0 + 8);
                strlcd("    ");
                cmdlcd(GOTO_LINE1_POS0 + 8);
                u32lcd(per);
                charlcd(0x25);
                cmdlcd(GOTO_LINE1_POS0 + 13);
                charlcd(CHAR_ARROW_LEFT);
                cmdlcd(GOTO_LINE1_POS0 + 15);
                charlcd(CHAR_ARROW_RIGHT);
            }
        }

        // Prepare CAN transmission
        txF.ID = 3;
        txF.vbf.DLC = 1;
        if(flag != 0) {
            if(flag == current) {
                txF.Data1 = 'X';
                txF.vbf.DLC = 1;
                CAN1_Tx(txF);
                current = 0;
            } else {
                if(flag == 1) {
                    cmdlcd(GOTO_LINE1_POS0 + 13);
                    charlcd(4);
                    txF.Data1 = 'L';
                } else if(flag == 2) {
                    cmdlcd(GOTO_LINE1_POS0 + 15);
                    charlcd(5);
                    txF.Data1 = 'R';
                }
                CAN1_Tx(txF);
                current = flag;
            }
            flag = 0;
        }
    }
}

