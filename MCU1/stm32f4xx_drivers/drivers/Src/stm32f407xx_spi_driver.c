/*
 * stm32407xx_spi_driver.c
 *
 *  Created on: Sep 10, 2022
 *      Author: linkachu
 */

#include "stm32f407xx_spi_driver.h"
#include "stm32f407xx.h"


// HELPER FUNCTION PROTOTYPES
static void spi_txe_interrupt_handle(SPI_Handle_t *pSPIHandle);
static void spi_rxne_interrupt_handle(SPI_Handle_t *pSPIHandle);
static void spi_ovr_err_interrupt_handle(SPI_Handle_t *pSPIHandle);

/*****************************************************************
 * @fn			- SPI_PeriClockControl
 *
 * @brief		- This function enables or disables peripheral clock for the given SPI line
 *
 * @param[in]	- Base address of the SPI peripheral
 * @param[in]	- ENABLE or DISABLE macros
 *
 * @return		- none
 *
 * @Note		- none
 */
void SPI_PeriClockControl(SPI_RegDef_t *pSPIx, uint8_t EnorDi){
	if(EnorDi == ENABLE){
		if(pSPIx == SPI1)
			SPI1_PCLK_EN();
		else if (pSPIx == SPI2)
			SPI2_PCLK_EN();
		else if (pSPIx == SPI3)
			SPI3_PCLK_EN();
	} else if (EnorDi == DISABLE){
		if(pSPIx == SPI1)
			SPI1_PCLK_DI();
		else if (pSPIx == SPI2)
			SPI2_PCLK_DI();
		else if (pSPIx == SPI3)
			SPI3_PCLK_DI();
	}
}

/*****************************************************************
 * @fn			- SPI_Init
 *
 * @brief		- This function initializes a single SPI peripheral
 *
 * @param[in]	- SPI Handler struct
 *
 * @return		- none
 *
 * @Note		- none
 */
void SPI_Init(SPI_Handle_t *pSPIHandle){

	SPI_PeriClockControl(pSPIHandle->pSPIx, ENABLE);

	uint32_t tempreg = 0;

	// 1. Master/Slave configuration
	tempreg |= (pSPIHandle->SPIConfig.DeviceMode << SPI_CR1_MSTR);

	// 2. Bus configuration
	if (pSPIHandle->SPIConfig.BusConfig == SPI_BUS_CONFIG_FD){
		// Bi-directional mode should be cleared
		tempreg &= ~(1 << SPI_CR1_BIDI_MODE);
	}
	else if (pSPIHandle->SPIConfig.BusConfig == SPI_BUS_CONFIG_HD){
		// Bi-directional mode should be set
		tempreg |= (1 << SPI_CR1_BIDI_MODE);
	} else if (pSPIHandle->SPIConfig.BusConfig == SPI_BUS_CONFIG_SIMPLEX_RXONLY){
		// Bi-directional mode should be cleared
		tempreg &= ~(1 << SPI_CR1_BIDI_MODE);
		// RXONLY bit must be set
		tempreg |= (1 << SPI_CR1_RX_ONLY);
	}

	// 3. Clock speed configuration
	tempreg |= (pSPIHandle->SPIConfig.SclkSpeed << SPI_CR1_BR);

	// 4. DFF Configuration
	tempreg |= (pSPIHandle->SPIConfig.DFF << SPI_CR1_DFF);

	// 5. CPOL Configuration
	tempreg |= (pSPIHandle->SPIConfig.CPOL << SPI_CR1_CPOL);

	// 6. CPHA Configuration
	tempreg |= (pSPIHandle->SPIConfig.CPHA << SPI_CR1_CPHA);

	// 7. SSM Configuration
	tempreg |= (pSPIHandle->SPIConfig.SSM << SPI_CR1_SSM);
	SPI_SSIControl(pSPIHandle->pSPIx, ENABLE);

	// 8. Frame Format Configuration
	tempreg |= (pSPIHandle->SPIConfig.FrameFormat << SPI_CR1_LSB_FIRST);

	pSPIHandle->pSPIx->CR1 |= tempreg;
}

/*****************************************************************
 * @fn			- SPI_DeInit
 *
 * @brief		- This function resets a SPI peripheral
 *
 * @param[in]	- Pointer to SPI peripheral base address
 *
 * @return		- none
 *
 * @Note		- none
 */
void SPI_DeInit(SPI_RegDef_t *pSPIx){
	if(pSPIx == SPI1)
		SPI1_REG_RESET();
	else if (pSPIx == SPI2)
		SPI2_REG_RESET();
	else if (pSPIx == SPI3)
		SPI3_REG_RESET();
}

//Enable and Disable

/*****************************************************************
 * @fn			- SPI_PeripheralControl
 *
 * @brief		- This function enables or disables a SPI peripheral
 *
 * @param[in]	- Pointer to SPI peripheral base address
 * @param[in]	- ENABLE or DISABLE
 *
 * @return		- none
 *
 * @Note		- none
 */
void SPI_PeripheralControl(SPI_RegDef_t * pSPIx, uint8_t EnorDi){
	if (EnorDi == ENABLE)
		pSPIx->CR1 |= (1 << SPI_CR1_SPE);
	else
		pSPIx->CR1 &= ~(1 << SPI_CR1_SPE);
}

/*****************************************************************
 * @fn			- SPI_SSIControl
 *
 * @brief		- This function enables or disables the SSI (Internal Slave Select) bit
 *
 * @param[in]	- Pointer to SPI peripheral base address
 * @param[in]	- ENABLE or DISABLE
 *
 * @return		- none
 *
 * @Note		- none
 */
void SPI_SSIControl(SPI_RegDef_t * pSPIx, uint8_t EnorDi){
	if (EnorDi == ENABLE)
		pSPIx->CR1 |= (1 << SPI_CR1_SSI);			// Pull SSI high
	else
		pSPIx->CR1 &= ~(1 << SPI_CR1_SSI);
}

/*****************************************************************
 * @fn			- SPI_SSOEControl
 *
 * @brief		- This function enables or disables the SSOE (SS Output Enable) bit
 *
 * @param[in]	- Pointer to SPI peripheral base address
 * @param[in]	- ENABLE or DISABLE
 *
 * @return		- none
 *
 * @Note		- none
 */
void SPI_SSOEControl(SPI_RegDef_t * pSPIx, uint8_t EnorDi){
	if (EnorDi == ENABLE)
		pSPIx->CR2 |= (1 << SPI_CR2_SSOE);
	else
		pSPIx->CR2 &= ~(1 << SPI_CR2_SSOE);
}

// Data send and receive

/*****************************************************************
 * @fn			- SPI_GetFlagStatus
 *
 * @brief		- This function checks the value of a flag in the SPI status register
 *
 * @param[in]	- Pointer to SPI peripheral base address
 * @param[in]	- Flag name macro
 *
 * @return		- Boolean which says whether the FLAG_SET or FLAG_RESET
 *
 * @Note		- none
 */
uint8_t SPI_GetFlagStatus(SPI_RegDef_t *pSPIx, uint32_t flagName){
	if (pSPIx->SR & flagName)
		return FLAG_SET;
	else
		return FLAG_RESET;
}

/*****************************************************************
 * @fn			- SPI_SendData
 *
 * @brief		- This sends information via SPI
 *
 * @param[in]	- Pointer to SPI peripheral base address
 * @param[in]	- Pointer to buffer containing information to send
 * @param[in]	- Length of buffer
 *
 * @return		- none
 *
 * @Note		- none
 */
void SPI_SendData(SPI_RegDef_t *pSPIx, uint8_t* pTxBuffer, uint32_t len){
	// Dummy variable for reading from the data register
	// uint32_t dummy;

	while(len > 0){
		// 1. Wait until TXE is set
		while(SPI_GetFlagStatus(pSPIx, SPI_TXE_FLAG) == FLAG_RESET)
			;

		// 2. Check the DFF bit in CR1
		if(pSPIx->CR1 & (1 << SPI_CR1_DFF)){
			pSPIx->DR = *((uint16_t*)pTxBuffer);
			pTxBuffer += 2;
		} else {
			pSPIx->DR = *((uint8_t*)pTxBuffer);
			pTxBuffer++;
		}
		len--;
	}
}

/*****************************************************************
 * @fn			- SPI_ReceiveData
 *
 * @brief		- This function receives data from a peripheral via SPI
 *
 * @param[in]	- Pointer to SPI peripheral base address
 * @param[in]	- Pointer to buffer to store information in
 * @param[in]	- Length of receiving buffer
 *
 * @return		- none
 *
 * @Note		- none
 */
void SPI_ReceiveData(SPI_RegDef_t *pSPIx, uint8_t* pRxBuffer, uint32_t len){
	while(len > 0){
		// 1. Wait until RXNE is set
		while(SPI_GetFlagStatus(pSPIx, SPI_RXNE_FLAG) == FLAG_RESET)
			;

		// 2. Check the DFF bit in CR1
		if(pSPIx->CR1 & (1 << SPI_CR1_DFF)){
			*((uint16_t*)pRxBuffer) = pSPIx->DR;
			pRxBuffer += 2;
		} else {
			*((uint8_t*)pRxBuffer) = pSPIx->DR;
			pRxBuffer++;
		}
		len--;
	}
}

// Data Send and Receive from interrupts

/*****************************************************************
 * @fn			- SPI_SendDataIT
 *
 * @brief		- This sends information via a SPI interrupt
 *
 * @param[in]	- Pointer to SPI Handle
 * @param[in]	- Pointer to buffer containing information to send
 * @param[in]	- Length of buffer
 *
 * @return		- none
 *
 * @Note		- none
 */
uint8_t SPI_SendDataIT(SPI_Handle_t *pSPIHandle, uint8_t* pTxBuffer, uint32_t len){
	uint8_t state = pSPIHandle->TxState;

	if(state != SPI_BUSY_IN_TX){
		// 1. Save the buffer to hold the tx data
		pSPIHandle->pTxBuffer = pTxBuffer;
		pSPIHandle->TxLen = len;

		// 2. Mark the SPI state as busy in transmission so that no other code
		// 	  can take over the same SPI peripheral until transmission is over
		pSPIHandle->TxState = SPI_BUSY_IN_TX;

		// 3. Enable the TXEIE control bit to get interrupt whenever TXE flag is set in SR
		pSPIHandle->pSPIx->CR2 |= (1 << SPI_CR2_TXEIE);

		// 4. Data Transmission will be handled by the ISR code
	}

	return state;
}

/*****************************************************************
 * @fn			- SPI_ReceiveDataIT
 *
 * @brief		- This function receives data from a peripheral via a SPI interrupt
 *
 * @param[in]	- Pointer to SPI Handle
 * @param[in]	- Pointer to buffer to store information in
 * @param[in]	- Length of receiving buffer
 *
 * @return		- none
 *
 * @Note		- none
 */
uint8_t SPI_ReceiveDataIT(SPI_Handle_t *pSPIHandle, uint8_t* pRxBuffer, uint32_t len){
	uint8_t state = pSPIHandle->TxState;

	if(state != SPI_BUSY_IN_RX){
		// 1. Save the buffer to hold the tx data
		pSPIHandle->pRxBuffer = pRxBuffer;
		pSPIHandle->RxLen = len;

		// 2. Mark the SPI state as busy in transmission so that no other code
		// 	  can take over the same SPI peripheral until transmission is over
		pSPIHandle->RxState = SPI_BUSY_IN_RX;

		// 3. Enable the TXEIE control bit to get interrupt whenever TXE flag is set in SR
		pSPIHandle->pSPIx->CR2 |= (1 << SPI_CR2_RXNEIE);

		// 4. Data Transmission will be handled by the ISR code
	}

	return state;
}

// IRQ Handling

/*****************************************************************
 * @fn			- SPI_IRQInterruptConfig
 *
 * @brief		- This function configures the interrupt settings for a SPI peripheral
 *
 * @param[in]	- IRQ Number
 * @param[in]	- Priority to set
 * @param[in]	- ENABLE or DISABLE
 *
 * @return		- none
 *
 * @Note		- none
 */
void SPI_IRQInterruptConfig(uint8_t IRQNumber, uint32_t IRQPriority, uint8_t EnorDi){
	// The enable bits for all three SPIs are in ISER1
	// The clear bits for all three SPIs are in ICER2
	if (EnorDi == ENABLE)
		*NVIC_ISER1 |= (1 << (IRQNumber % 32));
	else
		*NVIC_ICER1 |= (1 << (IRQNumber % 32));


	SPI_IRQPriorityConfig(IRQNumber, IRQPriority);
}

/*****************************************************************
 * @fn			- SPI_IRQPriorityConfig
 *
 * @brief		- This function sets the priority for a SPI IRQ number
 *
 * @param[in]	- IRQ Number
 * @param[in]	- IRQ Priority to set from 0-15
 *
 * @return		- none
 *
 * @Note		- none
 */
void SPI_IRQPriorityConfig(uint8_t IRQNumber, uint32_t IRQPriority){
	uint8_t iprx = IRQNumber / 4;
	uint8_t iprx_section = IRQNumber % 4;

	uint8_t shift_amount = ( 8 * iprx_section ) + (8 - NO_PRIORITY_BITS_IMPLEMENTED);

	__vo uint32_t* iprReg = (__vo uint32_t*)(NVIC_IPR_BASE_ADDR + (4 * iprx));

	*iprReg |= (IRQPriority << (shift_amount));
}

/*****************************************************************
 * @fn			- SPI_IRQHandling
 *
 * @brief		- TO BE FILLED IN
 *
 * @param[in]	- TO BE FILLED IN
 *
 * @return		- none
 *
 * @Note		- none
 */

void SPI_IRQHandling(SPI_Handle_t *pSPIHandle){
	uint8_t temp1, temp2;

	// First check for TXE and check that the TXE interrupt was enabled
	temp1 = pSPIHandle->pSPIx->SR & (SPI_TXE_FLAG);
	temp2 = pSPIHandle->pSPIx->CR2 & (1 << SPI_CR2_TXEIE);

	if (temp1 && temp2){
		// Handle TXE
		spi_txe_interrupt_handle(pSPIHandle);
	}

	temp1 = pSPIHandle->pSPIx->SR & (SPI_RXNE_FLAG);
	temp2 = pSPIHandle->pSPIx->CR2 & (1 << SPI_CR2_RXNEIE);

	if (temp1 && temp2){
		// Handle TXE
		spi_rxne_interrupt_handle(pSPIHandle);
	}

	temp1 = pSPIHandle->pSPIx->SR & (1 << SPI_SR_OVR);
	temp2 = pSPIHandle->pSPIx->CR2 & (1 << SPI_CR2_ERRIE);

	if (temp1 && temp2){
		// Handle TXE
		spi_ovr_err_interrupt_handle(pSPIHandle);
	}

}

/*****************************************************************
 * @fn			- SPI_ClearOVRFlag
 *
 * @brief		- Clears the OVR flag by reading from the data register, followed by the status register
 *
 * @param[in]	- Pointer to base address of SPI peripheral
 *
 * @return		- none
 *
 * @Note		- none
 */

void SPI_ClearOVRFlag(SPI_RegDef_t *pSPIx){
	uint8_t dummy;
	// 1. Clear the ovr flag
	dummy = pSPIx->DR;
	dummy = pSPIx->SR;
	(void)dummy;
}

/*****************************************************************
 * @fn			- SPI_CloseTransmission
 *
 * @brief		- Turns off the TXE interrupt, sets the tx buffer to NULL, and sets the transmission state to ready
 *
 * @param[in]	- Pointer to SPI Handler
 *
 * @return		- none
 *
 * @Note		- none
 */
void SPI_CloseTransmission(SPI_Handle_t *pSPIHandle){
	pSPIHandle->pSPIx->CR2 &= ~(1 << SPI_CR2_TXEIE);
	pSPIHandle->pTxBuffer = NULL;
	pSPIHandle->TxState = SPI_READY;
	SPI_ApplicationEventCallback(pSPIHandle, SPI_EVENT_TX_CMPLT);
}

/*****************************************************************
 * @fn			- SPI_CloseReception
 *
 * @brief		- Turns off the RXNE interrupt, sets the rx buffer to NULL, and sets the reception state to ready
 *
 * @param[in]	- Pointer to SPI Handler
 *
 * @return		- none
 *
 * @Note		- none
 */
void SPI_CloseReception(SPI_Handle_t *pSPIHandle){
	pSPIHandle->pSPIx->CR2 &= ~(1 << SPI_CR2_RXNEIE);
	pSPIHandle->pRxBuffer = NULL;
	pSPIHandle->RxState = SPI_READY;
	SPI_ApplicationEventCallback(pSPIHandle, SPI_EVENT_TX_CMPLT);
}

/*****************************************************************
 * @fn			- SPI_CloseReception
 *
 * @brief		- Turns off the RXNE interrupt, sets the rx buffer to NULL, and sets the reception state to ready
 *
 * @param[in]	- Pointer to SPI Handler
 *
 * @return		- none
 *
 * @Note		- none
 */
__weak void SPI_ApplicationEventCallback(SPI_Handle_t *pSPIHandle, uint8_t AppEv) {

}

// HELPER FUNCTION IMPLEMENTATIONS
static void spi_txe_interrupt_handle(SPI_Handle_t *pSPIHandle){
	// Check the DFF bit in CR1
	if(pSPIHandle->pSPIx->CR1 & (1 << SPI_CR1_DFF)){
		pSPIHandle->pSPIx->DR = *((uint16_t*)pSPIHandle->pTxBuffer);
		pSPIHandle->pTxBuffer += 2;
	} else {
		pSPIHandle->pSPIx->DR = *((uint8_t*)pSPIHandle->pTxBuffer);
		pSPIHandle->pTxBuffer++;
	}
	pSPIHandle->TxLen--;

	if (pSPIHandle->TxLen == 0){
		SPI_CloseTransmission(pSPIHandle);
		SPI_ApplicationEventCallback(pSPIHandle, SPI_EVENT_TX_CMPLT);
	}
}

static void spi_rxne_interrupt_handle(SPI_Handle_t *pSPIHandle){
	// 1. Check the DFF bit in CR1
	if(pSPIHandle->pSPIx->CR1 & (1 << SPI_CR1_DFF)){
		*((uint16_t*)pSPIHandle->pRxBuffer) = pSPIHandle->pSPIx->DR;
		pSPIHandle->pRxBuffer += 2;
	} else {
		*((uint8_t*)pSPIHandle->pRxBuffer) = pSPIHandle->pSPIx->DR;
		pSPIHandle->pRxBuffer++;
	}
	pSPIHandle->RxLen--;

	if (pSPIHandle->TxLen == 0){
		SPI_CloseReception(pSPIHandle);
		SPI_ApplicationEventCallback(pSPIHandle, SPI_EVENT_TX_CMPLT);
	}
}

static void spi_ovr_err_interrupt_handle(SPI_Handle_t *pSPIHandle){
	// 1. Clear the ovr flag
	if(pSPIHandle->TxState != SPI_BUSY_IN_TX){
		SPI_ClearOVRFlag(pSPIHandle->pSPIx);
		// 2. Inform the application
		SPI_ApplicationEventCallback(pSPIHandle, SPI_EVENT_TX_CMPLT);
	}

}
