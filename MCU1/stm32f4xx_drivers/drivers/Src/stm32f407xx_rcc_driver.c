/*
 * stm32f407xx_rcc_driver.c
 *
 *  Created on: Oct 12, 2022
 *      Author: linkachu
 */

#include "stm32f407xx_rcc_driver.h"

uint16_t AHBPrescaler[] = {2, 4, 8, 16, 64, 128, 256, 512};
uint16_t APB1Prescaler[] = {2, 4, 8, 16};
uint16_t APB2Prescaler[] = {2, 4, 8, 16};

/*****************************************************************
 * @fn			- RCC_GetPCLK1Value
 *
 * @brief		- This function retrieves the clock speed of the APB1 bus
 *
 * @return		- APB1 Clock speed
 *
 * @Note		- none
 */
uint32_t RCC_GetPCLK1Value(void){
	uint32_t pclk1, apb1p, ahbp, systemClk, temp;

	uint8_t clksrc;
	clksrc = (RCC->CFGR >> 2) & 0b11;

	// System clock is HSI
	if(clksrc == 0){
		systemClk = 16000000;
	}
	// System clock is HSE
	else if(clksrc == 1){
		systemClk = 8000000;
	}
	// System clock is PLL
	else if (clksrc == 2){
		// Not used in this course
		// systemClk = RCC_GetPLLOutputClock();
	}

	//AHB Prescaler starts at the fourth bit
	temp = (RCC->CFGR >> 4) & 0b1111;
	if (temp < 0b1000)
		ahbp = 1;
	else
		ahbp = AHBPrescaler[temp - 0b1000];

	//APB1 prescaler starts at 10th bit
	temp = (RCC->CFGR >> 10) & 0b111;
	if (temp < 0b100)
		apb1p = 1;
	else
		apb1p = APB1Prescaler[temp - 0b100];

	pclk1 = systemClk/(ahbp * apb1p);

	return pclk1;
}

/*****************************************************************
 * @fn			- RCC_GetPCLK2Value
 *
 * @brief		- This function retrieves the clock speed of the APB2 bus
 *
 * @return		- APB2 Clock speed
 *
 * @Note		- none
 */
uint32_t RCC_GetPCLK2Value(void){
	uint32_t pclk2, apb2p, ahbp, systemClk, temp;

	uint8_t clksrc;
	clksrc = (RCC->CFGR >> 2) & 0b11;

	// System clock is HSI
	if(clksrc == 0){
		systemClk = 16000000;
	}
	// System clock is HSE
	else if(clksrc == 1){
		systemClk = 8000000;
	}
	// System clock is PLL
	else if (clksrc == 2){
		// Not used in this course
		// systemClk = RCC_GetPLLOutputClock();
	}

	// AHB Prescaler starts at the fourth bit
	temp = (RCC->CFGR >> 4) & 0b1111;
	if (temp < 0b1000)
		ahbp = 1;
	else
		ahbp = AHBPrescaler[temp - 0b1000];

	// APB2 prescaler starts at 13th bit
	temp = (RCC->CFGR >> 13) & 0b111;
	if (temp < 0b100)
		apb2p = 1;
	else
		apb2p = APB2Prescaler[temp - 0b100];

	pclk2 = systemClk/(ahbp * apb2p);

	return pclk2;
}
