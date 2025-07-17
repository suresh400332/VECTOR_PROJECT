#include <lpc21xx.h>
#include "interrupt.h"
#include "types.h"
#include "interrupt_defines.h"
void Enable_EINT0(void)
{
 PINSEL0=PINSEL0&~(3<<2)|(3<<2);
 VICIntEnable=1<<EINT0_VIC_CHNO;
 VICVectCntl0=(1<<5)|EINT0_VIC_CHNO;
 VICVectAddr0=(u32)eint0_isr;
 EXTMODE=1<<0;
 }
void Enable_EINT1(void)
{
	PINSEL0=PINSEL0&~(3<<28)|(2<<28);
 VICIntEnable=1<<EINT0_VIC_CHNO;
 VICVectCntl0=(1<<5)|EINT0_VIC_CHNO;
 VICVectAddr1=(u32)eint1_isr;
 EXTMODE=1<<1;
}
	
