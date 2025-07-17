#include <LPC21xx.h>
#include "can.h"
#include "delay.h"

// Define the bit position for the Receive Buffer Status in the CAN Global Status Register
#define RBS_BIT_READ 0

// Define the base address for LEDs connected to P0.0 to P0.7
#define LED_AL  0  // P0.0 to P0.7

// Define a structure to hold received CAN frame data
CANF rxF;

int main()
{
    volatile s32 i; // Declare a volatile variable for loop control
    u8 mode = 0; // Initialize mode to 0 (OFF state)

    // Initialize CAN1 interface
    Init_CAN1();

    // Configure P0.0 to P0.7 as output for LEDs
    IODIR0 |= 0xFF << LED_AL;
    IOSET0 = 0xFF << LED_AL; // Initially turn off all LEDs (active-low)

    while(1)
    {
        // --- Check for new CAN message ---
        if(((C1GSR >> RBS_BIT_READ) & 1))
        {
            CAN1_Rx(&rxF); // Receive a CAN message into rxF
            if (rxF.ID == 3) // Check if the message ID is 3
            {
                // Check the data byte to determine the mode
                if (rxF.Data1 == 'L')
                {
                    mode = (mode != 1) ? 1 : 0; // Toggle to LEFT mode or OFF
                }
                else if (rxF.Data1 == 'R')
                {
                    mode = (mode != 2) ? 2 : 0; // Toggle to RIGHT mode or OFF
                }
            }
        }

        // --- LEFT pattern ---
        if (mode == 1)
        {
            // Loop through LEDs from P0.0 to P0.7
            for (i = 0; i < 8 && mode == 1; i++)
            {
                IOCLR0 = 1 << (LED_AL + i); // Turn on LED (active-low)
                delay_ms(100); // Delay for 100 milliseconds
                if(i == 7)
                {
                    IOSET0 = 0xFF << LED_AL; // Turn off all LEDs after the last one
                }

                // Check for new CAN message during blinking
                if(((C1GSR >> RBS_BIT_READ) & 1))
                {
                    CAN1_Rx(&rxF); // Always fetch new data
                    if (rxF.ID == 3)
                    {
                        // Check the data byte to determine the mode
                        if (rxF.Data1 == 'L')
                        {
                            mode = (mode != 1) ? 1 : 0; // Toggle to LEFT mode or OFF
                        }
                        else if (rxF.Data1 == 'R')
                        {
                            mode = (mode != 2) ? 2 : 0; // Toggle to RIGHT mode or OFF
                        }
                        break; // Exit the loop if a new message is received
                    }
                }
            }
        }

        // --- RIGHT pattern ---
        else if (mode == 2)
        {
            // Loop through LEDs from P0.7 to P0.0
            for (i = 7; i >= 0 && mode == 2; i--)
            {
                IOCLR0 = 1 << (LED_AL + i); // Turn on LED (active-low)
                delay_ms(100); // Delay for 100 milliseconds
                if(i == 0)
                {
                    IOSET0 = 0xFF << LED_AL; // Turn off all LEDs after the last one
                }

                // Check for new CAN message during blinking
                if(((C1GSR >> RBS_BIT_READ) & 1))
                {
                    CAN1_Rx(&rxF); // Always fetch new data
                    if (rxF.ID == 3)
                    {
                        // Check the data byte to determine the mode
                        if (rxF.Data1 == 'L')
                        {
                            mode = (mode != 1) ? 1 : 0; // Toggle to LEFT mode or OFF
                        }
                        else if (rxF.Data1 == 'R')
                        {
                            mode = (mode != 2) ? 2 : 0; // Toggle to RIGHT mode or OFF
                        }
                        break; // Exit the loop if a new message is received
                    }
                }
            }
        }

        // --- OFF mode ---
        else
        {
            IOSET0 = 0xFF << LED_AL; // Turn off all LEDs
        }
    }
}
