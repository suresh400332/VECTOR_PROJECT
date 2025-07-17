#include "delay.h"
#include "lcd_defines.h"
#include "lcd.h"
#include "adc.h"
#include "adc_defines.h"
#include "types.h"
#include "interrupt.h"
#include "can.h"
#include "can_defines.h"

// Define the minimum and maximum voltage levels for the sensor
#define Min 0.43
#define Max 2.45

// Declare variables for ADC value, ear voltage, and fuel percentage
u32 adcval;
f32 ear;
u32 fuel_per;

main(){
    CANF txF; // Declare a CAN frame structure for transmission

    // Initialize CAN1 interface
    Init_CAN1();

    // Initialize ADC (Analog-to-Digital Converter)
    init_adc();

    // Set up the CAN frame ID and data length code
    txF.ID = 2;
    txF.vbf.RTR = 0;
    txF.vbf.DLC = 1;

    while(1){
        // Read ADC value from channel 1 and calculate the corresponding voltage
        read_adc(1, &adcval, &ear);

        // Calculate the fuel percentage based on the ear voltage
        fuel_per = ((ear - Min) / (Max - Min)) * 100;

        // Introduce a delay of 500 milliseconds
        delay_ms(500);

        // Assign the fuel percentage to the CAN frame data
        txF.Data1 = fuel_per;
        txF.Data2 = 0;

        // Transmit the CAN frame
        CAN1_Tx(txF);

        // Invert the fuel percentage value for the next transmission
        txF.Data1 = ((~txF.Data1) & 0x0);
    }
}
