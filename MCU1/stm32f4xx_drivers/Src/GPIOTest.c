#include <stdint.h>
#include "stm32f407xx.h"

#include <unistd.h>
#include <stdio.h>

void delay(void){
	for(uint32_t i = 0; i < 100000; i++);
}

int main(void)
{
	GPIO_Handle_t B13;
	B13.pGPIOx = GPIOB;
	B13.GPIO_PinConfig.GPIO_PinNumber = GPIO_PIN_NO_13;
	B13.GPIO_PinConfig.GPIO_PinMode = GPIO_MODE_OUT;
	B13.GPIO_PinConfig.GPIO_PinSpeed = GPIO_SPEED_FAST;
	B13.GPIO_PinConfig.GPIO_PinPuPdControl = GPIO_NO_PUPD;
	B13.GPIO_PinConfig.GPIO_PinOPType = GPIO_OP_TYPE_PP;

	GPIO_Handle_t B15;
	B15.pGPIOx = GPIOB;
	B15.GPIO_PinConfig.GPIO_PinNumber = GPIO_PIN_NO_15;
	B15.GPIO_PinConfig.GPIO_PinMode = GPIO_MODE_OUT;
	B15.GPIO_PinConfig.GPIO_PinSpeed = GPIO_SPEED_FAST;
	B15.GPIO_PinConfig.GPIO_PinPuPdControl = GPIO_NO_PUPD;
	B15.GPIO_PinConfig.GPIO_PinOPType = GPIO_OP_TYPE_PP;

	GPIO_PeriClockControl(GPIOD, ENABLE);
	GPIO_Init(&B13);
	GPIO_Init(&B15);

	while(1){
		GPIO_ToggleOutputPin(GPIOB, GPIO_PIN_NO_13);
		GPIO_ToggleOutputPin(GPIOB, GPIO_PIN_NO_15);
		delay();
	}
}
