/*
 * stm324707xx_gpio_driver.h
 *
 *  Created on: Sep 3, 2022
 *      Author: linkachu
 */

#ifndef INC_STM32407XX_GPIO_DRIVER_H_
#define INC_STM32407XX_GPIO_DRIVER_H_

#include "stm32f407xx.h"

typedef struct{
	uint8_t GPIO_PinNumber;
	uint8_t GPIO_PinMode;
	uint8_t GPIO_PinSpeed;
	uint8_t GPIO_PinPuPdControl;
	uint8_t GPIO_PinOPType;
	uint8_t GPIO_PinAltFunMode;
} GPIO_PinConfig_t;

typedef struct{
	GPIO_RegDef_t *pGPIOx; // This is a pointer to the base address of the GPIO port to which the pin belongs
	GPIO_PinConfig_t GPIO_PinConfig;
} GPIO_Handle_t;


// *********** API PROTOTYPES ************* //

// Peripheral clock setup
void GPIO_PeriClockControl(GPIO_RegDef_t *pGPIOx, uint8_t EnorDi);

// Init and de-enit
void GPIO_Init(GPIO_Handle_t *pGPIOHandle);
void GPIO_DeInit(GPIO_RegDef_t *pGPIOx);

// Data read and write
uint8_t GPIO_ReadFromInputPin(GPIO_RegDef_t *pGPIOx, uint8_t pinNumber);
uint16_t GPIO_ReadFromInputPort(GPIO_RegDef_t *pGPIOx);
void GPIO_WriteToOutputPin(GPIO_RegDef_t *pGPIOx, uint8_t pinNumber, uint8_t value);
void GPIO_WriteToOutputPort(GPIO_RegDef_t *pGPIOx, uint16_t value);
void GPIO_ToggleOutputPin(GPIO_RegDef_t, uint8_t pinNumber);

// IRQ
void GPIO_IRQConfig(uint8_t IRQNumber, uint8_t IRQPriority, uint8_t EnorDi);
void GPIO_IRQHandling(uint8_t pinNumber);


#endif /* INC_STM32407XX_GPIO_DRIVER_H_ */
