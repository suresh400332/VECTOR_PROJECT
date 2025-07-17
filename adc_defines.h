//clock defines
#define FOSC 12000000
#define CCLK (FOSC*5)
#define PCLK (CCLK/4)
#define ADC_CLK 3000000
//#define chno 0
#define CLKDIV ((PCLK/ADC_CLK)-1)
//ADCR SFR DEFINES
#define CLKDIV_BITS 8
#define PDN_BIT    21
//ADDR SFR defines
#define ADC_START_CONV_BITS 6
#define DONE_BIT 31    
