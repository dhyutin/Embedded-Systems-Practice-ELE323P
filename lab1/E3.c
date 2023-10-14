#include "tm4c123gh6pm.h"
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

int main(void){
PortF_Init(); // make PF1 out (PF1 built-in LED)
while(1){
	Switch=GPIO_PORTF_DATA_R&0x11;
if(Switch==0x11)
GPIO_PORTF_DATA_R = 0x02; //red
else if(Switch==0x01)
GPIO_PORTF_DATA_R = 0x08;  //green
else if(Switch==0x10)
GPIO_PORTF_DATA_R = 0x04;  //blue
else if(Switch==0x00)
GPIO_PORTF_DATA_R = 0x0E; //white
else 
GPIO_PORTF_DATA_R = 0x00;

Delay();
}
}
// Color LED(s) PortF
// dark --- 0
// red R-- 0x02
// blue --B 0x04
// green -G- 0x08
// yellow RG- 0x0A
// sky blue -GB 0x0C
// white RGB 0x0E
// pink R-B 0x06