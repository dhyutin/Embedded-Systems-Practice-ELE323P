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
	GPIO_PORTB_DIR_R |= 0x10;   //Enable PB4 as output
	GPIO_PORTB_AFSEL_R &= (0x00);  //Disable alternate function 
	GPIO_PORTB_DEN_R |= 0x10; 
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
	SysInt();
	while(1) {
		for(i=56000;i<=184000;i+=10000) {  //0 to 180 deg
			GPIO_PORTB_DATA_R |=0x10;
			SysLoad(i);
			GPIO_PORTB_DATA_R &=~0x10;
			SysLoad(1600000-i);
		}
		for( i = 184000; i >= 56000; i-=10000) { //180 to 0 deg
			GPIO_PORTB_DATA_R |=0x10;
			SysLoad(i);
			GPIO_PORTB_DATA_R &=~0x10;
			SysLoad(1600000-i);
		}			

	}
}