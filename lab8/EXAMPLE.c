#include "tm4c123gh6pm.h"
#include "PLL.h"
#include<TM4C123.h>
// basic functions defined at end of startup.s
void DisableInterrupts(void); // Disable interrupts
void EnableInterrupts(void);  // Enable interrupts

unsigned char Index;

// Initialize 6-bit DAC 
void DAC_Init(void){
	unsigned long volatile delay;
  SYSCTL_RCGC2_R |= SYSCTL_RCGC2_GPIOB; // activate port B
  delay = SYSCTL_RCGC2_R;    // allow time to finish activating
	GPIO_PORTB_AMSEL_R = 0x00;	// no analog 
	GPIO_PORTB_PCTL_R =0x00;// regular function
	GPIO_PORTB_DIR_R |= 0x3F;	// make PB5-0 out
	GPIO_PORTB_AFSEL_R = 0x00;	// disable alt funct on PB5-0
	GPIO_PORTB_DEN_R |= 0x3F;
	GPIO_PORTB_DR8R_R|=0x3F;
}

// Initialize piano key inputs
void PortE_Init(void){ 
  volatile unsigned long  delay;
  SYSCTL_RCGC2_R |= 0x00000010;     // 1) activate clock for Port E
  delay = SYSCTL_RCGC2_R;           // allow time for clock to start
  GPIO_PORTE_AMSEL_R = 0x00;        // 2) disable analog on Port E
  GPIO_PORTE_PCTL_R = 0x00000000;   // 3) PCTL GPIO on PE0
  GPIO_PORTE_DIR_R = 0x00;          // 4) PE0 in
  GPIO_PORTE_AFSEL_R = 0x00;        // 5) disable alt funct on PE0
  GPIO_PORTE_DEN_R = 0x01;  				// 6) enable digital on PE0
}

const unsigned char SineWave[64]={32,35,38,41,44,47,49,52,54,56,58,				
  59,61,62,62,63,63,63,62,62,61,59,				
  58,56,54,52,49,47,44,41,38,35,				
  32,29,26,23,20,17,15,12,10,8,				
  6,5,3,2,2,1,1,1,2,2,3,				
  5,6,8,10,12,15,17,20,23,26,29};	

// Initialize Systick periodic interrupts	
void Sys_Init(void){
	Index=0;
	NVIC_ST_CTRL_R = 0;
	NVIC_ST_CURRENT_R = 0;      // any write to current clears it
  NVIC_SYS_PRI3_R = (NVIC_SYS_PRI3_R&0x00FFFFFF)|0x20000000; // priority 1      
  NVIC_ST_CTRL_R = 0x07;  // enable SysTick with core clock and interrupts 
}

void Sys_Load(unsigned long period){
// this routine sets the RELOAD and starts SysTick
  NVIC_ST_RELOAD_R = period-1;// reload value
  
}

void SysTick_Handler(void){
  GPIO_PORTB_DATA_R =SineWave[Index];
	Index = (Index+1)&0x3F;
}

int main(void){ 
  PLL_Init(); //set system clock to 80 MHz
	DAC_Init(); //PortB used for DAC 
	PortE_Init(); // PortE used for piano keys     
  Sys_Init(); // initialize SysTick timer 

	DisableInterrupts();
  
  while(1){                
			if((GPIO_PORTE_DATA_R&0x0F)==0x01){
			EnableInterrupts();
			Sys_Load(1194);
			}
			
			else{
				GPIO_PORTB_DATA_R = 0x00;
				DisableInterrupts();
		  }	
	}          
}