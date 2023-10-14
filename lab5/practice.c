#include "tm4c123gh6pm.h"
#include <stdint.h>
#include "PLL.h"

unsigned long i;
unsigned long Switch;
void PortA_Init(void) {
	volatile unsigned long delay;
	SYSCTL_RCGC2_R |= 0x00000001; //activating port clock
	delay = SYSCTL_RCGC2_R;
	GPIO_PORTA_AMSEL_R &= (0x00); //disable analog functionality
	GPIO_PORTA_PCTL_R &= (0x00); //select GPIO functionality
	GPIO_PORTA_DIR_R |= 0x88;   //Enable PA as output
	GPIO_PORTA_AFSEL_R &= (0x00);  //Disable alternate function 
	GPIO_PORTA_DEN_R |= 0x88; 
}

void PortF_Init(void) {
	volatile unsigned long delay;
	SYSCTL_RCGC2_R |= 0x00000020; //activating port clock
	delay = SYSCTL_RCGC2_R;
	GPIO_PORTF_AMSEL_R &= (0x00); //disable analog functionality
	GPIO_PORTF_PCTL_R &= (0x00); //select GPIO functionality
	GPIO_PORTF_DIR_R |= 0x11;   //Enable PA as output
	GPIO_PORTF_AFSEL_R &= (0x00);  //Disable alternate function 
	GPIO_PORTF_DEN_R |= 0x11; 
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
	PortF_Init();
	SysInt();
	while(1) {
		Switch = GPIO_PORTF_DATA_R&0x11;
		if(Switch==0x11)
		{
				GPIO_PORTA_DATA_R = 0x00;
				SysLoad(800000);
		}
		else if(Switch==0x01)
		{
				GPIO_PORTA_DATA_R = 0x08;
				SysLoad(400000);
				GPIO_PORTA_DATA_R = 0x00;
				SysLoad(400000);
		}
		else if(Switch==0x10)
		{
				GPIO_PORTA_DATA_R = 0x80;
				SysLoad(600000);
				GPIO_PORTA_DATA_R = 0x00;
				SysLoad(200000);
		}
		else if(Switch==0x00)
		{
				GPIO_PORTA_DATA_R = 0x08;
				SysLoad(720000);
				GPIO_PORTA_DATA_R = 0x00;
				SysLoad(80000);
		}
	
	}
}