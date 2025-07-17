#include"types.h"
void writelcd(u8 byte);
void cmdlcd(u8 cmd);
void charlcd(u8 ascii);
void initlcd(void);
void strlcd(s8*);
void u32lcd(u32);
void s32lcd(s32);
void f32lcd(f32,u8);
void buildcgram(s8*,u8);
void hexa(u8);
void BuildCGRAM(u8 *p,u8 nBytes);
