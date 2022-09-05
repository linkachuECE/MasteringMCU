/*
 * stm324707xx_gpio_driver.c
 *
 *  Created on: Sep 3, 2022
 *      Author: linkachu
 */

#include "stm32407xx_gpio_driver.h"
#include "stm32f407xx.h"
#include <stdio.h>
#include <stdint.h>
#include <unistd.h>
#include <stdlib.h>
#include <math.h>

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
void GPIO_PeriClockControl(GPIO_RegDef_t *pGPIOx, uint8_t EnorDi){
	if(EnorDi == ENABLE){
		if(pGPIOx == GPIOA)
			GPIOA_PCLK_EN();
		else if (pGPIOx == GPIOB)
			GPIOB_PCLK_EN();
		else if (pGPIOx == GPIOC)
			GPIOC_PCLK_EN();
		else if (pGPIOx == GPIOD)
			GPIOD_PCLK_EN();
		else if (pGPIOx == GPIOE)
			GPIOE_PCLK_EN();
		else if (pGPIOx == GPIOF)
			GPIOF_PCLK_EN();
		else if (pGPIOx == GPIOG)
			GPIOG_PCLK_EN();
		else if (pGPIOx == GPIOH)
			GPIOH_PCLK_EN();
		else if (pGPIOx == GPIOI)
			GPIOI_PCLK_EN();
	} else if (EnorDi == DISABLE){
		if(pGPIOx == GPIOA)
			GPIOA_PCLK_DI();
		else if (pGPIOx == GPIOB)
			GPIOB_PCLK_DI();
		else if (pGPIOx == GPIOC)
			GPIOC_PCLK_DI();
		else if (pGPIOx == GPIOD)
			GPIOD_PCLK_DI();
		else if (pGPIOx == GPIOE)
			GPIOE_PCLK_DI();
		else if (pGPIOx == GPIOF)
			GPIOF_PCLK_DI();
		else if (pGPIOx == GPIOG)
			GPIOG_PCLK_DI();
		else if (pGPIOx == GPIOH)
			GPIOH_PCLK_DI();
		else if (pGPIOx == GPIOI)
			GPIOI_PCLK_DI();
	}
}

// Init and de-enit

/*****************************************************************
 * @fn			- GPIO_Init
 *
 * @brief		- This function initializes a single pin in a port
 *
 * @param[in]	- GPIO Handler struct
 * @param[out]	- temporary uint32_t for register manipulation
 *
 * @return		- none
 *
 * @Note		- none
 */
void GPIO_Init(GPIO_Handle_t *pGPIOHandle){
	uint32_t temp = 0;
	// 1. Configure the mode of the GPIO pin

	// This block will execute if the pin mode is a non-interrupt mode
	if(pGPIOHandle->GPIO_PinConfig.GPIO_PinMode <= GPIO_MODE_ANALOG){
		// this temp variable stores the appropriate pin mode shifted by the number of bits
		// it takes to get to the appropriate bits for the pin in the port mode register.
		// The "2" multiplier is there because there are two bits per pin
		temp = (pGPIOHandle->GPIO_PinConfig.GPIO_PinMode << (2 * pGPIOHandle->GPIO_PinConfig.GPIO_PinNumber));
		pGPIOHandle->pGPIOx->MODER &= ~(0x3 << pGPIOHandle->GPIO_PinConfig.GPIO_PinNumber); // clearing
		pGPIOHandle->pGPIOx->MODER |= temp;  // settting
	}
	// This block will execute if the pin mode is an interrupt mode
	else{
			// code later
	}
	temp = 0;

	// 2. Configure the speed
	temp = (pGPIOHandle->GPIO_PinConfig.GPIO_PinSpeed << (2 * pGPIOHandle->GPIO_PinConfig.GPIO_PinNumber));
	pGPIOHandle->pGPIOx->OSPEEDR &= ~(0x3 << pGPIOHandle->GPIO_PinConfig.GPIO_PinNumber); // clearing
	pGPIOHandle->pGPIOx->OSPEEDR |= temp;
	temp = 0;

	// 3. Configure the PUPD settings
	temp = (pGPIOHandle->GPIO_PinConfig.GPIO_PinPuPdControl << (2 * pGPIOHandle->GPIO_PinConfig.GPIO_PinNumber));
	pGPIOHandle->pGPIOx->PUPDR &= ~(0x3 << pGPIOHandle->GPIO_PinConfig.GPIO_PinNumber); // clearing
	pGPIOHandle->pGPIOx->PUPDR |= temp;
	temp = 0;

	// 4. Configure the output type
	temp = (pGPIOHandle->GPIO_PinConfig.GPIO_PinOPType << (pGPIOHandle->GPIO_PinConfig.GPIO_PinNumber));
	pGPIOHandle->pGPIOx->OTYPER &= ~(0x1 << pGPIOHandle->GPIO_PinConfig.GPIO_PinNumber); // clearing
	pGPIOHandle->pGPIOx->OTYPER |= temp;
	temp = 0;

	// 5. Configure the alternate functionality
	if(pGPIOHandle->GPIO_PinConfig.GPIO_PinMode == GPIO_MODE_ALTFN){
		uint8_t temp1, temp2;
		// set temp equal to the alternate functionality mode, bit-shifted to the left by modulus 8 of the pin number multiplied by 4
		// The modulo is because there are two AFR registers, and the 4-times multiplication is because each pin is represented by 4 bits
		temp1 = pGPIOHandle->GPIO_PinConfig.GPIO_PinNumber / 8;
		temp2 = pGPIOHandle->GPIO_PinConfig.GPIO_PinNumber % 8;
		pGPIOHandle->pGPIOx->AFR[temp1] &= ~(0xF << (4 * temp2));
		pGPIOHandle->pGPIOx->AFR[temp1] |= (pGPIOHandle->GPIO_PinConfig.GPIO_PinAltFunMode << (4 * temp2));
	}


}

/*****************************************************************
 * @fn			- GPIO_DeInit
 *
 * @brief		- This function resets all of the pins in a GPIO port
 *
 * @param[in]	- Pointer to GPIO port base address
 *
 * @return		- none
 *
 * @Note		- none
 */
void GPIO_DeInit(GPIO_RegDef_t *pGPIOx){
	// Reset clock register
	if(pGPIOx == GPIOA)
		GPIOA_REG_RESET();
	else if (pGPIOx == GPIOB)
		GPIOB_REG_RESET();
	else if (pGPIOx == GPIOC)
		GPIOC_REG_RESET();
	else if (pGPIOx == GPIOD)
		GPIOD_REG_RESET();
	else if (pGPIOx == GPIOE)
		GPIOE_REG_RESET();
	else if (pGPIOx == GPIOF)
		GPIOF_REG_RESET();
	else if (pGPIOx == GPIOG)
		GPIOG_REG_RESET();
	else if (pGPIOx == GPIOH)
		GPIOH_REG_RESET();
	else if (pGPIOx == GPIOI)
		GPIOI_REG_RESET();
}

// Data read and write

/*****************************************************************
 * @fn			- GPIO_ReadFromInputPin
 *
 * @brief		- This function reads from the specified input pin
 *
 * @param[in]	- Pointer to GPIO port base address
 * @param[in]	- Pin number
 *
 * @return		- Value read, either 1 or 0
 *
 * @Note		- none
 */
uint8_t GPIO_ReadFromInputPin(GPIO_RegDef_t *pGPIOx, uint8_t pinNumber){
	uint8_t value;
	value = (uint8_t) ((pGPIOx->IDR >> pinNumber) & 0x00000001);
	return value;
}

/*****************************************************************
 * @fn			- GPIO_ReadFromInputPort
 *
 * @brief		- This function reads from the specified input port
 *
 * @param[in]	- Pointer to GPIO port base address
 *
 * @return		- 16-bit value read
 *
 * @Note		- none
 */
uint16_t GPIO_ReadFromInputPort(GPIO_RegDef_t *pGPIOx){
	uint16_t value;
	value = (uint16_t)(pGPIOx->IDR);
	return value;
}

/*****************************************************************
 * @fn			- GPIO_WriteToOutputPin
 *
 * @brief		- This function writes to the specified output pin
 *
 * @param[in]	- Pointer to GPIO port base address
 * @param[in]	- Pin number
 * @param[in]	- Value to write, either GPIO_PIN_SET or GPIO_PIN_RESET
 *
 * @return		- none
 *
 * @Note		- none
 */
void GPIO_WriteToOutputPin(GPIO_RegDef_t *pGPIOx, uint8_t pinNumber, uint8_t value){
	if(value == GPIO_PIN_SET)
		pGPIOx->ODR |= (1 << pinNumber);
	else
		pGPIOx->ODR &= ~(1 << pinNumber);
}

/*****************************************************************
 * @fn			- GPIO_WriteToOutputPort
 *
 * @brief		- This function writes to the specified output port
 *
 * @param[in]	- Pointer to GPIO port base address
 * @param[in]	- 16-bit value to write
 *
 * @return		- none
 *
 * @Note		- none
 */
void GPIO_WriteToOutputPort(GPIO_RegDef_t *pGPIOx, uint16_t value){
	pGPIOx->ODR = value;
}

/*****************************************************************
 * @fn			- GPIO_ToggleOutputPin
 *
 * @brief		- This function toggles the value of the specified pin
 *
 * @param[in]	- Pointer to GPIO port base address
 * @param[in]	- Pin number
 *
 * @return		- none
 *
 * @Note		- none
 */
void GPIO_ToggleOutputPin(GPIO_RegDef_t *pGPIOx, uint8_t pinNumber){
	pGPIOx->ODR ^= (1 << pinNumber);
}

// IRQ
void GPIO_IRQConfig(uint8_t IRQNumber, uint8_t IRQPriority, uint8_t EnorDi){}
void GPIO_IRQHandling(uint8_t pinNumber){}
