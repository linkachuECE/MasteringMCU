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
		switch ((uint32_t)pGPIOx){
			case (uint32_t)GPIOA:
				GPIOA_PCLK_EN();
				break;
			case (uint32_t)GPIOB:
				GPIOB_PCLK_EN();
				break;
			case (uint32_t)GPIOC:
				GPIOC_PCLK_EN();
				break;
			case (uint32_t)GPIOD:
				GPIOD_PCLK_EN();
				break;
			case (uint32_t)GPIOE:
				GPIOE_PCLK_EN();
				break;
			case (uint32_t)GPIOF:
				GPIOF_PCLK_EN();
				break;
			case (uint32_t)GPIOG:
				GPIOH_PCLK_EN();
				break;
			case (uint32_t)GPIOI:
				GPIOI_PCLK_EN();
				break;
			default:
				perror("Invalid pointer passed to GPIO_PeriClockControl()");
				exit(1);
		}
	} else if (EnorDi == DISABLE){
		switch ((uint32_t)pGPIOx){
			case (uint32_t)GPIOA:
				GPIOA_PCLK_DI();
				break;
			case (uint32_t)GPIOB:
				GPIOB_PCLK_DI();
				break;
			case (uint32_t)GPIOC:
				GPIOC_PCLK_DI();
				break;
			case (uint32_t)GPIOD:
				GPIOD_PCLK_DI();
				break;
			case (uint32_t)GPIOE:
				GPIOE_PCLK_DI();
				break;
			case (uint32_t)GPIOF:
				GPIOF_PCLK_DI();
				break;
			case (uint32_t)GPIOG:
				GPIOH_PCLK_DI();
				break;
			case (uint32_t)GPIOI:
				GPIOI_PCLK_DI();
				break;
			default:
				perror("Invalid pointer passed to GPIO_PeriClockControl()");
				exit(1);
		}
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

void GPIO_DeInit(GPIO_RegDef_t *pGPIOx){}

// Data read and write
uint8_t GPIO_ReadFromInputPin(GPIO_RegDef_t *pGPIOx, uint8_t pinNumber){}
uint16_t GPIO_ReadFromInputPort(GPIO_RegDef_t *pGPIOx){}
void GPIO_WriteToOutputPin(GPIO_RegDef_t *pGPIOx, uint8_t pinNumber, uint8_t value){}
void GPIO_WriteToOutputPort(GPIO_RegDef_t *pGPIOx, uint16_t value){}
void GPIO_ToggleOutputPin(GPIO_RegDef_t *pGPIOx, uint8_t pinNumber){}

// IRQ
void GPIO_IRQConfig(uint8_t IRQNumber, uint8_t IRQPriority, uint8_t EnorDi){}
void GPIO_IRQHandling(uint8_t pinNumber){}
