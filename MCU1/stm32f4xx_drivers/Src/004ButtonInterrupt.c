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
#include <string.h>

void EXTI0_IRQHandler(void);

void delay(void){
	for(uint32_t i = 0; i < 250000; i++);
}

int main(void)
{
	GPIO_Handle_t LED1;
	memset(&LED1, 0, sizeof(LED1));
	LED1.pGPIOx = GPIOD;
	LED1.GPIO_PinConfig.GPIO_PinNumber = GPIO_PIN_NO_12;
	LED1.GPIO_PinConfig.GPIO_PinMode = GPIO_MODE_OUT;
	LED1.GPIO_PinConfig.GPIO_PinSpeed = GPIO_SPEED_FAST;
	LED1.GPIO_PinConfig.GPIO_PinPuPdControl = GPIO_NO_PUPD;

	GPIO_Handle_t USRPB;
	memset(&USRPB, 0, sizeof(USRPB));
	USRPB.pGPIOx = GPIOA;
	USRPB.GPIO_PinConfig.GPIO_PinNumber = GPIO_PIN_NO_0;
	USRPB.GPIO_PinConfig.GPIO_PinMode = GPIO_MODE_IT_RT;
	USRPB.GPIO_PinConfig.GPIO_PinSpeed = GPIO_SPEED_FAST;
	USRPB.GPIO_PinConfig.GPIO_PinPuPdControl = GPIO_NO_PUPD;

	GPIO_PeriClockControl(GPIOD, ENABLE);
	GPIO_PeriClockControl(GPIOA, ENABLE);
	GPIO_Init(&LED1);
	GPIO_Init(&USRPB);

	GPIO_IRQInterruptConfig(IRQ_NO_EXTI0, 1, ENABLE);

	while(1)
		;
}

void EXTI0_IRQHandler(void){
	delay();
	GPIO_IRQHandling(GPIO_PIN_NO_0);
	GPIO_ToggleOutputPin(GPIOD, GPIO_PIN_NO_12);
}
