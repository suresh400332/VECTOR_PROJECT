#include <lpc21xx.h>
#include "types.h"
#include "delay.h"
#include "adc.h"
#include "adc_defines.h"
void init_adc(void)
{
PINSEL1=(PINSEL1&~(3<<28))|(1<<28);
ADCR=(1<<PDN_BIT)|(CLKDIV<<CLKDIV_BITS);
}
void read_adc(u8 chno,u32 *adcval,f32 *ear)
{
ADCR&=0XFFFFFF00;
ADCR|=1<<chno;
ADCR|=1<<24;
delay_us(3);
while(((ADDR>>DONE_BIT)&1)==0);
ADCR&=~(1<<24);
*adcval=((ADDR>>ADC_START_CONV_BITS)&1023);
*ear=(3.3/1023)*(*adcval);
}
