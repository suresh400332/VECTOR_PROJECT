#ifndef __CAN_H__
#define __CAN_H__
#include "types.h"
typedef struct CAN_Frame
{
	u32 ID;
	struct BitField
	{
		u8 RTR : 1;
		u8 DLC : 4;  
	}vbf; 
	u32 Data1,Data2;//8-bytes
}CANF;
void Init_CAN1(void);
void CAN1_Tx(CANF);
void CAN1_Rx(CANF*);
#endif
