#include "tm4c123gh6pm.h"
#include "PLL.h"
#include<TM4C123.h>
#include<stdint.h>

void DisableInterrupts(void); 
void EnableInterrupts(void);  

unsigned char Index = 0;


void DAC_Init(void){
	unsigned long volatile delay;
  SYSCTL_RCGC2_R |= SYSCTL_RCGC2_GPIOD; 
  delay = SYSCTL_RCGC2_R;    
	GPIO_PORTD_AMSEL_R = 0x00;	
	GPIO_PORTD_PCTL_R =0x00;
	GPIO_PORTD_DIR_R |= 0x3F;
	GPIO_PORTD_AFSEL_R = 0x00;	
	GPIO_PORTD_DEN_R |= 0x3F;
	GPIO_PORTD_DR8R_R|=0x3F;
}


void PortE_Init(void){ 
  volatile unsigned long  delay;
  SYSCTL_RCGC2_R |= 0x00000010;     
  delay = SYSCTL_RCGC2_R;           
  GPIO_PORTE_AMSEL_R = 0x00;        
  GPIO_PORTE_PCTL_R = 0x00000000;   
  GPIO_PORTE_DIR_R = 0x00;          
  GPIO_PORTE_AFSEL_R = 0x00;        
  GPIO_PORTE_DEN_R = 0x00;  				
}

const unsigned char SineWave[64]={32,34,36,39,42,45,48,51,54,55,
55,55,58,60,61,63,63,61,60,58,55,55,55,54,51,48,45,42,39,36,34,32,31,29,26,24,21,
19,19,18,18,16,14,12,10,8,4,2,1,1,2,3,5,6,10,15,18,22,25,28,30,31,32,32};

// Initialize Systick periodic interrupts	
void Sys_Init(void){
	//Index=0;
	NVIC_ST_CTRL_R = 0;
	NVIC_ST_CURRENT_R = 0;      
  NVIC_SYS_PRI3_R = (NVIC_SYS_PRI3_R&0x00FFFFFF)|0x20000000;      
  NVIC_ST_CTRL_R = 0x07;  
}

void Sys_Load(unsigned long period){

  NVIC_ST_RELOAD_R = period-1;
	while((NVIC_SYS_PRI3_R&0x08) == 0)
	{}
  NVIC_ST_CURRENT_R = 0;
		NVIC_ST_CTRL_R = 0X07;
}

void SysTick_Handler(void){
  GPIO_PORTD_DATA_R = SineWave[Index];
	Index = (Index+1)&0x3F;
}

int main(void){ 
  PLL_Init(); 
	DAC_Init(); 
	PortE_Init();     
  Sys_Init(); 

	DisableInterrupts();
  
  while(1){  
      // A Key		
			if((GPIO_PORTD_DATA_R&0x0F)==0x00000002){
			EnableInterrupts();
			Sys_Load(1420);
			}
			else
			{
				GPIO_PORTD_DATA_R = 0X00;
				DisableInterrupts();
			}
			DisableInterrupts();
	}          
}