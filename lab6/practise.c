#include "PLL.h"
#include "tm4c123gh6pm.h"
unsigned long Switch;
void PortF_Init(void){ volatile unsigned long delay;
  SYSCTL_RCGC2_R |= 0x00000020;     // 1) activate clock for Port F
  delay = SYSCTL_RCGC2_R;           // allow time for clock to start
  GPIO_PORTF_LOCK_R = 0x4C4F434B;   // 2) unlock GPIO Port F
  GPIO_PORTF_CR_R = 0x1F;           // allow changes to PF4-0
  // only PF0 needs to be unlocked, other bits can't be locked
  GPIO_PORTF_AMSEL_R = 0x00;        // 3) disable analog on PF
  GPIO_PORTF_PCTL_R = 0x00000000;   // 4) PCTL GPIO on PF4-0
  GPIO_PORTF_DIR_R = 0x0E;          // 5) PF4,PF0 in, PF3-1 out
  GPIO_PORTF_AFSEL_R = 0x00;        // 6) disable alt funct on PF7-0
  GPIO_PORTF_PUR_R = 0x11;          // enable pull-up on PF0 and PF4
  GPIO_PORTF_DEN_R = 0x1F;          // 7) enable digital I/O on PF4-0
}


void PortB_Init(void){ volatile unsigned long delay;
  SYSCTL_RCGC2_R |= 0x02;          // 1) activate Port D
  delay = SYSCTL_RCGC2_R;          // allow time for clock to stabilize                                  
  GPIO_PORTB_AMSEL_R &= 0x00;     // 3) disable analog functionality on PD3-0
  GPIO_PORTB_PCTL_R &= ~0x0000FFFF;  // 4) configure PD0-3 as GPIO
  GPIO_PORTB_DIR_R |= 0x0F;        // 5) make PD0-3 out
  GPIO_PORTB_AFSEL_R &= 0x00;     // 6) disable alt funct on PD0-3
  GPIO_PORTB_DEN_R |= 0x0F;        // 7) enable digital I/O on PD0-3
}

void SysTick_Init(unsigned long period){
  NVIC_ST_CTRL_R = 0;         // disable SysTick during setup
  NVIC_ST_RELOAD_R = period-1;// reload value
  NVIC_ST_CURRENT_R = 0;      // any write to current clears it
  NVIC_SYS_PRI3_R = (NVIC_SYS_PRI3_R&0x00FFFFFF); // priority 0
  NVIC_ST_CTRL_R = 0x07;   // enable SysTick with clock and interrupts
}

void SysTick_Handler(void){
  GPIO_PORTB_DATA_R ^= 0x01;       // toggle PB0 after 10msec
}

int main(void){
    PLL_Init(); // 80 MHz clock
    SysTick_Init(800000); //Initialize systick with a reload of 10msec
    PortF_Init();   // Initialize the LEDs and Switches of Port F
    PortB_Init(); // Initialize PD0 - 3 as digital GPIO output

    while(1)  
    {
        Switch = GPIO_PORTF_DATA_R&0x11;
        if(Switch==0x11)
        {
                GPIO_PORTF_DATA_R = 0x00;
        }
        else if(Switch==0x01)
        {
                GPIO_PORTF_DATA_R = 0x02;
        }
        else if(Switch==0x10)
        {
                GPIO_PORTF_DATA_R = 0x04;
        }
        else if(Switch==0x00)
        {
                GPIO_PORTF_DATA_R = 0x08;
        }
    }
}