#include "tm4c123gh6pm.h"
#include "PLL.h"
#include<TM4C123.h>
#include<stdint.h>

void DisableInterrupts(void); 
void EnableInterrupts(void);  

unsigned char Index;


void DAC_Init(void){
	unsigned long volatile delay;
  SYSCTL_RCGC2_R |= 0x00000008; 
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
  GPIO_PORTE_DEN_R = 0x01;  				
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
	NVIC_ST_CURRENT_R = 0;      
  NVIC_SYS_PRI3_R = (NVIC_SYS_PRI3_R&0x00FFFFFF)|0x20000000;      
  NVIC_ST_CTRL_R = 0x07;  
}

void Sys_Load(unsigned long period){

  NVIC_ST_RELOAD_R = period-1;
	//while((NVIC_SYS_PRI3_R&0x08) == 0)
	//{}
  
}

void SysTick_Handler(void){
  GPIO_PORTE_DATA_R = SineWave[Index];
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
			if((GPIO_PORTE_DATA_R&0x0F)==0x02){
			EnableInterrupts();
			Sys_Load(1420);
			}
			else
			{
				GPIO_PORTD_DATA_R = 0X00;
				DisableInterrupts();
			}
	}          
}