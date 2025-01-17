/*
 * stm32f407xx_usart_driver.c
 *
 *  Created on: Oct 12, 2022
 *      Author: ebraun
 */

#include "stm32f407xx_usart_driver.h"
#include "stm32f407xx_rcc_driver.h"
#include "stm32f407xx.h"

/*****************************************************************
 * @fn			- USART_PeriClockControl
 *
 * @brief		- This function enables or disables peripheral clock for the given USART Peripheral
 *
 * @param[in]	- Base address of the USART peripheral
 * @param[in]	- ENABLE or DISABLE macro
 *
 * @return		- none
 *
 * @Note		- none
 */
void USART_PeriClockControl(USART_RegDef_t *pUSARTx, uint8_t EnOrDi){
	if(EnOrDi == ENABLE){
		if(pUSARTx == USART1)
			USART1_PCLK_EN();
		else if(pUSARTx == USART2)
			USART2_PCLK_EN();
		else if(pUSARTx == USART3)
			USART3_PCLK_EN();
		else if(pUSARTx == UART4)
			UART4_PCLK_EN();
		else if(pUSARTx == UART5)
			UART5_PCLK_EN();
		else if(pUSARTx == USART6)
			USART6_PCLK_EN();
	} else if (EnOrDi == DISABLE){
		if(pUSARTx == USART1)
			USART1_PCLK_DI();
		else if(pUSARTx == USART2)
			USART2_PCLK_DI();
		else if(pUSARTx == USART3)
			USART3_PCLK_DI();
		else if(pUSARTx == UART4)
			UART4_PCLK_DI();
		else if(pUSARTx == UART5)
			UART5_PCLK_DI();
		else if(pUSARTx == USART6)
			USART6_PCLK_DI();
	}
}

/*****************************************************************
 * @fn			- USART_PeripheralControl
 *
 * @brief		- This function enables or disables a USART peripheral
 *
 * @param[in]	- Pointer to USART peripheral base address
 * @param[in]	- ENABLE or DISABLE macro
 *
 * @return		- none
 *
 * @Note		- none
 */
void USART_PeripheralControl(USART_RegDef_t *pUSARTx, uint8_t EnOrDi){
	if(EnOrDi == ENABLE)
		pUSARTx->CR1 |= (1 << USART_CR1_UE);
	else
		pUSARTx->CR1 &= ~(1 << USART_CR1_UE);
}

/*****************************************************************
 * @fn			- USART_GetFlagStatus
 *
 * @brief		- This function returns the value currently held in a bit or field in a USART status register
 *
 * @param[in]	- Pointer to USART base address
 * @param[in]	- Flag name macro
 *
 * @return		- SET or RESET
 *
 * @Note		- none
 */
uint8_t USART_GetFlagStatus(USART_RegDef_t* pUSARTx, uint8_t StatusFlagName){
	if(pUSARTx->SR & StatusFlagName)
		return FLAG_SET;
	else
		return FLAG_RESET;
}

/*****************************************************************
 * @fn			- USART_ClearFlag
 *
 * @brief		- Clears the flag provided in argument 2
 *
 * @param[in]	- Pointer to USART base address
 * @param[in]	- Flag name macro
 *
 * @return		- none
 *
 * @Note		- none
 */
void USART_ClearFlag(USART_RegDef_t *pUSARTx, uint16_t StatusFlagName){

}

/*****************************************************************
 * @fn			- USART_SetBaudRate
 *
 * @brief		- Sets the baud rate of a USART peripheral
 *
 * @param[in]	- Pointer to USART base address
 * @param[in]	- Baud rate to set
 *
 * @return		- none
 *
 * @Note		- none
 */
void USART_SetBaudRate(USART_RegDef_t *pUSARTx, uint32_t BaudRate)
{
	// Variable to hold the APB clock
	uint32_t PCLKx, usartdiv;

	// Variables to hold Mantissa and Fraction values
	uint32_t M_part, F_part;

	uint32_t tempreg = 0;

	// Get the value of APB bus clock in to the variable PCLKx
	if(pUSARTx == USART1 || pUSARTx == USART6){
	   // USART1 and USART6 are hanging on APB2 bus
	   PCLKx = RCC_GetPCLK2Value();
	} else {
	   PCLKx = RCC_GetPCLK1Value();
	}

	// Check for OVER8 configuration bit
	if(pUSARTx->CR1 & (1 << USART_CR1_OVER8))
	{
	   // OVER8 = 1 , over sampling by 8
	   usartdiv = ((25 * PCLKx) / (2 *BaudRate));
	} else {
	   // Over sampling by 16
	   usartdiv = ((25 * PCLKx) / (4 *BaudRate));
	}

	// Calculate the Mantissa part
	M_part = usartdiv/100;

	// Place the Mantissa part in appropriate bit position . refer USART_BRR
	tempreg |= M_part << 4;

	// Extract the fraction part
	F_part = (usartdiv - (M_part * 100));

	// Calculate the final fractional
	if(pUSARTx->CR1 & ( 1 << USART_CR1_OVER8)) {
	  //OVER8 = 1 , over sampling by 8
	  F_part = ((( F_part * 8)+ 50) / 100)& ((uint8_t)0x07);
	} else {
	   // Over sampling by 16
	   F_part = ((( F_part * 16)+ 50) / 100) & ((uint8_t)0x0F);
	}

	// Place the fractional part in appropriate bit position . refer USART_BRR
	tempreg |= F_part;

	// Copy the value of tempreg in to BRR register
	pUSARTx->BRR = tempreg;
}

/*********************************************************************
 * @fn      		  - USART_Init
 *
 * @brief             - Intitializes a USART peripheral based on the configuration structure
 *
 * @param[in]         - USART Handle
 *
 * @return            - none
 *
 * @Note              - none

 */
void USART_Init(USART_Handle_t *pUSARTHandle){

	// Temporary variable
	uint32_t tempreg=0;

/******************************** Configuration of CR1******************************************/

	// Implement the code to enable the Clock for given USART peripheral
	USART_PeriClockControl(pUSARTHandle->pUSARTx, ENABLE);

	// Enable USART Tx and Rx engines according to the USART_Mode configuration item
	if ( pUSARTHandle->USART_Config.mode == USART_MODE_ONLY_RX) {
		// Implement the code to enable the Receiver bit field
		tempreg|= (1 << USART_CR1_RE);
	} else if (pUSARTHandle->USART_Config.mode == USART_MODE_ONLY_TX) {
		// Implement the code to enable the Transmitter bit field
		tempreg |= ( 1 << USART_CR1_TE );

	} else if (pUSARTHandle->USART_Config.mode == USART_MODE_TXRX) {
		// Implement the code to enable the both Transmitter and Receiver bit fields
		tempreg |= ( ( 1 << USART_CR1_TE) | ( 1 << USART_CR1_RE) );
	}

    // Implement the code to configure the Word length configuration item
	tempreg |= pUSARTHandle->USART_Config.wordLength << USART_CR1_M;


    // Configuration of parity control bit fields
	if ( pUSARTHandle->USART_Config.parityControl == USART_PARITY_EN_EVEN) {
		// Implement the code to enable the parity control
		tempreg |= ( 1 << USART_CR1_PCE);

		// Implement the code to enable EVEN parity

		tempreg &= ~(1 << USART_CR1_PS);
		// Not required because by default EVEN parity will be selected once you enable the parity control

	} else if (pUSARTHandle->USART_Config.parityControl == USART_PARITY_EN_ODD ) {
		// Implement the code to enable the parity control
	    tempreg |= ( 1 << USART_CR1_PCE);

	    // Implement the code to enable ODD parity
	    tempreg |= ( 1 << USART_CR1_PS);
	}

	// Program the CR1 register
	pUSARTHandle->pUSARTx->CR1 = tempreg;

/******************************** Configuration of CR2******************************************/

	tempreg=0;

	// Implement the code to configure the number of stop bits inserted during USART frame transmission
	tempreg |= pUSARTHandle->USART_Config.noOfStopBits << USART_CR2_STOP;

	// Program the CR2 register
	pUSARTHandle->pUSARTx->CR2 = tempreg;

/******************************** Configuration of CR3******************************************/

	tempreg = 0;

	// Configuration of USART hardware flow control
	if ( pUSARTHandle->USART_Config.HWFlowControl == USART_HW_FLOW_CTRL_CTS) {
		// Implement the code to enable CTS flow control
		tempreg |= ( 1 << USART_CR3_CTSE);
	} else if (pUSARTHandle->USART_Config.HWFlowControl == USART_HW_FLOW_CTRL_RTS) {
		// Implement the code to enable RTS flow control
		tempreg |= (1 << USART_CR3_RTSE);
	} else if (pUSARTHandle->USART_Config.HWFlowControl == USART_HW_FLOW_CTRL_CTS_RTS) {
		// Implement the code to enable both CTS and RTS Flow control
		tempreg |= ((1 << USART_CR3_RTSE) | (1 << USART_CR3_CTSE));
	}
	pUSARTHandle->pUSARTx->CR3 = tempreg;

/******************************** Configuration of BRR(Baudrate register)******************************************/

	// Implement the code to configure the baud rate
	// We will cover this in the lecture. No action required here
	USART_SetBaudRate(pUSARTHandle->pUSARTx, pUSARTHandle->USART_Config.baudRate);
}

/*********************************************************************
 * @fn      		  - USART_SendData
 *
 * @brief             -
 *
 * @param[in]         - Handle for USART peripheral
 * @param[in]         - Tx buffer
 * @param[in]         - Len of data to be sent
 *
 * @return            -
 *
 * @Note              - Resolve all the TODOs
 */
void USART_SendData(USART_Handle_t *pUSARTHandle, uint8_t *pTxBuffer, uint32_t len) {
	uint16_t *pdata;
   // Loop over until "Len" number of bytes are transferred
	for(uint32_t i = 0 ; i < len; i++) {
		//Implement the code to wait until TXE flag is set in the SR
		while(! USART_GetFlagStatus(pUSARTHandle->pUSARTx,USART_TxE_FLAG));
         // Check the USART_WordLength item for 9BIT or 8BIT in a frame
		if(pUSARTHandle->USART_Config.wordLength == USART_WORDLEN_9BITS) {
			//if 9BIT, load the DR with 2bytes masking the bits other than first 9 bits
			pdata = (uint16_t*) pTxBuffer;
			pUSARTHandle->pUSARTx->DR = (*pdata & (uint16_t)0x01FF);

			// Check for USART_ParityControl
			if(pUSARTHandle->USART_Config.parityControl == USART_PARITY_DISABLE) {
				// No parity is used in this transfer. so, 9bits of user data will be sent
				// Implement the code to increment pTxBuffer twice
				pTxBuffer++;
				pTxBuffer++;
			} else {
				// Parity bit is used in this transfer . so , 8bits of user data will be sent
				// The 9th bit will be replaced by parity bit by the hardware
				pTxBuffer++;
			}
		} else {
			// This is 8-bit data transfer
			pUSARTHandle->pUSARTx->DR = (*pTxBuffer  & (uint8_t)0xFF);

			// Implement the code to increment the buffer address
			pTxBuffer++;
		}
	}
	// Implement the code to wait till TC flag is set in the SR
	while( ! USART_GetFlagStatus(pUSARTHandle->pUSARTx,USART_TC_FLAG));
}

/*********************************************************************
 * @fn      		  - USART_ReceiveData
 *
 * @brief             - Receive data from USART peripheral
 *
 * @param[in]         - USART Handle Structure
 * @param[in]         - Rx buffer
 * @param[in]         - Length of data to be received
 *
 * @return            -
 *
 * @Note              -

 */

void USART_ReceiveData(USART_Handle_t *pUSARTHandle, uint8_t *pRxBuffer, uint32_t len) {
   // Loop over until "Len" number of bytes are transferred
	for(uint32_t i = 0 ; i < len; i++) {
		// Implement the code to wait until RXNE flag is set in the SR
		while(! USART_GetFlagStatus(pUSARTHandle->pUSARTx,USART_RxNE_FLAG));

		// Check the USART_WordLength to decide whether we are going to receive 9bit of data in a frame or 8 bit
		if(pUSARTHandle->USART_Config.wordLength == USART_WORDLEN_9BITS) {
			// We are going to receive 9bit data in a frame

			// Check are we using USART_ParityControl control or not
			if(pUSARTHandle->USART_Config.parityControl == USART_PARITY_DISABLE) {
				// No parity is used. so, all 9bits will be of user data

				// Read only first 9 bits. so, mask the DR with 0x01FF
				*((uint16_t*) pRxBuffer) = (pUSARTHandle->pUSARTx->DR  & (uint16_t)0x01FF);

				// Now increment the pRxBuffer two times
				pRxBuffer += 2;
			} else {
				// Parity is used, so, 8bits will be of user data and 1 bit is parity
				*pRxBuffer = (pUSARTHandle->pUSARTx->DR  & (uint8_t)0xFF);

				// Increment the pRxBuffer
				pRxBuffer++;
			}
		} else {
			// We are going to receive 8bit data in a frame

			// Check are we using USART_ParityControl control or not
			if(pUSARTHandle->USART_Config.parityControl == USART_PARITY_DISABLE){
				// No parity is used , so all 8bits will be of user data

				// Read 8 bits from DR
				 *pRxBuffer = (pUSARTHandle->pUSARTx->DR & (uint8_t)0xFF);
			} else {
				// Parity is used so 7 bits will be of user data and 1 bit is parity

				// Read only 7 bits , hence mask the DR with 0X7F
				 *pRxBuffer = (uint8_t)(pUSARTHandle->pUSARTx->DR & (uint8_t)0x7F);
			}
			//increment the pRxBuffer
			pRxBuffer++;
		}
	}

}

/*********************************************************************
 * @fn      		  - USART_SendDataWithIT
 *
 * @brief             - Send data via a USART peripheral using interrupts
 *
 * @param[in]         - USART Handle
 * @param[in]         - Tx Buffer
 * @param[in]         - Length of data to send
 *
 * @return            -
 *
 * @Note              - Resolve all the TODOs
 */
uint8_t USART_SendDataIT(USART_Handle_t *pUSARTHandle, uint8_t *pTxBuffer, uint32_t len) {
	uint8_t txstate = pUSARTHandle->TxBusyState;

	if(txstate != USART_BUSY_IN_TX) {
		pUSARTHandle->TxLen = len;
		pUSARTHandle->pTxBuffer = pTxBuffer;
		pUSARTHandle->TxBusyState = USART_BUSY_IN_TX;

		//Implement the code to enable interrupt for TXE
		pUSARTHandle->pUSARTx->CR1 |= (1 << USART_CR1_TXEIE);

		//Implement the code to enable interrupt for TC
		pUSARTHandle->pUSARTx->CR1 |= (1 << USART_CR1_TCIE);
	}
	return txstate;
}

/*********************************************************************
 * @fn      		  - USART_ReceiveDataIT
 *
 * @brief             - Receive data via a USART peripheral using interrupts
 *
 * @param[in]         - Pointer to USART handle
 * @param[in]         - Pointer to Rx buffer
 * @param[in]         -
 *
 * @return            -
 *
 * @Note              - Resolve all the TODOs
 */
uint8_t USART_ReceiveDataIT(USART_Handle_t *pUSARTHandle, uint8_t *pRxBuffer, uint32_t len)
{
	uint8_t rxstate = pUSARTHandle->RxBusyState;

	if(rxstate != USART_BUSY_IN_RX)
	{
		pUSARTHandle->RxLen = len;
		pUSARTHandle->pRxBuffer = pRxBuffer;
		pUSARTHandle->RxBusyState = USART_BUSY_IN_RX;

		//Implement the code to enable interrupt for RXNE
		pUSARTHandle->pUSARTx->CR1 |= (1 << USART_CR1_RXNEIE);
	}

	return rxstate;

}

/*****************************************************************
 * @fn			- USART_IRQInterruptConfig
 *
 * @brief		- This functions enables or disables the interrupt for a USART periph. and sets the priority
 *
 * @param[in]	- Macro which pertains to a USART peripheral's event interrupts or error interrupts
 * @param[in]	- ENABLE or DISABLE
 *
 * @return		- none
 *
 * @Note		- none
 */
void USART_IRQInterruptConfig(uint8_t IRQNumber, uint8_t EnOrDi){
	if (EnOrDi == ENABLE){
		if (IRQNumber < 32)
			*NVIC_ISER0 |= (1 << IRQNumber);
		else if (IRQNumber >= 32 && IRQNumber < 64)
			*NVIC_ISER1 |= (1 << (IRQNumber % 32));
		else if (IRQNumber >= 64 && IRQNumber < 96)
			*NVIC_ISER2 |= (1 << (IRQNumber % 32));
	} else if (EnOrDi == DISABLE){
		if (IRQNumber < 32)
			*NVIC_ICER0 |= (1 << IRQNumber);
		else if (IRQNumber >= 32 && IRQNumber < 64)
			*NVIC_ICER1 |= (1 << (IRQNumber % 32));
		else if (IRQNumber >= 64 && IRQNumber < 96)
			*NVIC_ICER2 |= (1 << (IRQNumber % 32));
	}
}

/*****************************************************************
 * @fn			- USART_IRQPriorityConfig
 *
 * @brief		- This function sets the priority for an USART IRQ number
 *
 * @param[in]	- Macro which pertains to the IRQ number for a USART peripheral
 * @param[in]	- Priority level
 *
 * @return		- none
 *
 * @Note		- none
 */
void USART_IRQPriorityConfig(uint8_t IRQNumber, uint32_t IRQPriority){
	uint8_t iprx = IRQNumber / 4;
	uint8_t iprx_section = IRQNumber % 4;

	uint8_t shift_amount = ( 8 * iprx_section ) + (8 - NO_PRIORITY_BITS_IMPLEMENTED);

	__vo uint32_t* iprReg = (__vo uint32_t*)(NVIC_IPR_BASE_ADDR + (4 * iprx));

	*iprReg |= (IRQPriority << (shift_amount));
}


/*********************************************************************
 * @fn      		  - USART_IRQHandler
 *
 * @brief             -
 *
 * @param[in]         -
 * @param[in]         -
 * @param[in]         -
 *
 * @return            -
 *
 * @Note              - Resolve all the TODOs

 */
void USART_IRQHandling(USART_Handle_t *pUSARTHandle)
{

	uint32_t temp1, temp2, temp3;

/*************************Check for TC flag ********************************************/

    //Implement the code to check the state of TC bit in the SR
	temp1 = pUSARTHandle->pUSARTx->SR & ( 1 << USART_SR_TC);

	 //Implement the code to check the state of TCEIE bit
	temp2 = pUSARTHandle->pUSARTx->CR1 & ( 1 << USART_CR1_TCIE);

	if(temp1 && temp2 )
	{
		//this interrupt is because of TC

		//close transmission and call application callback if TxLen is zero
		if ( pUSARTHandle->TxBusyState == USART_BUSY_IN_TX)
		{
			//Check the TxLen . If it is zero then close the data transmission
			if(pUSARTHandle->TxLen == 0)
			{
				//Implement the code to clear the TC flag
				pUSARTHandle->pUSARTx->SR &= ~( 1 << USART_SR_TC);

				//Implement the code to clear the TCIE control bit
				pUSARTHandle->pUSARTx->CR1 &= ~(1 << USART_CR1_TCIE);

				//Reset the application state
				pUSARTHandle->TxBusyState = USART_READY;

				//Reset Buffer address to NULL
				pUSARTHandle->pTxBuffer = NULL;

				//Reset the length to zero
				pUSARTHandle->TxLen = 0;

				//Call the applicaton call back with event USART_EVENT_TX_CMPLT
				USART_ApplicationEventCallback(pUSARTHandle,USART_EVENT_TX_CMPLT);
			}
		}
	}

/*************************Check for TXE flag ********************************************/

	//Implement the code to check the state of TXE bit in the SR
	temp1 = pUSARTHandle->pUSARTx->SR & ( 1 << USART_SR_TxE);

	//Implement the code to check the state of TXEIE bit in CR1
	temp2 = pUSARTHandle->pUSARTx->CR1 & ( 1 << USART_CR1_TXEIE);


	if(temp1 && temp2 )
	{
		//this interrupt is because of TXE

		if(pUSARTHandle->TxBusyState == USART_BUSY_IN_TX)
		{
			//Keep sending data until Txlen reaches to zero
			if(pUSARTHandle->TxLen > 0)
			{
				//Check the USART_WordLength item for 9BIT or 8BIT in a frame
				if(pUSARTHandle->USART_Config.wordLength == USART_WORDLEN_9BITS)
				{
					//if 9BIT , load the DR with 2bytes masking the bits other than first 9 bits
					uint16_t* pdata = (uint16_t*) pUSARTHandle->pTxBuffer;

					//loading only first 9 bits , so we have to mask with the value 0x01FF
					pUSARTHandle->pUSARTx->DR = (*pdata & (uint16_t)0x01FF);

					//check for USART_ParityControl
					if(pUSARTHandle->USART_Config.parityControl == USART_PARITY_DISABLE)
					{
						//No parity is used in this transfer , so, 9bits of user data will be sent
						//Implement the code to increment pTxBuffer twice
						pUSARTHandle->pTxBuffer++;
						pUSARTHandle->pTxBuffer++;

						//Implement the code to decrement the length
						pUSARTHandle->TxLen -= 2;
					}
					else
					{
						//Parity bit is used in this transfer . so , 8bits of user data will be sent
						//The 9th bit will be replaced by parity bit by the hardware
						pUSARTHandle->pTxBuffer++;

						//Implement the code to decrement the length
						pUSARTHandle->TxLen--;
					}
				}
				else
				{
					//This is 8bit data transfer
					pUSARTHandle->pUSARTx->DR = (*(pUSARTHandle->pTxBuffer)  & (uint8_t)0xFF);

					//Implement the code to increment the buffer address
					pUSARTHandle->pTxBuffer++;

					//Implement the code to decrement the length
					pUSARTHandle->TxLen--;
				}

			}
			if (pUSARTHandle->TxLen == 0 )
			{
				//TxLen is zero
				//Implement the code to clear the TXEIE bit (disable interrupt for TXE flag )
				pUSARTHandle->pUSARTx->CR1 &= ~(1 << USART_CR1_TXEIE);
			}
		}
	}

/*************************Check for RXNE flag ********************************************/

	temp1 = pUSARTHandle->pUSARTx->SR & ( 1 << USART_SR_RxNE);
	temp2 = pUSARTHandle->pUSARTx->CR1 & ( 1 << USART_CR1_RXNEIE);


	if(temp1 && temp2 )
	{
		//this interrupt is because of rxne
		if(pUSARTHandle->RxBusyState == USART_BUSY_IN_RX)
		{
			//RXNE is set so receive data
			if(pUSARTHandle->RxLen > 0)
			{
				//Check the USART_WordLength to decide whether we are going to receive 9bit of data in a frame or 8 bit
				if(pUSARTHandle->USART_Config.wordLength == USART_WORDLEN_9BITS)
				{
					//We are going to receive 9bit data in a frame

					//Now, check are we using USART_ParityControl control or not
					if(pUSARTHandle->USART_Config.parityControl == USART_PARITY_DISABLE)
					{
						//No parity is used. so, all 9bits will be of user data

						//read only first 9 bits so mask the DR with 0x01FF
						*((uint16_t*) pUSARTHandle->pRxBuffer) = (pUSARTHandle->pUSARTx->DR  & (uint16_t)0x01FF);

						//Now increment the pRxBuffer two times
						pUSARTHandle->pRxBuffer++;
						pUSARTHandle->pRxBuffer++;

						//Implement the code to decrement the length
						pUSARTHandle->RxLen -= 2;
					}
					else
					{
						//Parity is used. so, 8bits will be of user data and 1 bit is parity
						 *(pUSARTHandle->pRxBuffer) = (pUSARTHandle->pUSARTx->DR  & (uint8_t)0xFF);

						 //Now increment the pRxBuffer
						 pUSARTHandle->pRxBuffer++;

						 //Implement the code to decrement the length
						 pUSARTHandle->RxLen--;
					}
				}
				else
				{
					//We are going to receive 8bit data in a frame

					//Now, check are we using USART_ParityControl control or not
					if(pUSARTHandle->USART_Config.parityControl == USART_PARITY_DISABLE)
					{
						//No parity is used , so all 8bits will be of user data

						//read 8 bits from DR
						 *(pUSARTHandle->pRxBuffer) = (uint8_t) (pUSARTHandle->pUSARTx->DR  & (uint8_t)0xFF);
					}

					else
					{
						//Parity is used, so , 7 bits will be of user data and 1 bit is parity

						//read only 7 bits , hence mask the DR with 0X7F
						 *(pUSARTHandle->pRxBuffer) = (uint8_t) (pUSARTHandle->pUSARTx->DR  & (uint8_t)0x7F);

					}

					//Now , increment the pRxBuffer
					pUSARTHandle->pRxBuffer++;

					//Implement the code to decrement the length
					pUSARTHandle->RxLen--;
				}


			}//if of >0

			if(pUSARTHandle->RxLen == 0)
			{
				//disable the rxne
				pUSARTHandle->pUSARTx->CR1 &= ~(1 << USART_CR1_RXNEIE);
				pUSARTHandle->RxBusyState = USART_READY;
				USART_ApplicationEventCallback(pUSARTHandle, USART_EVENT_RX_CMPLT);
			}
		}
	}


/*************************Check for CTS flag ********************************************/

	//Note : CTS feature is not applicable for UART4 and UART5
	if(pUSARTHandle->pUSARTx != UART4 && pUSARTHandle->pUSARTx != UART5){

		//Implement the code to check the status of CTS bit in the SR
		temp1 = pUSARTHandle->pUSARTx->SR & (1 << USART_SR_CTS);

		//Implement the code to check the state of CTSE bit in CR1
		temp2 = pUSARTHandle->pUSARTx->CR3 & ( 1 << USART_CR3_CTSE);

		//Implement the code to check the state of CTSIE bit in CR3 (This bit is not available for UART4 & UART5.)
		temp3 = pUSARTHandle->pUSARTx->CR3 & (1 << USART_CR3_CTSIE);

		if(temp1 && temp2)
		{
			//Implement the code to clear the CTS flag in SR
			pUSARTHandle->pUSARTx->SR &= ~(1 << USART_SR_CTS);

			//this interrupt is because of cts
			USART_ApplicationEventCallback(pUSARTHandle, USART_EVENT_CTS);
		}
	}

/*************************Check for IDLE detection flag ********************************************/

	//Implement the code to check the status of IDLE flag bit in the SR
	temp1 = pUSARTHandle->pUSARTx->SR & (1 << USART_SR_IDLE);

	//Implement the code to check the state of IDLEIE bit in CR1
	temp2 = pUSARTHandle->pUSARTx->CR3 & ( 1 << USART_SR_IDLE);


	if(temp1 && temp2)
	{
		//Implement the code to clear the IDLE flag. Refer to the RM to understand the clear sequence
		uint8_t dummyRead = pUSARTHandle->pUSARTx->SR;
		dummyRead = pUSARTHandle->pUSARTx->DR;
		(void)dummyRead;

		//this interrupt is because of idle
		USART_ApplicationEventCallback(pUSARTHandle, USART_EVENT_IDLE);
	}

/*************************Check for Overrun detection flag ********************************************/

	//Implement the code to check the status of ORE flag  in the SR
	temp1 = pUSARTHandle->pUSARTx->SR & USART_SR_ORE;

	//Implement the code to check the status of RXNEIE  bit in the CR1
	temp2 = pUSARTHandle->pUSARTx->CR1 & USART_CR1_RXNEIE;


	if(temp1 && temp2)
	{
		//Need not to clear the ORE flag here, instead give an api for the application to clear the ORE flag .

		//USART_ORE_ApplicationEventCallback();

		//this interrupt is because of Overrun error
		USART_ApplicationEventCallback(pUSARTHandle, USART_EVENT_ORE);
	}


/*************************Check for Error Flag ********************************************/

//Noise Flag, Overrun error and Framing Error in multibuffer communication
//We dont discuss multibuffer communication in this course. please refer to the RM
//The blow code will get executed in only if multibuffer mode is used.

	temp2 =  pUSARTHandle->pUSARTx->CR3 & (1 << USART_CR3_EIE) ;

	if(temp2)
	{
		temp1 = pUSARTHandle->pUSARTx->SR;
		if(temp1 & (1 << USART_SR_FE))
		{
			/*
				This bit is set by hardware when a de-synchronization, excessive noise or a break character
				is detected. It is cleared by a software sequence (an read to the USART_SR register
				followed by a read to the USART_DR register).
			*/
			USART_ApplicationEventCallback(pUSARTHandle, USART_ERREVENT_FE);
		}

		if(temp1 & (1 << USART_SR_NF))
		{
			/*
				This bit is set by hardware when noise is detected on a received frame. It is c	leared by a
				software sequence (an read to the USART_SR register followed by a read to the
				USART_DR register).
			*/
			USART_ApplicationEventCallback(pUSARTHandle, USART_ERREVENT_NE);
		}

		if(temp1 & ( 1 << USART_SR_ORE))
		{
			USART_ApplicationEventCallback(pUSARTHandle, USART_ERREVENT_ORE);
		}
	}

	(void)temp3;
}
