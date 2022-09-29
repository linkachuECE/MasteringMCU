/*
 * stm32f407xx_i2c_driver.c
 *
 *  Created on: Sep 21, 2022
 *      Author: linkachu
 */

#include "stm32f407xx.h"
#include "stm32f407xx_i2c_driver.h"

// STATIC FUNCTIONS

/*****************************************************************
 * @fn			- I2C_GenerateStartCondition
 *
 * @brief		- This static function generates the start condition for an I2C transmission
 *
 * @param[in]	- Pointer to I2C base address
 *
 * @return		- none
 *
 * @Note		- none
 */
static void I2C_GenerateStartCondition(I2C_RegDef_t *pI2Cx){
	pI2Cx->CR1 |= (1 << I2C_CR1_START);
}

/*****************************************************************
 * @fn			- I2C_GenerateStopCondition
 *
 * @brief		- This static function generates the stop condition for an I2C transmission
 *
 * @param[in]	- Pointer to I2C base address
 *
 * @return		- none
 *
 * @Note		- none
 */
static void I2C_GenerateStopCondition(I2C_RegDef_t *pI2Cx){
	pI2Cx->CR1 |= (1 << I2C_CR1_STOP);
}

/*****************************************************************
 * @fn			- I2C_ExecuteAddressPhase
 *
 * @brief		- This static function sends the address plus a read or write bit to the slave
 *
 * @param[in]	- Pointer to I2C base address
 * @param[in]	- Slave address
 * @param[in]   - Read or write bit
 *
 * @return		- none
 *
 * @Note		- none
 */
static void I2C_ExecuteAddressPhase(I2C_RegDef_t *pI2Cx, uint8_t slaveAddr, uint8_t RorW){
	if (RorW == READ)
		pI2Cx->DR = ((slaveAddr << 1) | (0b00000001));
	else if (RorW == WRITE)
		pI2Cx->DR = ((slaveAddr << 1) & ~(0b00000001));
}

/*****************************************************************
 * @fn			- I2C_ExecuteAddressPhase
 *
 * @brief		- This static function clears the ADDR flag in the I2C peripheral
 *
 * @param[in]	- Pointer to I2C base address
 *
 * @return		- none
 *
 * @Note		- none
 */
static void I2C_ClearADDRFlag(I2C_RegDef_t *pI2Cx){
	uint32_t dummy;
	dummy = pI2Cx->SR1;
	dummy = pI2Cx->SR2;
	(void)dummy;
}

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

uint32_t RCC_GetPLLOutputClock();


uint16_t AHBPrescaler[] = {2, 4, 8, 16, 64, 128, 256, 512};
uint16_t APB1Prescaler[] = {2, 4, 8, 16};


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

	I2C_PeriClockControl(pI2CHandle->pI2Cx, ENABLE);

	tempreg |= (pI2CHandle->I2C_Config.AckControl << 10);
	pI2CHandle->pI2Cx->CR1 = tempreg;

	// Configure the FREQ field of CR2
	tempreg = 0;
	tempreg = RCC_GetPCLK1Value() / 1000000U;
	pI2CHandle->pI2Cx->CR2 = (tempreg & 0x3F);

	// Address configuration
	tempreg = 0;
	tempreg = (pI2CHandle->I2C_Config.DeviceAddress << I2C_OAR1_ADD7);
	pI2CHandle->pI2Cx->OAR1 |= tempreg;

	// Set to 7-bit slave address mode
	pI2CHandle->pI2Cx->OAR1 &= ~(1 << I2C_OAR1_ADD_MODE);

	// Bit 14 of OAR1 has to be kept at 1 by software, for some reason
	pI2CHandle->pI2Cx->OAR1 |= (1 << 14);

	// CCR calculations
	uint16_t ccr_value = 0;
	if(pI2CHandle->I2C_Config.SCLSpeed <= I2C_SCL_SPEED_SM){
		// Mode is standard mode
		pI2CHandle->pI2Cx->CCR &= ~(1 << I2C_CCR_FS);
		ccr_value = RCC_GetPCLK1Value() / (2 * pI2CHandle->I2C_Config.SCLSpeed);
		tempreg |= ccr_value & (0xFFF);
	} else {
		// Mode is fast mode
		pI2CHandle->pI2Cx->CCR |= (1 << I2C_CCR_FS);

		if(pI2CHandle->I2C_Config.FMDutyCycle == I2C_FM_DUTY_2){
			ccr_value = RCC_GetPCLK1Value() / (3 * pI2CHandle->I2C_Config.SCLSpeed);
		} else {
			ccr_value = RCC_GetPCLK1Value() / (25 * pI2CHandle->I2C_Config.SCLSpeed);
		}

		tempreg |= ccr_value & (0xFFF);
	}

	pI2CHandle->pI2Cx->CCR |= (tempreg);

	//TRISE Configuration
	if(pI2CHandle->I2C_Config.SCLSpeed <= I2C_SCL_SPEED_SM){
		tempreg = (RCC_GetPCLK1Value() / 1000000U) + 1;
	} else {
		tempreg = ((RCC_GetPCLK1Value() * 300) /1000000U) + 1;
	}

	pI2CHandle->pI2Cx->TRISE =  (tempreg & 0x3F);
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

// Data send and receive

/*****************************************************************
 * @fn			- I2C_GetFlagStatus
 *
 * @brief		- This function returns the value currently held in a bit or field in a I2C status register
 *
 * @param[in]	- Pointer to I2C base address
 * @param[in]	- Flag name macro
 *
 * @return		- SET or RESET
 *
 * @Note		- none
 */
uint8_t I2C_GetSR1FlagStatus(I2C_RegDef_t *pI2Cx, uint32_t flagName){
	if(pI2Cx->SR1 & flagName)
		return FLAG_SET;
	else
		return FLAG_RESET;
}

/*****************************************************************
 * @fn			- I2C_MasterSendData
 *
 * @brief		- This function send data via I2C
 *
 * @param[in]	- Pointer to I2C Handle
 * @param[in]	- Pointer to data buffer
 * @param[in]	- Length of data to send
 * @param[in]	- Address of slave to send data to
 *
 *
 * @return		- none
 *
 * @Note		- none
 */
void I2C_MasterSendData(I2C_Handle_t *pI2CHandle, uint8_t *pTxbuffer, uint32_t len, uint8_t slaveAddr){
	// 1. Generate the START condition
	I2C_GenerateStartCondition(pI2CHandle->pI2Cx);

	// 2. Confirm that start generation is completed by checking the SB flag in the SR1
	// Note: Until SB is cleared SCL will be stretched
	while (I2C_GetSR1FlagStatus(pI2CHandle->pI2Cx, I2C_SB_FLAG) == FLAG_RESET);

	// 3. Send the address of the slave with r/nw bit set to w(0) (total 8 bits)
	I2C_ExecuteAddressPhase(pI2CHandle->pI2Cx, slaveAddr, WRITE);

	// 4. Confirm that address phase is completed by checking the ADDR flag in the SR1
	while (I2C_GetSR1FlagStatus(pI2CHandle->pI2Cx, I2C_ADDR_FLAG) == FLAG_RESET);

	// 5. Clear the ADDR flag according to its software sequence
	//    Note: until ADDR is cleared SCL will be stretched (pulled to LOW)
	I2C_ClearADDRFlag(pI2CHandle->pI2Cx);

	// 6. Send the data until len becomes 0
	for(int i = 0; i < len; i++){
		while(I2C_GetSR1FlagStatus(pI2CHandle->pI2Cx, I2C_TxE_FLAG) == FLAG_RESET);
		pI2CHandle->pI2Cx->DR = pTxbuffer[i];
	}

	// 7. When Len becomes zero wait for TXE=1 and BTF=1 before generating ther STOP condition
	//	  Note: TXE=1, BTF-1, means that both SR and DR are empty and next transmission should begin
	//	  when BTF=1 SCL will be stretched (pulled to LOW)
	while ((I2C_GetSR1FlagStatus(pI2CHandle->pI2Cx, I2C_BTF_FLAG) == FLAG_RESET) ||
			(I2C_GetSR1FlagStatus(pI2CHandle->pI2Cx, I2C_TxE_FLAG) == FLAG_RESET));

	// 8. Generate STOP condition and master need not to wait for the completion of stop condition
	//    Note: generating STOP automatically clears the BTF
	I2C_GenerateStopCondition(pI2CHandle->pI2Cx);

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
