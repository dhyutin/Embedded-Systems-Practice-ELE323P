#include "tm4c123gh6pm.h"
#include <stdint.h>
#include "PLL.h"
unsigned long i;

void PortD_Init(void) {
    volatile unsigned long delay;
    SYSCTL_RCGC2_R |= 0x00000008; //activating port B clock
    delay = SYSCTL_RCGC2_R;
    GPIO_PORTD_AMSEL_R &= (0x00); //disable analog functionality
    GPIO_PORTD_PCTL_R &= (0x00); //select GPIO functionality
    GPIO_PORTD_DIR_R |= 0x11;   //Enable PD0-3 as output
    GPIO_PORTD_AFSEL_R &= (0x00);  //Disable alternate function 
    GPIO_PORTD_DEN_R |= 0x11;  //Enable PD0-3 for digital output
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
    PortD_Init(); // calling port D initialization 
    SysInt();
    for(i=1; i>0; i++){
        GPIO_PORTD_DATA_R =0x01;
        SysLoad(400000);
        GPIO_PORTD_DATA_R =0x00;
        SysLoad(400000);
    }

}