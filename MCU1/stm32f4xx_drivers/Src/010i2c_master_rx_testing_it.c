/*
 * 008I2C_Arduino.c
 *
 *  Created on: Oct 1, 2022
 *      Author: linkachu
 */

#include "stm32f407xx.h"
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

extern void initialise_monitor_handles(void);

#define MYADDR 			0x3F
#define SLAVEADDR 		0x68
#define CMD_READLENGTH	0x51
#define CMD_READDATA	0x52

typedef struct {
	GPIO_Handle_t SDA;
	GPIO_Handle_t SCL;
} I2CGPIOHandle_t;

// Global handle for use with the interrupt
I2C_Handle_t myI2CHandle;

void delay(uint16_t ms){
	for(uint32_t i = 0; i < (ms*1000); i++);
}

void I2C1_GPIOInits(I2CGPIOHandle_t *pI2CGPIOHandle){
	pI2CGPIOHandle->SCL.pGPIOx = GPIOB;
	pI2CGPIOHandle->SCL.GPIO_PinConfig.GPIO_PinNumber = GPIO_PIN_NO_6;
	pI2CGPIOHandle->SCL.GPIO_PinConfig.GPIO_PinMode = GPIO_MODE_ALTFN;
	pI2CGPIOHandle->SCL.GPIO_PinConfig.GPIO_PinAltFunMode = 4;
	pI2CGPIOHandle->SCL.GPIO_PinConfig.GPIO_PinOPType = GPIO_OP_TYPE_OD;
	pI2CGPIOHandle->SCL.GPIO_PinConfig.GPIO_PinPuPdControl = GPIO_NO_PUPD;
	pI2CGPIOHandle->SCL.GPIO_PinConfig.GPIO_PinSpeed = GPIO_SPEED_FAST;

	pI2CGPIOHandle->SDA.pGPIOx = GPIOB;
	pI2CGPIOHandle->SDA.GPIO_PinConfig.GPIO_PinNumber = GPIO_PIN_NO_7;
	pI2CGPIOHandle->SDA.GPIO_PinConfig.GPIO_PinMode = GPIO_MODE_ALTFN;
	pI2CGPIOHandle->SDA.GPIO_PinConfig.GPIO_PinAltFunMode = 4;
	pI2CGPIOHandle->SDA.GPIO_PinConfig.GPIO_PinOPType = GPIO_OP_TYPE_OD;
	pI2CGPIOHandle->SDA.GPIO_PinConfig.GPIO_PinPuPdControl = GPIO_NO_PUPD;
	pI2CGPIOHandle->SDA.GPIO_PinConfig.GPIO_PinSpeed = GPIO_SPEED_FAST;

	GPIO_Init(&(pI2CGPIOHandle->SDA));
	GPIO_Init(&(pI2CGPIOHandle->SCL));
}

void I2C1_Init(I2C_Handle_t *pI2CHandle){
	pI2CHandle->pI2Cx = I2C1;
	pI2CHandle->I2C_Config.AckControl = I2C_ACK_ENABLE;
	pI2CHandle->I2C_Config.DeviceAddress = 0x3F;
	pI2CHandle->I2C_Config.FMDutyCycle = I2C_FM_DUTY_2;
	pI2CHandle->I2C_Config.SCLSpeed = I2C_SCL_SPEED_SM;

	I2C_Init(pI2CHandle);
}

void USRBTN_Init(GPIO_Handle_t* pUSRPB){
	pUSRPB->pGPIOx = GPIOA;
	pUSRPB->GPIO_PinConfig.GPIO_PinNumber = GPIO_PIN_NO_0;
	pUSRPB->GPIO_PinConfig.GPIO_PinMode = GPIO_MODE_IT_RT;
	pUSRPB->GPIO_PinConfig.GPIO_PinSpeed = GPIO_SPEED_FAST;
	pUSRPB->GPIO_PinConfig.GPIO_PinPuPdControl = GPIO_NO_PUPD;

	GPIO_Init(pUSRPB);

	GPIO_IRQInterruptConfig(IRQ_NO_EXTI0, 1, ENABLE);
}

void sendCommand(uint8_t cmd){
	while(I2C_MasterSendDataIT(&myI2CHandle, &cmd, sizeof(uint8_t), SLAVEADDR, I2C_ENABLE_RS) != I2C_READY);
}

// Retrieves the length of the string
uint8_t getLength(){
	uint8_t strLen;

	sendCommand(CMD_READLENGTH);

	while(I2C_MasterReceiveDataIT(&myI2CHandle, &strLen, 1, SLAVEADDR, I2C_ENABLE_RS)!= I2C_READY);

	return strLen;
}

// Retrieves the string itself
uint8_t* getData(uint8_t len){
	uint8_t* data = (uint8_t*)malloc((len * sizeof(uint8_t)) + 1);

	sendCommand(CMD_READDATA);

	while(I2C_MasterReceiveDataIT(&myI2CHandle, data, len, SLAVEADDR, I2C_DISABLE_RS)!= I2C_READY);

	return data;
}

void readFromArduino(){
	I2C_PeripheralControl(&myI2CHandle, ENABLE);

	uint8_t len = getLength();
	uint8_t* data = getData(len);
	I2C_PeripheralControl(&myI2CHandle, DISABLE);

	data[len] = '\0';

	printf("Received string: %s\n", (char*)data);

	free(data);
}

int main(){
	initialise_monitor_handles();

	// Initialize the GPIO's to be used for I2C
	I2CGPIOHandle_t I2C1GPIOs;
	I2C1_GPIOInits(&I2C1GPIOs);

	// Initialize I2C1 itself
	I2C1_Init(&myI2CHandle);

	// I2C IRQ Configurations
	I2C_IRQInterruptConfig(IRQ_NO_I2C1_ER, 0, ENABLE);
	I2C_IRQInterruptConfig(IRQ_NO_I2C1_EV, 0, ENABLE);

	// Initialize push button
	GPIO_Handle_t USRPB;
	USRBTN_Init(&USRPB);

	while(1);
}

void EXTI0_IRQHandler(void){
	delay(200);
	GPIO_IRQHandling(GPIO_PIN_NO_0);
	readFromArduino();
}

void I2C1_ER_IRQHandler(void){
	I2C_ER_IRQHandling(&myI2CHandle);
}

void I2C1_EV_IRQHandler(void){
	I2C_EV_IRQHandling(&myI2CHandle);
}

void I2C_ApplicationEventCallback(I2C_Handle_t *pI2CHandle, uint8_t EvorEr){
	if(EvorEr == I2C_EV_TX_CMPLT){
		printf("Tx is completed\n");
	} else if (EvorEr == I2C_EV_RX_CMPLT){
		printf("Rx is completed\n");
	} else if (EvorEr == I2C_EV_STOP){
		printf("Communications have been stopped\n");
	} else if (EvorEr == I2C_ERROR_BERR){
		printf("Error: Bus error\n");
	} else if (EvorEr == I2C_ERROR_ARLO){
		printf("Error: Arbitration lost\n");
	} else if (EvorEr == I2C_ERROR_AF){
		// In master mode, ACK faiure happens when the slave fails to send ACK
		// for the byte sent from the master
		printf("Error: ACK failure\n");
		I2C_CloseSendData(pI2CHandle);

		// Generate the stop condition to release the bus
		I2C_GenerateStopCondition(pI2CHandle->pI2Cx);

		// Hang in infinite loop
		while(1);

	} else if (EvorEr == I2C_ERROR_OVR){
		printf("Error: Overrun/underrun\n");
	} else if (EvorEr == I2C_ERROR_TIMEOUT){
		printf("Error: Timeout\n");
	}
}
