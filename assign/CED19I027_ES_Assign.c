// CED19I027 ES Assignment - Stepper Motor
// 1 rotation -200 steps / rotation
// 9 rotation in 1 min(60 sec)
// 1 rotation = 60/9 sec
// 1 rotation = 200 steps
// 1 Step = 1/30 Seconds
#include "PLL.h"
#include "tm4c123gh6pm.h"
#include <stdint.h>
struct State
{
    uint32_t present;
    uint32_t Time; 
    uint32_t Next;
};

typedef const struct State State_t;

#define S0 0
#define S1 1
#define S2 2
#define S3 3

State_t FSM[4]=
{{0x5,2666667,S1},{0x6,2666667,S2},{0xA,2666667,S3},{0x9,2666667,S0}
};

uint32_t S;
uint32_t Input;

// Initialise port e to use 4 ports
void PortE_Init(void)
{ 
	unsigned long volatile delay;
  SYSCTL_RCGC2_R |= 0x00000010;
  delay = SYSCTL_RCGC2_R;
  GPIO_PORTE_DIR_R |= 0x0F;
  GPIO_PORTE_DEN_R |= 0x0F;
}
		
void SysLoad(unsigned long period)
{
	NVIC_ST_RELOAD_R = period -1;
	NVIC_ST_CURRENT_R = 0;
	while((NVIC_ST_CTRL_R&0x00010000)==0)
	{	}
}		

void SysInit(void)
{
	NVIC_ST_CTRL_R = 0;
	NVIC_ST_CURRENT_R = 0;
	NVIC_SYS_PRI3_R=NVIC_SYS_PRI3_R&0x00FFFFFF;
	NVIC_ST_CTRL_R=0x00000005;
}
void systick_wait10ms(uint32_t delay)
{
	unsigned long i;
	for(i = 0;i<delay;i++)
	{
		SysLoad(800000);
	}
}
int main(void)
{ 
	volatile uint32_t delay;
  PLL_Init(); 
	PortE_Init();
  SysInit();

  S = S0;
  while(1)
	{
    GPIO_PORTE_DATA_R = FSM[S].present;  
    SysLoad(FSM[S].Time);
    S = FSM[S].Next; 
  }
}
