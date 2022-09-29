/*
 * stm32f407xx_i2c_driver.h
 *
 *  Created on: Sep 21, 2022
 *      Author: linkachu
 */

#ifndef INC_STM32F407XX_I2C_DRIVER_H_
#define INC_STM32F407XX_I2C_DRIVER_H_

#include "stm32f407xx.h"

typedef struct{
	uint32_t SCLSpeed;
	uint8_t DeviceAddress;
	uint8_t AckControl;
	uint8_t FMDutyCycle;
} I2C_Config_t;

/*
 * @SCLSpeed
 */
#define I2C_SCL_SPEED_SM	100000
#define I2C_SCL_SPEED_FM4K	400000
#define I2C_SCL_SPEED_FM2K	200000

/*
 * @AckControl
 */
#define I2C_ACK_ENABLE		1
#define I2C_ACK_DISABLE		0

/*
 * @FMDutyCycle
 */
#define I2C_FM_DUTY_2		0
#define I2C_FM_DUTY_16_9	1

typedef struct{
	I2C_RegDef_t *pI2Cx;
	I2C_Config_t I2C_Config;
} I2C_Handle_t;

// Status flag macros
#define I2C_SB_FLAG 			(1 << I2C_SR1_SB)
#define I2C_ADDR_FLAG 			(1 << I2C_SR1_ADDR)
#define I2C_BTF_FLAG 			(1 << I2C_SR1_BTF)
#define I2C_ADD10_FLAG 			(1 << I2C_SR1_ADD10)
#define I2C_STOPF_FLAG 			(1 << I2C_SR1_STOPF)
#define I2C_RxNE_FLAG 			(1 << I2C_SR1_RxNE)
#define I2C_TxE_FLAG 			(1 << I2C_SR1_TxE)
#define I2C_BERR_FLAG 			(1 << I2C_SR1_BERR)
#define I2C_ARLO_FLAG 			(1 << I2C_SR1_ARLO)
#define I2C_AF_FLAG 			(1 << I2C_SR1_AF)
#define I2C_OVR_FLAG 			(1 << I2C_SR1_OVR)
#define I2C_PEC_ERR_FLAG 		(1 << I2C_SR1_PEC_ERR)
#define I2C_TIME_OUT_FLAG 		(1 << I2C_SR1_TIME_OUT)
#define I2C_SMB_ALERT_FLAG 		(1 << I2C_SR1_SMB_ALERT)

#define I2C_MSL_FLAG 			(1 << I2C_SR2_MSL)
#define I2C_BUSY_FLAG 			(1 << I2C_SR2_BUSY)
#define I2C_TRA_FLAG 			(1 << I2C_SR2_TRA)
#define I2C_GEN_CALL_FLAG 		(1 << I2C_SR2_GEN_CALL)
#define I2C_SMBDE_FAULT_FLAG 	(1 << I2C_SR2_SMBDE_FAULT)
#define I2C_SMB_HOST_FLAG 		(1 << I2C_SR2_SMB_HOST)
#define I2C_DUALF_FLAG 			(1 << I2C_SR2_DUALF)
#define I2C_PEC_FLAG 			(1 << I2C_SR2_PEC)

// Peripheral clock setup
void I2C_PeriClockControl(I2C_RegDef_t *pI2Cx, uint8_t EnorDi);
uint32_t RCC_GetPCLK1Value(void);

// Init and de-enit
void I2C_Init(I2C_Handle_t *pI2CHandle);
void I2C_DeInit(I2C_RegDef_t *pI2Cx);

// Function to enable and disable I2C
void I2C_PeripheralControl(I2C_RegDef_t* pI2Cx, uint8_t EnorDi);

// IRQ Configuration and ISR Handling
void I2C_IRQInterruptConfig(uint8_t IRQNumber, uint32_t IRQPriority, uint8_t EnorDi);
void I2C_IRQPriorityConfig(uint8_t IRQNumber, uint32_t IRQPriority);
void I2C_IRQHandling(I2C_Handle_t *pI2CHandle);

// Application callback
void I2C_ApplicationEventCallback(I2C_Handle_t *pI2CHandle, uint8_t AppEv);

// Data send and receive
uint8_t I2C_GetSR1FlagStatus(I2C_RegDef_t *pI2Cx, uint32_t flagName);
void I2C_MasterSendData(I2C_Handle_t *pI2CHandle, uint8_t *pTxbuffer, uint32_t len, uint8_t slaveAddr);
void I2C_MasterReceiveData(I2C_Handle_t *pI2CHandle, uint8_t *pRxbuffer, uint32_t len, uint8_t slaveAddr);
#endif /* INC_STM32F407XX_I2C_DRIVER_H_ */
