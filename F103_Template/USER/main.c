#include "stm32f10x.h"
#include  "led.h"
#include "delay.h"
#include "PCA9685.h"

int main(void)
{
	int led=0;
	delay_init();
	LED_Init();
	PCA9685_Init(60,180);
	while(1)
	{
		PCA9685_setPWM(0,0,2048);
		if(led) GPIO_SetBits(GPIOC, GPIO_Pin_13);
		else  GPIO_ResetBits(GPIOC, GPIO_Pin_13);
		led = !led;
		delay_ms(500);
	}
}

