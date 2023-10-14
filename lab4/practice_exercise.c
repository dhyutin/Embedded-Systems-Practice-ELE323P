#include "PLL.h"
#include "tm4c123gh6pm.h"
#include <TM4C123.h>


void PortE_Init(void)
{
volatile unsigned long delay;
	SYSCTL_RCGC2_R |= 0X10;
	delay = SYSCTL_RCGC2_R;
	GPIO_PORTE_AMSEL_R |= 0X0F;
  GPIO_PORTE_PCTL_R &= 0X0000FFFF;
	GPIO_PORTE_DIR_R |= 0X0F;
	GPIO_PORTE_AFSEL_R |= 0X0F;	
	GPIO_PORTE_DEN_R |= 0X0F;
}

void SysInit(void){
	NVIC_ST_CTRL_R = 0;
	NVIC_ST_CURRENT_R = 0;
	NVIC_ST_CTRL_R = 0X00000005;
}

void SysLoad(unsigned long period){
	NVIC_ST_RELOAD_R = period-1;
	NVIC_ST_CURRENT_R = 0;
	while((NVIC_ST_CTRL_R & 0X00010000)==0){
	}
}

int main(void)
	{
	int i;
PLL_Init();
SysInit();
PortE_Init();

for ( i=0; i<25; i++)
{
		GPIO_PORTE_DATA_R = (0x05);
		SysLoad(800000);

		GPIO_PORTE_DATA_R = (0x06);
		SysLoad(800000);

		
		GPIO_PORTE_DATA_R = (0x0A);
		SysLoad(800000);

		GPIO_PORTE_DATA_R = (0x09);
		SysLoad(800000);
      }

    GPIO_PORTE_DATA_R = 0x00; // 1s of zero logic
    SysLoad(16000000);
		SysLoad(16000000);
    SysLoad(16000000);
    SysLoad(16000000);
    SysLoad(16000000);

   for ( i=25; i<50; i++)
{
		GPIO_PORTE_DATA_R = (0x09);
		SysLoad(800000);

		GPIO_PORTE_DATA_R = (0x0A);
		SysLoad(800000);

		
		GPIO_PORTE_DATA_R = (0x06);
		SysLoad(800000);

		GPIO_PORTE_DATA_R = (0x05);
		SysLoad(800000);
      }
		}