#include "PLL.h"
#include "tm4c123gh6pm.h"
//Constant declarations to access port registers symbolic names instead of addresses
#include <stdint.h>

unsigned long ADCvalue;
	
void Port_Init(void){ volatile unsigned long delay;
	SYSCTL_RCGC2_R |= 0x00000001; 
	delay = SYSCTL_RCGC2_R; //activate Port A
	GPIO_PORTA_AMSEL_R &= 0x00; //no analog
	GPIO_PORTA_PCTL_R &= ~0x00F00000; //regular function
	GPIO_PORTA_DIR_R |= 0x20; //make PA5 out
	GPIO_PORTA_DR8R_R |= 0x20; //can drive upto 8mA out
	GPIO_PORTA_AFSEL_R &= ~0x20; //disable alt function on PA5
	GPIO_PORTA_DEN_R |= 0x20; //enable digital I/O on PA5
}


//ADC initialization on PE2

void ADC0_InitSWTriggerSeq3_Ch1(void)
{
	volatile unsigned long delay;
	SYSCTL_RCGC2_R |= 0x00000010; //1)activate clock for portE
	delay = SYSCTL_RCGC2_R; //allow time for clock to stabilize
	GPIO_PORTE_DIR_R &= ~0x04; //2)make PE2 input
 	GPIO_PORTE_AFSEL_R |= 0x04; //3)enable alternate funvtion on PE2
	GPIO_PORTE_DEN_R &= ~0x04; //4)disable digital I/O on PE2 since analog i/p
	GPIO_PORTE_AMSEL_R |= 0x04; //5)enable analog function on PE2
	SYSCTL_RCGC0_R |= 0x00010000; //6)activate ADC0
	delay = SYSCTL_RCGC2_R;
	SYSCTL_RCGC0_R &= ~0x00000300; //7)configure for 125K
	ADC0_SSPRI_R = 0x0123; //8)sequencer 3 is highest priority
	ADC0_ACTSS_R &= ~0x0008; //9)disable sample sequencer 3
	ADC0_EMUX_R &= ~0xF000; //10)seq3 os software trigger
	ADC0_SSMUX3_R = (ADC0_SSMUX3_R&0xFFFFFFF0)+1; //11)channel Ain1 (PE2)
	ADC0_SSCTL3_R = 0x0006; //12)no TS0 D0,yes IE0 END0
	ADC0_ACTSS_R |= 0x0008; //13)enable sample sequencer 3
}


//--------ADC0_InSeq3---------
//busy-wait Analog to digital conversion
//input: none
//output: 12-bit result of ADCconversion
unsigned long ADC0_InSeq3(void){
	unsigned long result;
	ADC0_PSSI_R = 0x0008; //1)initiate SS3
	while((ADC0_RIS_R&0x08) == 0){}; //2)wait for conversion done    
	  result = ADC0_SSFIFO3_R&0xFFF; //3)read result
			ADC0_ISC_R = 0x0008; //acknowledge completion
		return result;
	}

	
	void SysLoad(unsigned long period){
		NVIC_ST_RELOAD_R = period-1; //number of counts to wauit
		NVIC_ST_CURRENT_R = 0; //any value written to CURRENT clears
		while((NVIC_ST_CTRL_R&0x00010000)==0){; //wait for count flag
	}
}


void SysFun(void){
	NVIC_ST_CTRL_R = 0; //disable SysTick during setup
	NVIC_ST_CTRL_R = 0x00000005;
}


//MAIN: Mandatory for a C Program to be executable
int main(void){
	PLL_Init(); //8-0MHz
	Port_Init();
	SysFun();
	ADC0_InitSWTriggerSeq3_Ch1();
	
	//Initialize ports, ADC and timers/
	while(1){
		ADCvalue = ADC0_InSeq3();
		if(ADCvalue == 0)
		{
			GPIO_PORTA_DATA_R |= (0x20);
			SysLoad(800000);
			GPIO_PORTA_DATA_R &= ~(0x20);
SysLoad(800000);
		}


if(ADCvalue > 0)
{
GPIO_PORTA_DATA_R |= (0x20);
SysLoad(800000+(ADCvalue*156));
GPIO_PORTA_DATA_R &= ~(0x20);
SysLoad(800000-(ADCvalue*156));
}
	}
}