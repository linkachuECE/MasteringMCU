/*
 * stm324707xx_gpio_driver.c
 *
 *  Created on: Sep 3, 2022
 *      Author: linkachu
 */

#include "stm32407xx_gpio_driver.h"
#include "stm32f407xx.h"

// *********** API IMPLEMENTATIONS ************* //

// Peripheral clock setup

/*****************************************************************
 * @fn			- GPIO_PeriClockControl
 *
 * @brief		- This function enables or disables peripheral clock for the given GPIO port
 *
 * @param[in]	- Base address of the GPIO peripheral
 * @param[in]	- ENABLE or DISABLE macros
 *
 * @return		- none
 *
 * @Note		- none
 */
void GPIO_PeriClockControl(GPIO_RegDef_t *pGPIOx, uint8_t EnorDi){}

// Init and de-enit
void GPIO_Init(GPIO_Handle_t *pGPIOHandle){}
void GPIO_DeInit(GPIO_RegDef_t *pGPIOx){}

// Data read and write
uint8_t GPIO_ReadFromInputPin(GPIO_RegDef_t *pGPIOx, uint8_t pinNumber){}
uint16_t GPIO_ReadFromInputPort(GPIO_RegDef_t *pGPIOx){}
void GPIO_WriteToOutputPin(GPIO_RegDef_t *pGPIOx, uint8_t pinNumber, uint8_t value){}
void GPIO_WriteToOutputPort(GPIO_RegDef_t *pGPIOx, uint16_t value){}
void GPIO_ToggleOutputPin(GPIO_RegDef_t, uint8_t pinNumber){}

// IRQ
void GPIO_IRQConfig(uint8_t IRQNumber, uint8_t IRQPriority, uint8_t EnorDi){}
void GPIO_IRQHandling(uint8_t pinNumber){}
