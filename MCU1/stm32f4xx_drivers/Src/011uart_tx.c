/*
 * 011uart_tx.c
 *
 *  Created on: Oct 14, 2022
 *      Author: ebraun
 */

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "stm32f407xx.h"

char msg[1024] = "This is a test message...\n\r";

USART_Handle_t usart2_handle;

void USART2_Init(void){
	usart2_handle.pUSARTx = USART2;
	usart2_handle.USART_Config.baudRate = USART_STD_BAUD_9600;
	usart2_handle.USART_Config.HWFlowControl = USART_HW_FLOW_CTRL_NONE;
	usart2_handle.USART_Config.mode = USART_MODE_TXRX;
	usart2_handle.USART_Config.noOfStopBits = USART_STOPBITS_1;
	usart2_handle.USART_Config.wordLength = USART_WORDLEN_8BITS;
	usart2_handle.USART_Config.parityControl = USART_PARITY_DISABLE;
	USART_Init(&usart2_handle);
}

void USART2_GPIOInit(void){
	GPIO_Handle_t usart_gpios;

	usart_gpios.pGPIOx = GPIOA;
	usart_gpios.GPIO_PinConfig.GPIO_PinMode = GPIO_MODE_ALTFN;
	usart_gpios.GPIO_PinConfig.GPIO_PinOPType = GPIO_OP_TYPE_PP;
	usart_gpios.GPIO_PinConfig.GPIO_PinPuPdControl = GPIO_PU;
	usart_gpios.GPIO_PinConfig.GPIO_PinSpeed = GPIO_SPEED_FAST;
	usart_gpios.GPIO_PinConfig.GPIO_PinAltFunMode = 7;

	// USART2 TX
	usart_gpios.GPIO_PinConfig.GPIO_PinNumber = GPIO_PIN_NO_2;
	GPIO_Init(&usart_gpios);

	// USART2 RX
	usart_gpios.GPIO_PinConfig.GPIO_PinNumber = GPIO_PIN_NO_3;
	GPIO_Init(&usart_gpios);
}

int main(void){
	USART2_GPIOInit();

	USART2_Init();

	USART_PeripheralControl(USART2, ENABLE);

	USART_SendData(&usart2_handle, (uint8_t*)msg, strlen(msg));

	while(1);

	return 0;
}
