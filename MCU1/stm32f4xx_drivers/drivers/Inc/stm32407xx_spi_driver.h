/*
 * stm32407xx_spi_driver.h
 *
 *  Created on: Sep 10, 2022
 *      Author: linkachu
 */

#ifndef INC_STM32407XX_SPI_DRIVER_H_
#define INC_STM32407XX_SPI_DRIVER_H_

#include "stm32f407xx.h"

typedef struct{
	uint8_t DeviceMode;
	uint8_t BusConfig;
	uint8_t SclkSpeed;
	uint8_t DFF;
	uint8_t CPOL;
	uint8_t CPHA;
	uint8_t SSM;
	uint8_t FrameFormat;
} SPI_Config_t;


/*
 * @DeviceMode
 */
#define SPI_DEVICE_MODE_MASTER	1
#define SPI_DEVICE_MODE_SLAVE	0

/*
 * @BusConfig
 */
#define SPI_BUS_CONFIG_FD				1
#define SPI_BUS_CONFIG_HD				2
#define SPI_BUS_CONFIG_SIMPLEX_RXONLY	3

/*
 * @SclkSpeed
 */
#define SPI_SCLK_SPEED_DIV2		0
#define SPI_SCLK_SPEED_DIV4		1
#define SPI_SCLK_SPEED_DIV8		2
#define SPI_SCLK_SPEED_DIV16	3
#define SPI_SCLK_SPEED_DIV32	4
#define SPI_SCLK_SPEED_DIV64	5
#define SPI_SCLK_SPEED_DIV128	6
#define SPI_SCLK_SPEED_DIV256	7

/*
 * @SclkSpeed
 */
#define SPI_DFF_8BITS 	0
#define SPI_DFF_16BITS 	1

/*
 * @CPOL
 */
#define SPI_CPOL_HIGH	1
#define SPI_CPOL_LOW 	0

/*
 * @CPHA
 */
#define SPI_CPHA_SECOND	1
#define SPI_CPHA_FIRST 	0

/*
 * @SSM
 */
#define SPI_SSM_HW		0
#define SPI_SSM_SW		1

/*
 * @FrameFormat
 */
#define SPI_FRAME_FORMAT_MSBFIRST 	0
#define SPI_FRAME_FORMAT_LSBFIRST 	1

/*
 * Status flag macros
 */
#define SPI_TXE_FLAG 	(1 << SPI_SR_TXE)
#define SPI_RXNE_FLAG	(1 << SPI_SR_RXNE)
#define SPI_CHSIDE_FLAG (1 << SPI_SR_CHSIDE)
#define SPI_UDR_FLAG	(1 << SPI_SR_UDR)
#define SPI_CRCERR_FLAG	(1 << SPI_SR_CRC_ERR)
#define SPI_MODF_FLAG	(1 << SPI_SR_MODF)
#define SPI_OVR_FLAG	(1 << SPI_SR_OVR)
#define SPI_BSY_FLAG	(1 << SPI_SR_BSY)
#define SPI_FRE_FLAG	(1 << SPI_SR_FRE)

// SPI states
#define SPI_READY		0
#define SPI_BUSY_IN_RX	1
#define SPI_BUSY_IN_TX	2

// Possible SPI Application events
#define SPI_EVENT_TX_CMPLT	1
#define SPI_EVENT_RX_CMPLT	2
#define SPI_EVENT_OVR_ERR	3
#define SPI_EVENT_CRC_ERR	4

typedef struct{
	SPI_RegDef_t 	*pSPIx;
	SPI_Config_t	SPIConfig;
	uint8_t			*pTxBuffer;	// To store the app. Tx buffer address
	uint8_t			*pRxBuffer; // To store the app. Rx buffer address
	uint32_t		TxLen;
	uint32_t		RxLen;
	uint8_t			TxState;
	uint8_t			RxState;
} SPI_Handle_t;

// Peripheral clock setup
void SPI_PeriClockControl(SPI_RegDef_t *pSPIx, uint8_t EnorDi);

// Init and de-enit
void SPI_Init(SPI_Handle_t *pSPIHandle);
void SPI_DeInit(SPI_RegDef_t *pSPIx);

// Function to enable and disable SPI
void SPI_PeripheralControl(SPI_RegDef_t* pSPIx, uint8_t EnorDi);
void SPI_SSIControl(SPI_RegDef_t * pSPIx, uint8_t EnorDi);
void SPI_SSOEControl(SPI_RegDef_t * pSPIx, uint8_t EnorDi);

// Data Send and Receive
uint8_t SPI_GetFlagStatus(SPI_RegDef_t *pSPIx, uint32_t flagName);
void SPI_SendData(SPI_RegDef_t *pSPIx, uint8_t* pTxBuffer, uint32_t len);
void SPI_ReceiveData(SPI_RegDef_t *pSPIx, uint8_t* pRxBuffer, uint32_t len);

// Interrupt Data Send and Receive
uint8_t SPI_SendDataIT(SPI_Handle_t *pSPIHandle, uint8_t* pTxBuffer, uint32_t len);
uint8_t SPI_ReceiveDataIT(SPI_Handle_t *pSPIHandle, uint8_t* pRxBuffer, uint32_t len);

// IRQ Configuration and ISR Handling
void SPI_IRQInterruptConfig(uint8_t IRQNumber, uint32_t IRQPriority, uint8_t EnorDi);
void SPI_IRQPriorityConfig(uint8_t IRQNumber, uint32_t IRQPriority);
void SPI_IRQHandling(SPI_Handle_t *pSPIHandle);

void SPI_ClearOVRFlag(SPI_RegDef_t *pSPIHandle);
void SPI_CloseTransmission(SPI_Handle_t *pSPIHandle);
void SPI_CloseReception(SPI_Handle_t *pSPIHandle);

// Application callback
void SPI_ApplicationEventCallback(SPI_Handle_t *pSPIHandle, uint8_t AppEv);

#endif /* INC_STM32407XX_SPI_DRIVER_H_ */
