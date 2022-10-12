/*
 * stm32f407xx_usart_driver.c
 *
 *  Created on: Oct 12, 2022
 *      Author: ebraun
 */

#include "stm32f407xx_usart_driver.h"
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
