/*
 * stm32f407xx_i2c_driver.c
 *
 *  Created on: Sep 21, 2022
 *      Author: linkachu
 */

#include "stm32f407xx.h"
#include "stm32f407xx_i2c_driver.h"


/*****************************************************************
 * @fn			- I2C_PeriClockControl
 *
 * @brief		- This function enables or disables peripheral clock for the given I2C Peripheral
 *
 * @param[in]	- Base address of the I2C peripheral
 * @param[in]	- ENABLE or DISABLE macros
 *
 * @return		- none
 *
 * @Note		- none
 */
void I2C_PeriClockControl(I2C_RegDef_t *pI2Cx, uint8_t EnorDi){
	if(EnorDi == ENABLE){
		if(pI2Cx == I2C1)
			I2C1_PCLK_EN();
		else if (pI2Cx == I2C2)
			I2C2_PCLK_EN();
		else if (pI2Cx == I2C3)
			I2C3_PCLK_EN();
	} else if (EnorDi == DISABLE){
		if(pI2Cx == I2C1)
			I2C1_PCLK_DI();
		else if (pI2Cx == I2C2)
			I2C2_PCLK_DI();
		else if (pI2Cx == I2C3)
			I2C3_PCLK_DI();
	}
}

// NOT USED IN THIS COURSE
uint32_t RCC_GetOutputClock(void){
	return 0;
}

uint16_t AHBPreScaler[9] = {2,4,8,16,32,64,128,256,512};
uint16_t APB1PreScaler[4] = {2,4,8,16};

uint32_t RCC_GetPCLK1Value(void){
	uint32_t pclk1, systemClk;
	uint16_t ahbp, apb1p;
	uint8_t clkSrc, temp;

	// Find where the clock source is coming from
	clkSrc = (RCC->CFGR >> 2) & 0b11;

	// If clock source is HSI
	if(clkSrc == 0){
		systemClk = 16000000;
	// If clock source is HSE
	} else if (clkSrc == 1){
		systemClk = 8000000;
	// If clock source is PLL (Not used in this course)
	} else if (clkSrc == 2){
		systemClk = RCC_GetOutputClock();
	}

	// Get the value for the AHB clock prescaler
	temp =  (RCC->CFGR >> 4) & 0b1111;

	if (temp < 0b1000)
		ahbp = 1;
	else
		ahbp = AHBPreScaler[temp - 0b1000];

	// Get the value for the APB1 prescaler
	temp = 0;
	temp = (RCC->CFGR >> 10) & 0b111;

	if (temp < 0b100)
		apb1p = 1;
	else
		apb1p = APB1PreScaler[temp - 0b100];

	pclk1 = systemClk / (ahbp * apb1p);

	return pclk1;
}


/*****************************************************************
 * @fn			- I2C_Init
 *
 * @brief		- This function initializes an I2C peripheral
 *
 * @param[in]	- Pointer to I2C peripheral base address
 *
 * @return		- none
 *
 * @Note		- none
 */
void I2C_Init(I2C_Handle_t *pI2CHandle){
	uint32_t tempreg = 0;

	tempreg |= pI2CHandle->I2C_Config.AckControl;


}

/*****************************************************************
 * @fn			- I2C_DeInit
 *
 * @brief		- This function resets a I2C peripheral
 *
 * @param[in]	- Pointer to I2C peripheral base address
 *
 * @return		- none
 *
 * @Note		- none
 */
void I2C_DeInit(I2C_RegDef_t *pI2Cx){
	if(pI2Cx == I2C1)
		I2C1_REG_RESET();
	else if (pI2Cx == I2C2)
		I2C2_REG_RESET();
	else if (pI2Cx == I2C3)
		I2C3_REG_RESET();
}

/*****************************************************************
 * @fn			- I2C_PeripheralControl
 *
 * @brief		- This function enables or disables a I2C peripheral
 *
 * @param[in]	- Pointer to I2C peripheral base address
 * @param[in]	- ENABLE or DISABLE
 *
 * @return		- none
 *
 * @Note		- none
 */
void I2C_PeripheralControl(I2C_RegDef_t * pI2Cx, uint8_t EnorDi){
	if (EnorDi == ENABLE)
		pI2Cx->CR1 |= (1 << I2C_CR1_PE);
	else
		pI2Cx->CR1 &= ~(1 << I2C_CR1_PE);
}
