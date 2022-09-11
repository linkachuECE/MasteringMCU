/*
 * 005SPI_sendData.c
 *
 *  Created on: Sep 10, 2022
 *      Author: linkachu
 */
#include "stm32f407xx.h"

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
	SPIDevice->SPIConfig.SclkSpeed = SPI_SCLK_SPEED_DIV2;
	SPIDevice->SPIConfig.DFF = SPI_DFF_8BITS;
	SPIDevice->SPIConfig.CPOL = SPI_CPOL_LOW;
	SPIDevice->SPIConfig.CPHA = SPI_CPHA_FIRST;
	SPIDevice->SPIConfig.SSM = SPI_SSM_SW;

	SPI_Init(SPIDevice);
}

int main(void){

	SPI_GPIO_Pins_t spi2Pins;
	SPI2_GPIO_Inits(&spi2Pins);

	SPI_Handle_t mySPIDevice;
	SPI2_Init(&mySPIDevice);

	uint8_t myData[10] = {1,2,3,4,5,6,7,8,9,10};

	SPI_SendData(mySPIDevice.pSPIx, myData, 10);

	while(1);
}
