#include <lpc21xx.h>
#include "types.h"
#include "delay.h"
#include "lcd_defines.h"
#include "lcd.h"

void writelcd(u8 byte)
{
    //select write operations
    IOCLR0=1<<LCD_RW;
    //write any byte onto d0 to d7
    IOPIN0=((IOPIN0&~(0XFF<<LCD_DATA))|(byte<<LCD_DATA));
    //apply high to low on enable
    IOSET0=1<<LCD_EN;
    delay_us(1);
    IOCLR0=1<<LCD_EN;
    delay_ms(2);
}

void cmdlcd (u8 cmd)
{
    //select command register
    IOCLR0=1<<LCD_RS;
    //write to command register
    writelcd(cmd);
}

void charlcd(u8 ascii)
{
    //select data reg
    IOSET0=1<<LCD_RS;
    //write ddram via data reg
    writelcd(ascii);
}

void initlcd(void)
{
    //cfg(p0.8)(d0) to p0.15(d7)
    IODIR0|=((0XFF<<LCD_DATA));
    IODIR0|=(1<<LCD_RS)|(1<<LCD_RW)|(1<<LCD_EN);
    delay_ms(15);
    cmdlcd(0x30);
    delay_ms(5);
    cmdlcd(0x30);
    delay_us(160);
    cmdlcd(0x30);
    cmdlcd(MODE_8BIT_2LINE);
    cmdlcd(DSP_ON_CUR_OFF);
    cmdlcd(CLEAR_LCD);
    cmdlcd(SHIFT_CUR_RIGHT);
}

void strlcd(s8 *s)
{
    while(*s)
    {
        charlcd(*s++);
    }
}

void u32lcd(u32 n)
{
    s32 i=0;
    u8 digit[10];
    if(n==0)
    {
        charlcd('0');
    }
    else
    {
        while(n>0)
        {
            digit[i]=(n%10)+48;
            n=n/10;
            i++;
        }
        for(--i;i>=0;i--)
        {
            charlcd(digit[i]);
        }
    }
}

void s32lcd(s32 n)
{
    if(n<0)
    {
        charlcd('-');
        n=-n;
    }
    u32lcd(n);
}

void f32lcd(f32 fnum,u8 ndp)
{
    s32 i;
    u32 n;
    if(fnum<0)
    {
        charlcd('-');
        fnum=-fnum;
    }
    n=fnum;
    u32lcd(n);
    charlcd('.');
    for(i=0;i<ndp;i++)
    {
        fnum=(fnum-n)*10;
        n=fnum;
        charlcd(n+48);
    }
}
void hexa(u8 byte)
{
	u8 str[]="0123456789ABCDEF";
	charlcd(str[byte>>4]);
	charlcd(str[byte&0x0f]);
}
void BuildCGRAM(u8 *p,u8 nBytes)
{
	u8 i;
	//take curson to start of cgram
	cmdlcd(0x40);
	for(i=0;i<nBytes;i++)
	{
		//Write to CGRAM 
		charlcd(p[i]);
	}
	//bring back cursor to ddram start
	cmdlcd(0x80);
}	
