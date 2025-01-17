 /**
 ******************************************************************************
 * @file           : main.c
 * @author         : Auto-generated by STM32CubeIDE
 * @brief          : Main program body
 ******************************************************************************
 * @attention
 *
 * Copyright (c) 2022 STMicroelectronics.
 * All rights reserved.
 *
 * This software is licensed under terms that can be found in the LICENSE file
 * in the root directory of this software component.
 * If no LICENSE file comes with this software, it is provided AS-IS.
 *
 ******************************************************************************
 */

#include <stdint.h>
#include "stm32f407xx.h"

#include <unistd.h>
#include <stdio.h>

void delay(void){
	for(uint32_t i = 0; i < 10000; i++);
}

int main(void)
{
	GPIO_Handle_t EXTLED;
	EXTLED.pGPIOx = GPIOA;
	EXTLED.GPIO_PinConfig.GPIO_PinNumber = GPIO_PIN_NO_8;
	EXTLED.GPIO_PinConfig.GPIO_PinMode = GPIO_MODE_OUT;
	EXTLED.GPIO_PinConfig.GPIO_PinSpeed = GPIO_SPEED_FAST;
	EXTLED.GPIO_PinConfig.GPIO_PinPuPdControl = GPIO_NO_PUPD;
	EXTLED.GPIO_PinConfig.GPIO_PinOPType = GPIO_OP_TYPE_PP;

	GPIO_Handle_t EXTPB;
	EXTPB.pGPIOx = GPIOB;
	EXTPB.GPIO_PinConfig.GPIO_PinNumber = GPIO_PIN_NO_12;
	EXTPB.GPIO_PinConfig.GPIO_PinMode = GPIO_MODE_IN;
	EXTPB.GPIO_PinConfig.GPIO_PinSpeed = GPIO_SPEED_FAST;
	EXTPB.GPIO_PinConfig.GPIO_PinPuPdControl = GPIO_NO_PUPD;

	GPIO_PeriClockControl(GPIOA, ENABLE);
	GPIO_PeriClockControl(GPIOB, ENABLE);
	GPIO_Init(&EXTLED);
	GPIO_Init(&EXTPB);


	while(1){
		if(GPIO_ReadFromInputPin(GPIOB, GPIO_PIN_NO_12)){
			delay();
			GPIO_ToggleOutputPin(GPIOA, GPIO_PIN_NO_8);
		}
	}
}
