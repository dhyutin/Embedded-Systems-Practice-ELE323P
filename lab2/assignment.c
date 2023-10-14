#include "tm4c123gh6pm.h"
#include "PLL.h"
#include <stdint.h>
unsigned long Switch; 
void PortF_Init(void){ volatile unsigned long delay;
SYSCTL_RCGC2_R |= 0x00000020; // 1) activate clock for Port F
delay = SYSCTL_RCGC2_R; // allow time for clock to start
GPIO_PORTF_LOCK_R = 0x4C4F434B; // 2) unlock GPIO Port F
GPIO_PORTF_CR_R = 0x1F; // allow changes to PF4-0
// only PF0 needs to be unlocked, other bits can't be locked
GPIO_PORTF_AMSEL_R = 0x00; // 3) disable analog on PF
GPIO_PORTF_PCTL_R = 0x00000000; // 4) PCTL GPIO on PF4-0
GPIO_PORTF_DIR_R = 0x0E; // 5) PF4,PF0 in, PF3-1 out
GPIO_PORTF_AFSEL_R = 0x00; // 6) disable alt funct on PF7-0
GPIO_PORTF_PUR_R = 0x11; // enable pull-up on PF0 and PF4
GPIO_PORTF_DEN_R = 0x1F; // 7) enable digital I/O on PF4-0
}
unsigned long Led;
void Delay(void){unsigned long volatile time;
time =145448; // 0.1sec
while(time){
time--;
}
}

void SysInit(void)
{
NVIC_ST_CTRL_R = 0;
NVIC_ST_CURRENT_R = 0;// any write to current clears it
//NVIC_SYS_PRI3_R = NVIC_SYS_PRI3_R&0x00FFFFFF;//priority 0
NVIC_ST_CTRL_R = 0x00000005;// enable with core clock and interrupts
}
void SysLoad(unsigned long period)
{
NVIC_ST_RELOAD_R = period -1;
NVIC_ST_CURRENT_R = 0; // any value written to CURRENT clears
while((NVIC_ST_CTRL_R&0x00010000)==0){ // wait for count flag
}
}
int main(void)
{
PLL_Init();
PortF_Init();
SysInit();
while(1)
{
	int count = 0;
	Switch = GPIO_PORTF_DATA_R&0x11;
  if(Switch == 0x00)
	{
	while(count < 6){
		
		GPIO_PORTF_DATA_R = 0x02;
		SysLoad(200000);
		GPIO_PORTF_DATA_R = 0x00;
		SysLoad(200000);
		count++;
	}
  }
	else if(Switch == 0x01)
	{
	    GPIO_PORTF_DATA_R = 0x00;
	    SysLoad(16000000);
	    GPIO_PORTF_DATA_R = 0x00;
	    SysLoad(16000000);
	    GPIO_PORTF_DATA_R = 0x00;
	    SysLoad(16000000);
	    GPIO_PORTF_DATA_R = 0x00;
	    SysLoad(16000000);
	    GPIO_PORTF_DATA_R = 0x00;
	    SysLoad(16000000);
	    count = 0;
	}
	else if(Switch==0x10)
	{
	while(count < 5){
		GPIO_PORTF_DATA_R = 0x02;
		SysLoad(280000);
		GPIO_PORTF_DATA_R = 0x00;
		SysLoad(120000);
		count++;
	}

  }
	else if(Switch==0x11)
	{
	GPIO_PORTF_DATA_R = 0x00;
	SysLoad(4000000);
	}
}
}