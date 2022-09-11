/*
 * 005SPI_sendData.c
 *
 *  Created on: Sep 10, 2022
 *      Author: linkachu
 */
#include "stm32f407xx.h"

int main(void){
	return 0;

	uint8_t myData[10] = {1,2,3,4,5,6,7,8,9,10};

	SPI_Handle_t mySPIDevice;
	mySPIDevice.SPIConfig.DeviceMode = SPI_DEVICE_MODE_MASTER;
	mySPIDevice.SPIConfig.BusConfig = SPI_BUS_CONFIG_FD;
	mySPIDevice.SPIConfig.SclkSpeed = SPI_SCLK_SPEED_DIV2;
	mySPIDevice.SPIConfig.DFF = SPI_DFF_16BITS;
	mySPIDevice.SPIConfig.CPOL = SPI_CPOL_HIGH;
	mySPIDevice.SPIConfig.CPHA = SPI_CPHA_SECOND;
	mySPIDevice.SPIConfig.SSM = SPI_SSM_HW;

	SPI_Init(&mySPIDevice);
	SPI_SendData(mySPIDevice.pSPIx, myData, 10);
};
