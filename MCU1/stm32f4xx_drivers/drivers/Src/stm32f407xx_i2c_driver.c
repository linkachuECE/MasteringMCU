/*
 * stm32f407xx_i2c_driver.c
 *
 *  Created on: Sep 21, 2022
 *      Author: linkachu
 */

#include "stm32f407xx.h"
#include "stm32f407xx_i2c_driver.h"

// STATIC FUNCTIONS

static void I2C_MasterHandleRXNEInterrupt(I2C_Handle_t* pI2CHandle);
static void I2C_MasterHandleTXEInterrupt(I2C_Handle_t* pI2CHandle);

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
static void I2C_ClearADDRFlag(I2C_Handle_t *pI2CHandle){
	uint32_t dummy_read;
	// Check for device mode
	if(pI2CHandle->pI2Cx->SR2 & (1 << I2C_SR2_MSL)){
		// device is in master mode
		if(pI2CHandle->TxRxState == I2C_BUSY_IN_RX){
			if(pI2CHandle->RxSize == 1){
				// First disable the ACK
				I2C_ManageAcking(pI2CHandle->pI2Cx, I2C_ACK_DISABLE);

				// Clear the ADDR flag (Read SR1, read SR2)
				dummy_read = pI2CHandle->pI2Cx->SR1;
				dummy_read = pI2CHandle->pI2Cx->SR2;
				(void)dummy_read;
			} else {
				// Clear the ADDR flag (Read SR1, read SR2)
				dummy_read = pI2CHandle->pI2Cx->SR1;
				dummy_read = pI2CHandle->pI2Cx->SR2;
				(void)dummy_read;
			}
		}
	} else {
		// device is in slave mode
		// Clear the ADDR flag (Read SR1, read SR2)
		dummy_read = pI2CHandle->pI2Cx->SR1;
		dummy_read = pI2CHandle->pI2Cx->SR2;
		(void)dummy_read;
	}
}

// Main API's

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
 * @fn			- I2C_ManageAcking
 *
 * @brief		- This static function sets the ACK or NACK bit for I2C reception
 *
 * @param[in]	- Pointer to I2C Peripheral base address
 * @param[in]	- ACK or NACK
 *
 * @return		- none
 *
 * @Note		- none
 */
void I2C_ManageAcking(I2C_RegDef_t *pI2Cx, uint8_t ackOrNack){
	if(ackOrNack == I2C_ACK_ENABLE)
		pI2Cx->CR1 |= (1 << I2C_CR1_ACK);
	else
		pI2Cx->CR1 &= ~(1 << I2C_CR1_ACK);
}

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
void I2C_GenerateStartCondition(I2C_RegDef_t *pI2Cx){
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
void I2C_GenerateStopCondition(I2C_RegDef_t *pI2Cx){
	pI2Cx->CR1 |= (1 << I2C_CR1_STOP);
}

/*****************************************************************
 * @fn			- I2C_MasterSendData
 *
 * @brief		- This function sends data via I2C
 *
 * @param[in]	- Pointer to I2C Handle
 * @param[in]	- Pointer to data buffer
 * @param[in]	- Length of data to send
 * @param[in]	- Address of slave to send data to
 * @param[in]	- Repeated start
 *
 *
 * @return		- none
 *
 * @Note		- none
 */
void I2C_MasterSendData(I2C_Handle_t *pI2CHandle, uint8_t *pTxbuffer, uint32_t len, uint8_t slaveAddr, uint8_t sr){
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
	I2C_ClearADDRFlag(pI2CHandle);

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
	if (sr == I2C_DISABLE_RS)
		I2C_GenerateStopCondition(pI2CHandle->pI2Cx);

}

/*****************************************************************
 * @fn			- I2C_MasterReceiveData
 *
 * @brief		- This function receives data via I2C
 *
 * @param[in]	- Pointer to I2C Handle
 * @param[in]	- Pointer to data buffer
 * @param[in]	- Length of Rx buffer
 * @param[in]	- Address of slave to receive data from
 * @param[in]	- Repeated start enable or disable
 *
 * @return		- none
 *
 * @Note		- none
 */
void I2C_MasterReceiveData(I2C_Handle_t *pI2CHandle, uint8_t *pRxBuffer, uint32_t len, uint8_t slaveAddr, uint8_t sr){
	// 1. Generate start condition
	I2C_GenerateStartCondition(pI2CHandle->pI2Cx);

	// 2. Confirm that start generation is completed by checking the SB flag in the SR1
	// Note: Until SB is cleared SCL will be stretched
	while (I2C_GetSR1FlagStatus(pI2CHandle->pI2Cx, I2C_SB_FLAG) == FLAG_RESET);

	// 3. Send slave address
	I2C_ExecuteAddressPhase(pI2CHandle->pI2Cx, slaveAddr, READ);

	// 4. Confirm that Ack bit is received by checking addr bit
	while (I2C_GetSR1FlagStatus(pI2CHandle->pI2Cx, I2C_ADDR_FLAG) == FLAG_RESET);

	// Procedure to read only 1 byte from slave
	if (len == 1){
		// Disable ACKing
		I2C_ManageAcking(pI2CHandle->pI2Cx, I2C_ACK_DISABLE);

		// Clear the ADDR flag
		I2C_ClearADDRFlag(pI2CHandle);

		// Wair until RxNE becomes 1
		while(I2C_GetSR1FlagStatus(pI2CHandle->pI2Cx, I2C_RxNE_FLAG) == FLAG_RESET)

		// Generate STOP condition
		if (sr == I2C_DISABLE_RS)
			I2C_GenerateStopCondition(pI2CHandle->pI2Cx);

		// Read data into buffer
		*pRxBuffer = (uint8_t)(pI2CHandle->pI2Cx->DR);
	}
	// Procedure to read multiple bytes from slave
	else {
		// Clear the ADDR flag
		I2C_ClearADDRFlag(pI2CHandle);

		// Read the data until len becomes zero
		for (uint32_t i = len; i > 0; i--){
			// Wait until RXnE becomes 1
			while (I2C_GetSR1FlagStatus(pI2CHandle->pI2Cx, I2C_RxNE_FLAG) == FLAG_RESET);

			if(i == 2){ // if the last 2 bytes are remaining
				// Clear the ack bit
				I2C_ManageAcking(pI2CHandle->pI2Cx, I2C_ACK_DISABLE);

				// Generate STOP condition
				if (sr == I2C_DISABLE_RS)
					I2C_GenerateStopCondition(pI2CHandle->pI2Cx);
			}

			// Read the data from data register into buffer and increment buffer
			*(pRxBuffer++) = (uint8_t)(pI2CHandle->pI2Cx->DR);
		}
	}

	// Re-enable ACKing
	if(pI2CHandle->I2C_Config.AckControl == I2C_ACK_ENABLE)
		I2C_ManageAcking(pI2CHandle->pI2Cx, I2C_ACK_ENABLE);
}

/*****************************************************************
 * @fn			- I2C_SlaveSendData
 *
 * @brief		- This function sends data via I2C in slave mode
 *
 * @param[in]	- Pointer to base address of I2C peripheral
 * @param[in]	- Data to send
 *
 * @return		- Data received
 *
 * @Note		- none
 */
void I2C_SlaveSendData(I2C_RegDef_t *pI2Cx, uint8_t data){
	pI2Cx->DR = data;
}

/*****************************************************************
 * @fn			- I2C_SlaveReceiveData
 *
 * @brief		- This function receives data via I2C in slave mode
 *
 * @param[in]	- Pointer to base address of I2C peripheral
 *
 * @return		- Data received
 *
 * @Note		- none
 */
uint8_t I2C_SlaveReceiveData(I2C_RegDef_t *pI2Cx){
	return (uint8_t)pI2Cx->DR;
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
void I2C_PeripheralControl(I2C_Handle_t* pI2CHandle, uint8_t EnorDi){
	if (EnorDi == ENABLE){
		pI2CHandle->pI2Cx->CR1 |= (1 << I2C_CR1_PE);
		I2C_ManageAcking(pI2CHandle->pI2Cx, pI2CHandle->I2C_Config.AckControl);
	}
	else
		pI2CHandle->pI2Cx->CR1 &= ~(1 << I2C_CR1_PE);
}

// INTERRUPT FUNCTIONS

/*****************************************************************
 * @fn			- I2C_IRQInterruptConfig
 *
 * @brief		- This functions enables or disables the interrupt for a I2C periph. and sets the priority
 *
 * @param[in]	- Macro which pertains to either a I2C peripheral's event interrupts or error interrupts
 * @param[in]	- Priority level
 * @param[in]	- ENABLE or DISABLE
 *
 * @return		- none
 *
 * @Note		- none
 */
void I2C_IRQInterruptConfig(uint8_t IRQNumber, uint32_t IRQPriority, uint8_t EnorDi){
	if (EnorDi == ENABLE){
		if (IRQNumber < 32)
			*NVIC_ISER0 |= (1 << IRQNumber);
		else if (IRQNumber >= 32 && IRQNumber < 64)
			*NVIC_ISER1 |= (1 << (IRQNumber % 32));
		else if (IRQNumber >= 64 && IRQNumber < 96)
			*NVIC_ISER2 |= (1 << (IRQNumber % 32));
	} else if (EnorDi == DISABLE){
		if (IRQNumber < 32)
			*NVIC_ICER0 |= (1 << IRQNumber);
		else if (IRQNumber >= 32 && IRQNumber < 64)
			*NVIC_ICER1 |= (1 << (IRQNumber % 32));
		else if (IRQNumber >= 64 && IRQNumber < 96)
			*NVIC_ICER2 |= (1 << (IRQNumber % 32));
	}

	I2C_IRQPriorityConfig(IRQNumber, IRQPriority);
}

/*****************************************************************
 * @fn			- I2C_IRQPriorityConfig
 *
 * @brief		- This function sets the priority for an I2C IRQ number
 *
 * @param[in]	- Macro which pertains to the IRQ number for either a I2C peripheral's event interrupts or error interrupts
 * @param[in]	- Priority level
 *
 * @return		- none
 *
 * @Note		- none
 */
void I2C_IRQPriorityConfig(uint8_t IRQNumber, uint32_t IRQPriority){
	uint8_t iprx = IRQNumber / 4;
	uint8_t iprx_section = IRQNumber % 4;

	uint8_t shift_amount = ( 8 * iprx_section ) + (8 - NO_PRIORITY_BITS_IMPLEMENTED);

	__vo uint32_t* iprReg = (__vo uint32_t*)(NVIC_IPR_BASE_ADDR + (4 * iprx));

	*iprReg |= (IRQPriority << (shift_amount));
}

/*****************************************************************
 * @fn			- I2C_MasterHandleTXEInterrupt
 *
 * @brief		- This helper function handles a TXE interrupt
 *
 * @param[in]	- Pointer to I2C handle
 *
 * @return		- none
 *
 * @Note		- none
 */

static void I2C_MasterHandleTXEInterrupt(I2C_Handle_t* pI2CHandle){
	if(pI2CHandle->TxLen > 0){
		// Load the data into DR
		pI2CHandle->pI2Cx->DR = *(pI2CHandle->pTxBuffer);

		// Decrement TxLen
		pI2CHandle->TxLen--;

		// Increment the buffer address
		pI2CHandle->pTxBuffer++;
	}
}

/*****************************************************************
 * @fn			- I2C_MasterHandleRXNEInterrupt
 *
 * @brief		- This helper function handles an RXNE interrupt
 *
 * @param[in]	- Pointer to I2C handle
 *
 * @return		- none
 *
 * @Note		- none
 */
static void I2C_MasterHandleRXNEInterrupt(I2C_Handle_t* pI2CHandle){
	// We have to do data reception
	if(pI2CHandle->RxSize == 1){
		*(pI2CHandle->pRxBuffer) = pI2CHandle->pI2Cx->DR;

		pI2CHandle->RxLen--;
	} else if (pI2CHandle->RxSize > 1){
		if(pI2CHandle->RxLen == 2){ // if the last 2 bytes are remaining
			// Clear the ack bit
			I2C_ManageAcking(pI2CHandle->pI2Cx, I2C_ACK_DISABLE);
		}

		*pI2CHandle->pRxBuffer = pI2CHandle->pI2Cx->DR;
		pI2CHandle->pRxBuffer++;
		pI2CHandle->RxLen--;
	} else if (pI2CHandle->RxSize == 0){
		// Close the I2C data reception and notify the application

		// Generate the stop condition
		if(pI2CHandle->sr == I2C_DISABLE_RS)
			I2C_GenerateStopCondition(pI2CHandle->pI2Cx);

		// Close the I2C rx
		I2C_CloseReceiveData(pI2CHandle);

		// Notify the application
		I2C_ApplicationEventCallback(pI2CHandle, I2C_EV_RX_CMPLT);
	}
}

/*****************************************************************
 * @fn			- I2C_EV_IRQHandling
 *
 * @brief		- This function handles I2C IRQ Event Interrupts
 *
 * @param[in]	- Pointer to I2C handle
 *
 * @return		- none
 *
 * @Note		- none
 */
void I2C_EV_IRQHandling(I2C_Handle_t *pI2CHandle){
	uint32_t temp1, temp2, temp3;

	temp1 = pI2CHandle->pI2Cx->CR2 & (1 << I2C_CR2_ITEVTEN);
	temp2 = pI2CHandle->pI2Cx->CR2 & (1 << I2C_CR2_ITBUFEN);

	// Handle for interrupt generate by SB event
	// Note: SB flag is only applicable in Master mode
	temp3 = pI2CHandle->pI2Cx->SR1 & (1 << I2C_SR1_SB);
	if(temp1 && temp3){
		// SB flag is set
		if(pI2CHandle->TxRxState == I2C_BUSY_IN_RX){
			I2C_ExecuteAddressPhase(pI2CHandle->pI2Cx, pI2CHandle->DevAddr, READ);
		} else if (pI2CHandle->TxRxState == I2C_BUSY_IN_TX){
			I2C_ExecuteAddressPhase(pI2CHandle->pI2Cx, pI2CHandle->DevAddr, WRITE);
		}
	}

	// Handle for interrupt generated by ADDR event
	// Note: When master mode, address is sent
	// 		 When slave mode, address matched with own address
	temp3 = pI2CHandle->pI2Cx->SR1 & (1 << I2C_SR1_ADDR);
	if(temp1 && temp3){
		// ADDR flag is set
		I2C_ClearADDRFlag(pI2CHandle);
	}

	// Handle for interrupt gernerated by BTF (Byte Transfer Finished) event
	temp3 = pI2CHandle->pI2Cx->SR1 & (1 << I2C_SR1_BTF);
	if(temp1 && temp3){
		// BTF flag is set
		if(pI2CHandle->TxRxState == I2C_BUSY_IN_RX){

		} else if (pI2CHandle->TxRxState == I2C_BUSY_IN_TX){
			// Check if TX is empty, meaning that the transmission is over
			if(pI2CHandle->pI2Cx->SR1 & (1 << I2C_SR1_TxE)){
				// BTF & TXE = 1
				if(pI2CHandle->TxLen == 0){
					// Generate the stop condition
					if(pI2CHandle->sr == I2C_DISABLE_RS)
					I2C_GenerateStopCondition(pI2CHandle->pI2Cx);

					// Reset all the member elements of the handle structure
					I2C_CloseSendData(pI2CHandle);

					// Notify the application the transmission has been complete
					I2C_ApplicationEventCallback(pI2CHandle, I2C_EV_TX_CMPLT);
				}
			}

		}
	}

	// Handle for interrupt generated by STOPF event
	// Note: Stop detection flag is applicable only in slave mode.
	temp3 = pI2CHandle->pI2Cx->SR1 & (1 << I2C_SR1_STOPF);
	if(temp1 && temp3){
		// STOF flag is set
		// Clear the STOPF by reading SR1 then writing to CR1
		pI2CHandle->pI2Cx-> CR1 |= 0x0000;

		// Notify the application that STOP is detected
		I2C_ApplicationEventCallback(pI2CHandle, I2C_EV_STOP);
	}

	// Handle for interrupt generated by TXE event
	// Note: This interrupt will only go off if both ITEVTEN and ITBUFEN are set
	temp3 = pI2CHandle->pI2Cx->SR1 & (1 << I2C_SR1_TxE);
	if (temp1 && temp2 && temp3){
		// TXE flag is set
		// Check for device mode
		if(pI2CHandle->pI2Cx->SR1 & (1 << I2C_SR2_MSL) ){
			// Device is in master mode
			// Data transmission
			if(pI2CHandle->TxRxState == I2C_BUSY_IN_TX){
				I2C_MasterHandleTXEInterrupt(pI2CHandle);
			}
		} else {
			// Device is in slave mode
			// Make sure that the slave is in transmitter mode
			if(pI2CHandle->pI2Cx->SR2 & (1 << I2C_SR2_TRA))
				I2C_ApplicationEventCallback(pI2CHandle, I2C_EV_DATA_REQ);
		}
	}

	// Handle for interrupt generated by RXNE event
	// Note: This interrupt will only go off if both ITEVTEN and ITBUFEN are set
	temp3 = pI2CHandle->pI2Cx->SR1 & (1 << I2C_SR1_RxNE);
	if (temp1 && temp2 && temp3){
		// Check device mode
		if(pI2CHandle->pI2Cx->SR2 & (1 << I2C_SR2_MSL)){
			// Device is in master mode
			// RXNE flag is set
			if(pI2CHandle->TxRxState == I2C_BUSY_IN_RX){
				// Currently receiving
				I2C_MasterHandleRXNEInterrupt(pI2CHandle);
			}
		} else {
			// Device is in slave mode
			if(!(pI2CHandle->pI2Cx->SR2 & (1 << I2C_SR2_TRA)))
				I2C_ApplicationEventCallback(pI2CHandle, I2C_EV_DATA_RCV);
		}
	}
}

/*****************************************************************
 * @fn			- I2C_ER_IRQHandling
 *
 * @brief		- This function handles I2C IRQ Error Interrupts
 *
 * @param[in]	- Pointer to I2C handle
 *
 * @return		- none
 *
 * @Note		- none
 */
void I2C_ER_IRQHandling(I2C_Handle_t *pI2CHandle)
{

	uint32_t temp1,temp2;

    //Know the status of ITERREN control bit in the CR2
	temp2 = (pI2CHandle->pI2Cx->CR2) & ( 1 << I2C_CR2_ITERREN);


/***********************Check for Bus error************************************/
	temp1 = (pI2CHandle->pI2Cx->SR1) & ( 1<< I2C_SR1_BERR);
	if(temp1  && temp2 )
	{
		// This is Bus error

		// Implement the code to clear the buss error flag
		pI2CHandle->pI2Cx->SR1 &= ~( 1 << I2C_SR1_BERR);

		// Implement the code to notify the application about the error
	   I2C_ApplicationEventCallback(pI2CHandle, I2C_ERROR_BERR);
	}

/***********************Check for arbitration lost error************************************/
	temp1 = (pI2CHandle->pI2Cx->SR1) & ( 1 << I2C_SR1_ARLO );
	if(temp1  && temp2)
	{
		// This is arbitration lost error

		// Implement the code to clear the arbitration lost error flag
		pI2CHandle->pI2Cx->SR1 &= ~( 1 << I2C_SR1_ARLO);

		// Implement the code to notify the application about the error
		I2C_ApplicationEventCallback(pI2CHandle, I2C_ERROR_ARLO);
	}

/***********************Check for ACK failure error************************************/

	temp1 = (pI2CHandle->pI2Cx->SR1) & ( 1 << I2C_SR1_AF);
	if(temp1  && temp2)
	{
		// This is ACK failure error

	    // Implement the code to clear the ACK failure error flag
		pI2CHandle->pI2Cx->SR1 &= ~( 1 << I2C_SR1_AF);
		// Implement the code to notify the application about the error
		I2C_ApplicationEventCallback(pI2CHandle, I2C_ERROR_AF);
	}

/***********************Check for Overrun/underrun error************************************/
	temp1 = (pI2CHandle->pI2Cx->SR1) & ( 1 << I2C_SR1_OVR);
	if(temp1  && temp2)
	{
		// This is Overrun/underrun

	    // Implement the code to clear the Overrun/underrun error flag
		pI2CHandle->pI2Cx->SR1 &= ~( 1 << I2C_SR1_OVR);
		// Implement the code to notify the application about the error
		I2C_ApplicationEventCallback(pI2CHandle, I2C_ERROR_OVR);
	}

/***********************Check for Time out error************************************/
	temp1 = (pI2CHandle->pI2Cx->SR1) & ( 1 << I2C_SR1_TIME_OUT);
	if(temp1  && temp2)
	{
		// This is Time out error

	    // Implement the code to clear the Time out error flag
		pI2CHandle->pI2Cx->SR1 &= ~( 1 << I2C_SR1_TIME_OUT);
		// Implement the code to notify the application about the error
		I2C_ApplicationEventCallback(pI2CHandle, I2C_ERROR_TIMEOUT);
	}

}

// Interrupt send and receive API's
/*****************************************************************
 * @fn			- I2C_MasterSendDataIT
 *
 * @brief		- This function Sends data from an irq
 *
 * @param[in]	- Pointer to I2C Handle
 * @param[in]	- Pointer to data buffer
 * @param[in]	- Length of Tx buffer
 * @param[in]	- Address of slave to send data to
 * @param[in]	- Repeated start enable or disable
 *
 * @return		- Status
 *
 * @Note		- none
 */
uint8_t  I2C_MasterSendDataIT(I2C_Handle_t *pI2CHandle, uint8_t *pTxBuffer, uint32_t len, uint8_t slaveAddr, uint8_t sr)
{

	uint8_t busystate = pI2CHandle->TxRxState;

	if( (busystate != I2C_BUSY_IN_TX) && (busystate != I2C_BUSY_IN_RX))
	{
		pI2CHandle->pTxBuffer = pTxBuffer;
		pI2CHandle->TxLen = len;
		pI2CHandle->TxRxState = I2C_BUSY_IN_TX;
		pI2CHandle->DevAddr = slaveAddr;
		pI2CHandle->sr = sr;

		//Implement code to Generate START Condition
		I2C_GenerateStartCondition(pI2CHandle->pI2Cx);

		//Implement the code to enable ITBUFEN Control Bit
		pI2CHandle->pI2Cx->CR2 |= ( 1 << I2C_CR2_ITBUFEN);

		//Implement the code to enable ITEVFEN Control Bit
		pI2CHandle->pI2Cx->CR2 |= ( 1 << I2C_CR2_ITEVTEN);

		//Implement the code to enable ITERREN Control Bit
		pI2CHandle->pI2Cx->CR2 |= ( 1 << I2C_CR2_ITERREN);

	}

	return busystate;

}

/*****************************************************************
 * @fn			- I2C_MasterReceiveDataIT
 *
 * @brief		- This function receives data from an IRQ
 *
 * @param[in]	- Pointer to I2C Handle
 * @param[in]	- Pointer to data buffer
 * @param[in]	- Length of Rx buffer
 * @param[in]	- Address of slave to receive data from
 * @param[in]	- Repeated start enable or disable
 *
 * @return		- Status
 *
 * @Note		- none
 */
uint8_t I2C_MasterReceiveDataIT(I2C_Handle_t *pI2CHandle, uint8_t *pRxBuffer, uint32_t len, uint8_t slaveAddr, uint8_t sr){

	uint8_t busystate = pI2CHandle->TxRxState;

	if( (busystate != I2C_BUSY_IN_TX) && (busystate != I2C_BUSY_IN_RX))
	{
		pI2CHandle->pRxBuffer = pRxBuffer;
		pI2CHandle->RxLen = len;
		pI2CHandle->TxRxState = I2C_BUSY_IN_RX;
		pI2CHandle->RxSize = len; //Rxsize is used in the ISR code to manage the data reception
		pI2CHandle->DevAddr = slaveAddr;
		pI2CHandle->sr = sr;

		//Implement code to Generate START Condition
		I2C_GenerateStartCondition(pI2CHandle->pI2Cx);

		//Implement the code to enable ITBUFEN Control Bit
		pI2CHandle->pI2Cx->CR2 |= ( 1 << I2C_CR2_ITBUFEN);

		//Implement the code to enable ITEVFEN Control Bit
		pI2CHandle->pI2Cx->CR2 |= ( 1 << I2C_CR2_ITEVTEN);

		//Implement the code to enable ITERREN Control Bit
		pI2CHandle->pI2Cx->CR2 |= ( 1 << I2C_CR2_ITERREN);

	}

	return busystate;
}

// Close communications

/*****************************************************************
 * @fn			- I2C_CloseSendData
 *
 * @brief		- This function closes communications on a transmission
 *
 * @param[in]	- Pointer to I2C Handle
 *
 * @return		- Status
 *
 * @Note		- none
 */
void I2C_CloseSendData(I2C_Handle_t *pI2CHandle){\
	// Disable I2BUFEN control bit
	pI2CHandle->pI2Cx->CR2 &= ~(1 << I2C_CR2_ITBUFEN);

	// Disable ITEVFEN Control bit
	pI2CHandle->pI2Cx->CR2 &= ~(1 << I2C_CR2_ITEVTEN);

	pI2CHandle->TxRxState = I2C_READY;
	pI2CHandle->pRxBuffer = NULL;
	pI2CHandle->TxLen = 0;
}

/*****************************************************************
 * @fn			- I2C_CloseReceiveData
 *
 * @brief		- This function closes communications on a reception
 *
 * @param[in]	- Pointer to I2C Handle
 *
 * @return		- Status
 *
 * @Note		- none
 */
void I2C_CloseReceiveData(I2C_Handle_t *pI2CHandle){
	// Disable I2BUFEN control bit
	pI2CHandle->pI2Cx->CR2 &= ~(1 << I2C_CR2_ITBUFEN);

	// Disable ITEVFEN Control bit
	pI2CHandle->pI2Cx->CR2 &= ~(1 << I2C_CR2_ITEVTEN);

	pI2CHandle->TxRxState = I2C_READY;
	pI2CHandle->pRxBuffer = NULL;
	pI2CHandle->RxLen = 0;
	pI2CHandle->RxSize = 0;
	if(pI2CHandle->I2C_Config.AckControl == I2C_ACK_ENABLE)
		I2C_ManageAcking(pI2CHandle->pI2Cx, ENABLE);
}
