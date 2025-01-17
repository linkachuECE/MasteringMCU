/*
 * 005SPI_sendData.c
 *
 *  Created on: Sep 10, 2022
 *      Author: linkachu
 */
#include "stm32f407xx.h"
#include "stm32407xx_spi_driver.h"
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

	GPIO_PeriClockControl(GPIOB, ENABLE);

	GPIO_Init(&(SPIPort->MOSI));
	GPIO_Init(&(SPIPort->MISO));
	GPIO_Init(&(SPIPort->SCK));
	GPIO_Init(&(SPIPort->NSS));
}

void SPI2_Init(SPI_Handle_t* SPIDevice){
	/*
	 * This version is configured with the following settings:
	 * - Master Mode
	 * - Full Duplex
	 * - 8-bit data frame format
	 * - Low Clock polarity, first phase detection
	 * - Hardware controlled slave select
	 * - Single master
	 */

	SPIDevice->pSPIx = SPI2;
	SPIDevice->SPIConfig.DeviceMode = SPI_DEVICE_MODE_MASTER;
	SPIDevice->SPIConfig.BusConfig = SPI_BUS_CONFIG_FD;
	SPIDevice->SPIConfig.SclkSpeed = SPI_SCLK_SPEED_DIV2;
	SPIDevice->SPIConfig.DFF = SPI_DFF_8BITS;
	SPIDevice->SPIConfig.CPOL = SPI_CPOL_LOW;
	SPIDevice->SPIConfig.CPHA = SPI_CPHA_FIRST;
	SPIDevice->SPIConfig.SSM = SPI_SSM_HW;
	SPIDevice->SPIConfig.FrameFormat = SPI_FRAME_FORMAT_LSBFIRST;

	SPI_Init(SPIDevice);

	// Enable SSOE so that NSS goes low when SPE goes high
	SPI_SSOEControl(SPIDevice->pSPIx, ENABLE);
}

void USRBTN_Init(GPIO_Handle_t* USRPB){
	USRPB->pGPIOx = GPIOA;
	USRPB->GPIO_PinConfig.GPIO_PinNumber = GPIO_PIN_NO_0;
	USRPB->GPIO_PinConfig.GPIO_PinMode = GPIO_MODE_IN;
	USRPB->GPIO_PinConfig.GPIO_PinSpeed = GPIO_SPEED_FAST;
	USRPB->GPIO_PinConfig.GPIO_PinPuPdControl = GPIO_NO_PUPD;

	GPIO_Init(USRPB);

	GPIO_IRQInterruptConfig(IRQ_NO_EXTI0, 1, ENABLE);
}

int main(void){

	// Initialize the appropriate GPIO pins on port B
	SPI_GPIO_Pins_t spi2Pins;
	SPI2_GPIO_Inits(&spi2Pins);

	// Initialize the SPI2 peripheral
	SPI_Handle_t mySPIDevice;
	SPI2_Init(&mySPIDevice);

	// Initialize user button and interrupts
	GPIO_Handle_t USRPB;
	memset(&USRPB, 0, sizeof(USRPB));
	USRBTN_Init(&USRPB);

	char* myString = "In the name of Robert of the house Baratheon, the first of his name, king of the Andals and the first men, lord of the seven kingdoms and protector of the realm. I Eddard Of the house Stark, lord of Winterfell and warden of the north sentence you to die.";

	SPI_PeripheralControl(mySPIDevice.pSPIx, ENABLE);

	while(1){
		while ( ! GPIO_ReadFromInputPin(GPIOA, GPIO_PIN_NO_0));
		delay();

		SPI_PeripheralControl(mySPIDevice.pSPIx, ENABLE);

		uint8_t stringSize = strlen(myString);

		SPI_SendData(mySPIDevice.pSPIx, &stringSize, sizeof(uint8_t));

		SPI_SendData(mySPIDevice.pSPIx, (uint8_t*)myString, strlen(myString));
		delay();

		while(SPI_GetFlagStatus(mySPIDevice.pSPIx, SPI_BSY_FLAG));

		SPI_PeripheralControl(mySPIDevice.pSPIx, DISABLE);
	}
}
