#include "tm4c123gh6pm.h"
#include <stdint.h>
#include "PLL.h"

 void PortF_Init(void)
{
 unsigned long delay;
	SYSCTL_RCGC2_R |= 0x20;// clock for Port F
	delay = SYSCTL_RCGC2_R;// wait 3-5 bus cycles 	
	GPIO_PORTF_LOCK_R = 0x4C4F434B;//unlock GPIOPortF
	GPIO_PORTF_CR_R = 0x1F; // allow changes to PF4-0
	// only PF0 to be unlocked, other bits can't be
	GPIO_PORTF_AMSEL_R = 0x00;// disable analog
	GPIO_PORTF_PCTL_R = 0x00;// bits for PF4-0
	GPIO_PORTF_DIR_R = 0x0E;// PF4,0 in, PF3,1 out
	GPIO_PORTF_AFSEL_R = 0x00;//disable alt func
	GPIO_PORTF_PUR_R = 0x11;// enable pullup on PF0,4 
	GPIO_PORTF_DEN_R = 0x1F;// enable digital I/O
}
 
void SysTick_Init(void){
NVIC_ST_CTRL_R = 0; // 1) disable SysTick during setup
//NVIC_ST_RELOAD_R = 0x00FFFFFF; // 2) maximum reload value
NVIC_ST_CURRENT_R = 0; // 3) any write to current clears it
NVIC_ST_CTRL_R = 0x00000005; // 4) enable SysTick with core clock
}
void SysLoad(unsigned long period){
NVIC_ST_RELOAD_R = period -1; // setting desired reload value
NVIC_ST_CURRENT_R = 0; // any value written to current clears it
while((NVIC_ST_CTRL_R&0x00010000) == 0){//wait for count flag
}
}

 int main(void)
 {
 PortF_Init();
	SysTick_Init();
 
 while(1)
 {

 GPIO_PORTF_DATA_R = 0x08;
 SysLoad(400000);	 
 GPIO_PORTF_DATA_R = 0x04;
 SysLoad(600000);
 GPIO_PORTF_DATA_R = 0x02;
 SysLoad(800000);
 GPIO_PORTF_DATA_R = 0x06;
 SysLoad(1000000);
}}