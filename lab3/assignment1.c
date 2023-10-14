#include "tm4c123gh6pm.h"
#include <stdint.h>
#include "PLL.h"

unsigned long i;
unsigned long j;

void PortA_Init(void) {
	volatile unsigned long delay;
	SYSCTL_RCGC2_R |= 0x00000001; //activating port clock
	delay = SYSCTL_RCGC2_R;
	GPIO_PORTA_AMSEL_R &= (0x00); //disable analog functionality
	GPIO_PORTA_PCTL_R &= (0x00); //select GPIO functionality
	GPIO_PORTA_DIR_R |= 0x08;   //Enable PA3 as output
	GPIO_PORTA_AFSEL_R &= (0x00);  //Disable alternate function 
	GPIO_PORTA_DEN_R |= 0x08; 
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
	PortA_Init();
	SysInt();
	while(1) {
		for(i=56000;i<=152000;i+=10000) {  //0 to 135 deg
			GPIO_PORTA_DATA_R |=0x08;
			SysLoad(i);
			GPIO_PORTA_DATA_R &=~0x08;
			for(j=0;j<=100;j++)
			SysLoad(1600000-i);
		}
		for(i=152000;i>=56000;i-=10000) {  //135 to 0 deg
			GPIO_PORTA_DATA_R |=0x08;
			SysLoad(i);
			GPIO_PORTA_DATA_R &=~0x08;
			for(j=0;j<=100;j++)
			SysLoad(1600000-i);
		}
	}
}