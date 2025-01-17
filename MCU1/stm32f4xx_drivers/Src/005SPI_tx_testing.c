/*
 * 005SPI_sendData.c
 *
 *  Created on: Sep 10, 2022
 *      Author: linkachu
 */
#include "stm32f407xx.h"
#include <string.h>
#include <stdio.h>

void delay(void){
	for(uint32_t i = 0; i < 20000; i++);
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
	SPIDevice->SPIConfig.DeviceMode = SPI_DEVICE_MODE_MASTER;
	SPIDevice->SPIConfig.BusConfig = SPI_BUS_CONFIG_FD;
	SPIDevice->SPIConfig.SclkSpeed = SPI_SCLK_SPEED_DIV4;
	SPIDevice->SPIConfig.DFF = SPI_DFF_8BITS;
	SPIDevice->SPIConfig.CPOL = SPI_CPOL_LOW;
	SPIDevice->SPIConfig.CPHA = SPI_CPHA_FIRST;
	SPIDevice->SPIConfig.SSM = SPI_SSM_SW;
	SPIDevice->SPIConfig.FrameFormat = SPI_FRAME_FORMAT_LSBFIRST;

	SPI_Init(SPIDevice);
}

int main(void){

	SPI_GPIO_Pins_t spi2Pins;
	SPI2_GPIO_Inits(&spi2Pins);

	SPI_Handle_t mySPIDevice;
	SPI2_Init(&mySPIDevice);

	// uint16_t myData[] = {0x0000, 0x1111, 0x2222, 0x3333, 0x4444, 0x5555, 0x6666, 0x7777, 0x8888, 0x9999, 0xAAAA, 0xBBBB, 0xCCCC, 0xDDDD, 0xEEEE, 0xFFFF};
	char* myString = "Fuck you";

	//SPI_SendData(mySPIDevice.pSPIx, myData, 10);

	while(1){
		SPI_SendData(mySPIDevice.pSPIx, (uint8_t*)myString, strlen(myString));
		delay();
	}
}
