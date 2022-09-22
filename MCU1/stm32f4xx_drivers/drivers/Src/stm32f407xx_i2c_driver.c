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
