#include "tm4c123gh6pm.h"
#include <stdint.h>
#include "PLL.h"

unsigned long i;

void PortB_Init(void) {
	volatile unsigned long delay;
	SYSCTL_RCGC2_R |= 0x00000002; //activating port clock
	delay = SYSCTL_RCGC2_R;
	GPIO_PORTB_AMSEL_R &= (0x00); //disable analog functionality
	GPIO_PORTB_PCTL_R &= (0x00); //select GPIO functionality
	GPIO_PORTB_DIR_R = 0x20;//Enable PB5 as output
	GPIO_PORTB_AFSEL_R &= (0x00);  //Disable alternate function 
	GPIO_PORTB_DEN_R |= 0x20;
}
void PortF_Init(void)
	{ volatile unsigned long delay;
SYSCTL_RCGC2_R |= 0x00000020; // 1) activate clock for Port F
delay = SYSCTL_RCGC2_R; // allow time for clock to start
GPIO_PORTF_LOCK_R = 0x4C4F434B; // 2) unlock GPIO Port F
GPIO_PORTF_CR_R = 0x1F; // allow changes to PF4-0
// only PF0 needs to be unlocked, other bits can't be locked
GPIO_PORTF_AMSEL_R = 0x00; // 3) disable analog on PF
GPIO_PORTF_PCTL_R = 0x00000000; // 4) PCTL GPIO on PF4-0
GPIO_PORTF_DIR_R = 0x0E; // 5) PF4,PF0 in, PF3-1 out
GPIO_PORTF_AFSEL_R = 0x00; // 6) disable alt funct on PF7-0
GPIO_PORTF_DEN_R = 0x1F; // 7) 
}

void SysInt(void) {
	NVIC_ST_CTRL_R = 0;
	NVIC_ST_CURRENT_R = 0;
	NVIC_ST_CTRL_R = 0x00000005;
}

void SysLoad(unsigned long period) {
	NVIC_ST_RELOAD_R = period-1;
	NVIC_ST_CURRENT_R = 0;
	while((NVIC_ST_CTRL_R&0x00010000)==0) {
	}
}

int main(void) {
	PLL_Init();
	PortB_Init();
	PortF_Init();
	SysInt();
	while(1) {
		for(i=56000;i<=184000;i+=10000) {  //0 to 180 deg in portB (PB5), Red LED at PF1
			GPIO_PORTB_DATA_R |=0x20;
			GPIO_PORTF_DATA_R ^= 0x02; // Red LED ON PF1
			SysLoad(i);
			GPIO_PORTB_DATA_R &=~0x20;
			GPIO_PORTF_DATA_R ^=0x02; // Red LED OFF PF1
			SysLoad(1600000-i);
		}
		GPIO_PORTB_DATA_R = 0x00; // 0.5s of zero logic
    SysLoad(16000000);
		SysLoad(16000000);
		SysLoad(8000000);
		for(i=184000;i>=56000;i-=10000) {  //180 to 0 deg
			GPIO_PORTB_DATA_R |=0x20;
			GPIO_PORTF_DATA_R ^=0x08; // Green LED ON PF3
			SysLoad(i);
			GPIO_PORTB_DATA_R &=~0x20;
			GPIO_PORTF_DATA_R ^=0x08; // Green LED OFF PF3
			SysLoad(1600000-i);
		}
		GPIO_PORTB_DATA_R = 0x00; //0.2s of zero logic
    SysLoad(16000000);
	}
}