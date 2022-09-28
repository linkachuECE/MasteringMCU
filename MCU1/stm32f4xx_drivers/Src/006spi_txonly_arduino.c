/*
 * 005SPI_sendData.c
 *
 *  Created on: Sep 10, 2022
 *      Author: linkachu
 */
#include "stm32f407xx.h"
#include <string.h>
#include <stdio.h>

void delay(uint16_t ms){
	for(uint32_t i = 0; i < (ms*1000); i++);
}

typedef struct {
	GPIO_Handle_t MOSI;
	GPIO_Handle_t MISO;
	GPIO_Handle_t SCK;
	GPIO_Handle_t NSS;
} SPI_GPIO_Pins_t;

void SPI2_GPIO_Inits(SPI_GPIO_Pins_t* SPIPort){
	SPIPort->MOSI.pGPIOx = GPIOB;
	SPIPort->MOSI.GPIO_PinConfig.GPIO_PinNumber = GPIO_PIN_NO_15;
	SPIPort->MOSI.GPIO_PinConfig.GPIO_PinMode = GPIO_MODE_ALTFN;
	SPIPort->MOSI.GPIO_PinConfig.GPIO_PinSpeed = GPIO_SPEED_HIGH;
	SPIPort->MOSI.GPIO_PinConfig.GPIO_PinPuPdControl = GPIO_NO_PUPD;
	SPIPort->MOSI.GPIO_PinConfig.GPIO_PinOPType = GPIO_OP_TYPE_PP;
	SPIPort->MOSI.GPIO_PinConfig.GPIO_PinAltFunMode = 5;

	SPIPort->MISO.pGPIOx = GPIOB;
	SPIPort->MISO.GPIO_PinConfig.GPIO_PinNumber = GPIO_PIN_NO_14;
	SPIPort->MISO.GPIO_PinConfig.GPIO_PinMode = GPIO_MODE_ALTFN;
	SPIPort->MISO.GPIO_PinConfig.GPIO_PinSpeed = GPIO_SPEED_HIGH;
	SPIPort->MISO.GPIO_PinConfig.GPIO_PinPuPdControl = GPIO_NO_PUPD;
	SPIPort->MISO.GPIO_PinConfig.GPIO_PinOPType = GPIO_OP_TYPE_PP;
	SPIPort->MISO.GPIO_PinConfig.GPIO_PinAltFunMode = 5;

	SPIPort->SCK.pGPIOx = GPIOB;
	SPIPort->SCK.GPIO_PinConfig.GPIO_PinNumber = GPIO_PIN_NO_13;
	SPIPort->SCK.GPIO_PinConfig.GPIO_PinMode = GPIO_MODE_ALTFN;
	SPIPort->SCK.GPIO_PinConfig.GPIO_PinSpeed = GPIO_SPEED_HIGH;
	SPIPort->SCK.GPIO_PinConfig.GPIO_PinPuPdControl = GPIO_NO_PUPD;
	SPIPort->SCK.GPIO_PinConfig.GPIO_PinOPType = GPIO_OP_TYPE_PP;
	SPIPort->SCK.GPIO_PinConfig.GPIO_PinAltFunMode = 5;

	SPIPort->NSS.pGPIOx = GPIOB;
	SPIPort->NSS.GPIO_PinConfig.GPIO_PinNumber = GPIO_PIN_NO_12;
	SPIPort->NSS.GPIO_PinConfig.GPIO_PinMode = GPIO_MODE_ALTFN;
	SPIPort->NSS.GPIO_PinConfig.GPIO_PinSpeed = GPIO_SPEED_HIGH;
	SPIPort->NSS.GPIO_PinConfig.GPIO_PinPuPdControl = GPIO_NO_PUPD;
	SPIPort->NSS.GPIO_PinConfig.GPIO_PinOPType = GPIO_OP_TYPE_PP;
	SPIPort->NSS.GPIO_PinConfig.GPIO_PinAltFunMode = 5;

	GPIO_Init(&(SPIPort->MOSI));
	GPIO_Init(&(SPIPort->MISO));
	GPIO_Init(&(SPIPort->SCK));
	GPIO_Init(&(SPIPort->NSS));
}

void SPI2_Init(SPI_Handle_t* SPIDevice){
	SPIDevice->pSPIx = SPI2;
	SPIDevice->SPIConfig.BusConfig = SPI_BUS_CONFIG_FD;
	SPIDevice->SPIConfig.DeviceMode = SPI_DEVICE_MODE_MASTER;
	SPIDevice->SPIConfig.SclkSpeed = SPI_SCLK_SPEED_DIV16;
	SPIDevice->SPIConfig.DFF = SPI_DFF_8BITS;
	SPIDevice->SPIConfig.CPOL = SPI_CPOL_LOW;
	SPIDevice->SPIConfig.CPHA = SPI_CPHA_FIRST;
	SPIDevice->SPIConfig.SSM = SPI_SSM_HW;
	SPIDevice->SPIConfig.FrameFormat = SPI_FRAME_FORMAT_MSBFIRST;

	SPI_Init(SPIDevice);
}

void USRBTN_Init(GPIO_Handle_t* USRPB){
	USRPB->pGPIOx = GPIOA;
	USRPB->GPIO_PinConfig.GPIO_PinNumber = GPIO_PIN_NO_0;
	USRPB->GPIO_PinConfig.GPIO_PinMode = GPIO_MODE_IT_RT;
	USRPB->GPIO_PinConfig.GPIO_PinSpeed = GPIO_SPEED_FAST;
	USRPB->GPIO_PinConfig.GPIO_PinPuPdControl = GPIO_NO_PUPD;

	GPIO_Init(USRPB);
}

//Global variables for interrupt
SPI_GPIO_Pins_t spi2Pins;
SPI_Handle_t mySPIDevice;

void sendMessage(){
	char* myString = "I want to marry Janiyah";

	SPI_PeripheralControl(mySPIDevice.pSPIx, ENABLE);

	uint8_t messageLen = strlen(myString);

	// First send the length
	SPI_SendData(mySPIDevice.pSPIx, &messageLen, sizeof(uint8_t));
	delay(1);

	// Then send the message
	SPI_SendData(mySPIDevice.pSPIx, (uint8_t*)myString, messageLen);

	// Wait for busy flag to be reset
	while(SPI_GetFlagStatus(SPI2, SPI_BSY_FLAG));

	SPI_PeripheralControl(mySPIDevice.pSPIx, DISABLE);
}

int main(void){

	SPI2_GPIO_Inits(&spi2Pins);
	SPI2_Init(&mySPIDevice);

	// Enable SSOE (Slave Select Output Enable)
	// This allows the NSS to be toggled every time
	// we use SPI_PeripheralControl
	SPI_SSOEControl(SPI2, ENABLE);

	// Initialize user button
	GPIO_Handle_t USRPB;
	memset(&USRPB, 0, sizeof(USRPB));
	USRBTN_Init(&USRPB);

	// Create interrupt for button:
	GPIO_IRQInterruptConfig(IRQ_NO_EXTI0, 1, ENABLE);

	while(1);
}

void EXTI0_IRQHandler(void){
	delay(200);
	GPIO_IRQHandling(GPIO_PIN_NO_0);
	sendMessage();
}
