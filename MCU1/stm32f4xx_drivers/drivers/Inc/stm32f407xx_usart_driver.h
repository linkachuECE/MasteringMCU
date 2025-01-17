/*
 * stm32f407xx_usart_driver.h
 *
 *  Created on: Oct 12, 2022
 *      Author: ebraun
 */

#ifndef INC_STM32F407XX_USART_DRIVER_H_
#define INC_STM32F407XX_USART_DRIVER_H_

#include "stm32f407xx.h"

typedef struct{
	uint8_t mode;
	uint32_t baudRate;
	uint8_t noOfStopBits;
	uint8_t wordLength;
	uint8_t parityControl;
	uint8_t HWFlowControl;
} USART_Config_t;

typedef struct{
	USART_RegDef_t* pUSARTx;
	USART_Config_t USART_Config;
	uint32_t 		TxLen;
	uint32_t 		RxLen;
	uint8_t 		TxBusyState;
	uint8_t			RxBusyState;
	uint8_t 		sr;
	uint8_t*		pTxBuffer;
	uint8_t*		pRxBuffer;
} USART_Handle_t;

/*
 *@mode
 *Possible options for USART_Mode
 */
#define USART_MODE_ONLY_TX 0
#define USART_MODE_ONLY_RX 1
#define USART_MODE_TXRX  2

/*
 *@baudRate
 *Possible options for USART_Baud
 */
#define USART_STD_BAUD_1200					1200
#define USART_STD_BAUD_2400					400
#define USART_STD_BAUD_9600					9600
#define USART_STD_BAUD_19200 				19200
#define USART_STD_BAUD_38400 				38400
#define USART_STD_BAUD_57600 				57600
#define USART_STD_BAUD_115200 				115200
#define USART_STD_BAUD_230400 				230400
#define USART_STD_BAUD_460800 				460800
#define USART_STD_BAUD_921600 				921600
#define USART_STD_BAUD_2M 					2000000
#define SUART_STD_BAUD_3M 					3000000

/*
 *@parityControl
 *Possible options for USART_ParityControl
 */
#define USART_PARITY_EN_ODD   2
#define USART_PARITY_EN_EVEN  1
#define USART_PARITY_DISABLE   0

/*
 *@wordLength
 *Possible options for USART_WordLength
 */
#define USART_WORDLEN_8BITS  0
#define USART_WORDLEN_9BITS  1

/*
 *@noOfStopBits
 *Possible options for USART_NoOfStopBits
 */
#define USART_STOPBITS_1     0
#define USART_STOPBITS_0_5   1
#define USART_STOPBITS_2     2
#define USART_STOPBITS_1_5   3

/*
 *@HWFlowControl
 *Possible options for USART_HWFlowControl
 */
#define USART_HW_FLOW_CTRL_NONE    	0
#define USART_HW_FLOW_CTRL_CTS    	1
#define USART_HW_FLOW_CTRL_RTS    	2
#define USART_HW_FLOW_CTRL_CTS_RTS	3

// USART Busy States
#define USART_READY					0
#define USART_BUSY_IN_TX			1
#define USART_BUSY_IN_RX			2

// Status flag macros
#define USART_PE_FLAG		(1 << USART_SR_PE)
#define USART_FE_FLAG		(1 << USART_SR_FE)
#define USART_NF_FLAG		(1 << USART_SR_NF)
#define USART_ORE_FLAG		(1 << USART_SR_ORE)
#define USART_IDLE_FLAG		(1 << USART_SR_IDLE)
#define USART_RxNE_FLAG		(1 << USART_SR_RxNE)
#define USART_TC_FLAG		(1 << USART_SR_TC)
#define USART_TxE_FLAG		(1 << USART_SR_TxE)
#define USART_LBD_FLAG		(1 << USART_SR_LBD)
#define USART_CTS_FLAG		(1 << USART_SR_CTS)

// Event macros
#define USART_EVENT_TX_CMPLT 	0
#define USART_EVENT_RX_CMPLT 	1
#define USART_EVENT_CTS			2
#define USART_EVENT_IDLE		3
#define USART_EVENT_ORE			4
#define USART_ERREVENT_FE		5
#define USART_ERREVENT_NE		6
#define USART_ERREVENT_ORE		7
/*
 * Peripheral Clock setup
 */
void USART_PeriClockControl(USART_RegDef_t *pUSARTx, uint8_t EnOrDi);

/*
 * Init and De-init
 */
void USART_Init(USART_Handle_t *pUSARTHandle);
void USART_DeInit(USART_RegDef_t *pUSARTx);

/*
 * Data Send and Receive
 */

void USART_SendData(USART_Handle_t *pUSARTHandle, uint8_t *pTxBuffer, uint32_t len);
void USART_ReceiveData(USART_Handle_t *pUSARTHandle, uint8_t *pRxBuffer, uint32_t len);
uint8_t USART_SendDataIT(USART_Handle_t *pUSARTHandle,uint8_t *pTxBuffer, uint32_t len);
uint8_t USART_ReceiveDataIT(USART_Handle_t *pUSARTHandle, uint8_t *pRxBuffer, uint32_t len);
void USART_SetBaudRate(USART_RegDef_t *pUSARTx, uint32_t BaudRate);
/*
 * IRQ Configuration and ISR handling
 */
void USART_IRQInterruptConfig(uint8_t IRQNumber, uint8_t EnOrDi);
void USART_IRQPriorityConfig(uint8_t IRQNumber, uint32_t IRQPriority);
void USART_IRQHandling(USART_Handle_t *pHandle);

/*
 * Other Peripheral Control APIs
 */
void USART_PeripheralControl(USART_RegDef_t *pUSARTx, uint8_t EnOrDi);
uint8_t USART_GetFlagStatus(USART_RegDef_t* pUSARTx, uint8_t StatusFlagName);
void USART_ClearFlag(USART_RegDef_t *pUSARTx, uint16_t StatusFlagName);

/*
 * Application callback
 */
void USART_ApplicationEventCallback(USART_Handle_t *pUSARTHandle,uint8_t AppEv);

#endif /* INC_STM32F407XX_USART_DRIVER_H_ */
