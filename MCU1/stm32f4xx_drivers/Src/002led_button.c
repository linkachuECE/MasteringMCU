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
	for(uint32_t i = 0; i < 500000; i++);
}

int main(void)
{
	GPIO_Handle_t LED4;
	LED4.pGPIOx = GPIOD;
	LED4.GPIO_PinConfig.GPIO_PinNumber = GPIO_PIN_NO_12;
	LED4.GPIO_PinConfig.GPIO_PinMode = GPIO_MODE_OUT;
	LED4.GPIO_PinConfig.GPIO_PinSpeed = GPIO_SPEED_FAST;
	LED4.GPIO_PinConfig.GPIO_PinPuPdControl = GPIO_NO_PUPD;
	LED4.GPIO_PinConfig.GPIO_PinOPType = GPIO_OP_TYPE_PP;

	GPIO_Handle_t PB1;
	PB1.pGPIOx = GPIOA;
	PB1.GPIO_PinConfig.GPIO_PinNumber = GPIO_PIN_NO_0;
	PB1.GPIO_PinConfig.GPIO_PinMode = GPIO_MODE_IN;
	PB1.GPIO_PinConfig.GPIO_PinSpeed = GPIO_SPEED_FAST;
	PB1.GPIO_PinConfig.GPIO_PinPuPdControl = GPIO_NO_PUPD;

	GPIO_PeriClockControl(GPIOD, ENABLE);
	GPIO_PeriClockControl(GPIOA, ENABLE);
	GPIO_Init(&LED4);
	GPIO_Init(&PB1);


	while(1){
		if(GPIO_ReadFromInputPin(GPIOA, GPIO_PIN_NO_0)){
			GPIO_ToggleOutputPin(GPIOD, GPIO_PIN_NO_12);
			delay();
		}
	}
}
