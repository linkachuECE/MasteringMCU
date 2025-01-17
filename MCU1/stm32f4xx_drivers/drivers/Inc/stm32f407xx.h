/*
 * stm32f407xx.h
 *
 *  Created on: Aug 31, 2022
 *      Author: egbra
 */

#ifndef INC_STM32F407XX_H_
#define INC_STM32F407XX_H_

#include <stdint.h>
#include <stddef.h>

#define __vo volatile
#define __weak __attribute__((weak))

//***************** PROCESSOR SPECIFIC DETAILS **********************//
// ARM Cortex Mx Processor NVIC ISERx register Addresses

#define NVIC_ISER0 					( (__vo uint32_t*) 0xE000E100UL )
#define NVIC_ISER1 					( (__vo uint32_t*) 0xE000E104UL )
#define NVIC_ISER2 					( (__vo uint32_t*) 0xE000E108UL )
#define NVIC_ISER3 					( (__vo uint32_t*) 0xE000E10CUL )

// ARM Cortex Mx Processor NVIC ICERx register Addresses

#define NVIC_ICER0 					( (__vo uint32_t*) 0xE000E180UL )
#define NVIC_ICER1 					( (__vo uint32_t*) 0xE000E184UL )
#define NVIC_ICER2 					( (__vo uint32_t*) 0xE000E188UL )
#define NVIC_ICER3 					( (__vo uint32_t*) 0xE000E18CUL )

// ARM Cortex Mx Processor NVIC IPR register Addresses
#define NVIC_IPR_BASE_ADDR			0xE000E400UL

// ARM Cortex Mx Processor number of priority bits implemented in Priority Register
#define NO_PRIORITY_BITS_IMPLEMENTED 	4

// **************** BASE ADDRESSES ****************** //

// MEMORY BASE ADDRESSES
#define FLASH_BASEADDR				0x08000000UL
#define SRAM1_BASEADDR				0x20000000UL
#define SRAM2_BASEADDR				0x2001C000UL
#define ROM_BASEADDR				0x1FFF0000UL
#define SRAM						SRAM1_BASEADDR

//PERIPHERAL BASE ADDRESSES
#define PERIPH_BASEADDR				0x40000000UL
#define APB1PERIPH_BASEADDR			PERIPH_BASEADDR
#define APB2PERIPH_BASEADDR			0x40010000UL
#define AHB1PERIPH_BASEADDR			0x40020000UL
#define AHB2PERIPH_BASEADDR			0x50000000UL

// AHB1 PERIPHERAL ADDRESSES
#define GPIOA_BASEADDR				(AHB1PERIPH_BASEADDR + 0x0000UL)
#define GPIOB_BASEADDR				(AHB1PERIPH_BASEADDR + 0x0400UL)
#define GPIOC_BASEADDR				(AHB1PERIPH_BASEADDR + 0x0800UL)
#define GPIOD_BASEADDR				(AHB1PERIPH_BASEADDR + 0x0C00UL)
#define GPIOE_BASEADDR				(AHB1PERIPH_BASEADDR + 0x1000UL)
#define GPIOF_BASEADDR				(AHB1PERIPH_BASEADDR + 0x1400UL)
#define GPIOG_BASEADDR				(AHB1PERIPH_BASEADDR + 0x1800UL)
#define GPIOH_BASEADDR				(AHB1PERIPH_BASEADDR + 0x1C00UL)
#define GPIOI_BASEADDR				(AHB1PERIPH_BASEADDR + 0x2000UL)

#define RCC_BASEADDR				(AHB1PERIPH_BASEADDR + 0x3800UL)

// APB1 PERIPHERAL ADDRESSES
#define I2C1_BASEADDR				(APB1PERIPH_BASEADDR + 0x5400UL)
#define I2C2_BASEADDR				(APB1PERIPH_BASEADDR + 0x5800UL)
#define I2C3_BASEADDR				(APB1PERIPH_BASEADDR + 0x5C00UL)

#define SPI2_BASEADDR				(APB1PERIPH_BASEADDR + 0x3800UL)
#define SPI3_BASEADDR				(APB1PERIPH_BASEADDR + 0x3C00UL)

#define USART2_BASEADDR				(APB1PERIPH_BASEADDR + 0x4400UL)
#define USART3_BASEADDR				(APB1PERIPH_BASEADDR + 0x4800UL)
#define UART4_BASEADDR				(APB1PERIPH_BASEADDR + 0x4C00UL)
#define UART5_BASEADDR				(APB1PERIPH_BASEADDR + 0x5000UL)

// APB2 PERIPHERAL ADDRESSES
#define SPI1_BASEADDR				(APB2PERIPH_BASEADDR + 0x3000UL)

#define USART1_BASEADDR				(APB2PERIPH_BASEADDR + 0x1000UL)
#define USART6_BASEADDR				(APB2PERIPH_BASEADDR + 0x1400UL)

#define EXTI_BASEADDR				(APB2PERIPH_BASEADDR + 0x3C00UL)

#define SYSCFG_BASEADDR				(APB2PERIPH_BASEADDR + 0x3800UL)


// **************** PERIHERAL REGISTER DEFINITION STRUCTURES ****************** //

// GPIO
typedef struct {
	__vo uint32_t MODER;			// Mode register
	__vo uint32_t OTYPER;			// Output type register
	__vo uint32_t OSPEEDR;			// Output speed register
	__vo uint32_t PUPDR;			// Pull-up/pull-down register
	__vo uint32_t IDR;				// Input data register
	__vo uint32_t ODR;				// Output data register
	__vo uint32_t BSRR;				// Bit set/reset register
	__vo uint32_t LCKR;				// Configuration lock register
	__vo uint32_t AFR[2];			// AFR[0] : GPIO alternate function low register, AFR[1] : GPIO alternate function high register
} GPIO_RegDef_t;

// I2C
typedef struct {
	__vo uint32_t CR1;				// Control register 1
	__vo uint32_t CR2;				// Control register 2
	__vo uint32_t OAR1;				// Own address register
	__vo uint32_t OAR2;				// Own address register 2
	__vo uint32_t DR;				// Data register
	__vo uint32_t SR1;				// Status register 1
	__vo uint32_t SR2;				// Status register 2
	__vo uint32_t CCR;				// Clock control register
	__vo uint32_t TRISE;			// TRISE register
	__vo uint32_t FLTR;				// FLTR register
} I2C_RegDef_t;


// SPI
typedef struct {
	__vo uint32_t CR1;				// Control register 1
	__vo uint32_t CR2;				// Control register 2
	__vo uint32_t SR;				// Status register
	__vo uint32_t DR;				// Data register
	__vo uint32_t CRCPR;			// CRC polynomial register
	__vo uint32_t RXCRCR;			// RX CRC register
	__vo uint32_t TXCRCR;			// TX CRC register
	__vo uint32_t I2SCFGR;			// I2S configuration register
	__vo uint32_t I2SPR;			// I2S pre-scaler register
} SPI_RegDef_t;

// USART
typedef struct {
	__vo uint32_t SR;				// Status register
	__vo uint32_t DR;				// Data register
	__vo uint32_t BRR;				// Baud rate register
	__vo uint32_t CR1;				// Control register 1
	__vo uint32_t CR2;				// Control register 2
	__vo uint32_t CR3;				// Control register 3
	__vo uint32_t GTPR;				// Guard time and prescaler register
} USART_RegDef_t;

// RCC
typedef struct {
// 	REGISTER							DESCRIPTION														OFFSET
	__vo uint32_t CR;					// Clock control register										0x00
	__vo uint32_t PLLCFGR;				// PLL configuration register									0x04
	__vo uint32_t CFGR;					// Clock configuration register									0x08
	__vo uint32_t CIR;					// Clock interrupt register										0x0C
	__vo uint32_t AHB1RSTR;				// AHB1 peripheral reset register								0x10
	__vo uint32_t AHB2RSTR;				// AHB2 peripheral reset register								0x14
	__vo uint32_t AHB3RSTR;				// AHB3 peripheral reset register								0x18
		 uint32_t RESERVED0;			// RESERVED														0x1C
	__vo uint32_t APB1RSTR;				// APB1 peripheral reset register								0x20
	__vo uint32_t APB2RSTR;				// APB2 peripheral reset register								0x24
	 	 uint32_t RESERVED1;			// RESERVED														0x28
	 	 uint32_t RESERVED2;			// RESERVED														0x2C
	__vo uint32_t AHB1ENR;				// AHB1 peripheral clock register								0x30
	__vo uint32_t AHB2ENR;				// AHB2 peripheral clock enable register						0x34
	__vo uint32_t AHB3ENR;				// AHB3 peripheral clock enable register						0x38
	 	 uint32_t RESERVED3;			// RESERVED														0x3C
	__vo uint32_t APB1ENR;				// APB1 peripheral clock enable register						0x40
	__vo uint32_t APB2ENR;				// APB2 peripheral clock enable register						0x44
	 	 uint32_t RESERVED4;			// RESERVED														0x48
	 	 uint32_t RESERVED5;			// RESERVED														0x4C
	__vo uint32_t AHB1LPENR;			// AHB1 peripheral clock enable in low power mode register		0x50
	__vo uint32_t AHB2LPENR;			// AHB2 peripheral clock enable in low power mode register		0x54
	__vo uint32_t AHB3LPENR;			// AHB3 peripheral clock enable in low power mode register		0x58
	 	 uint32_t RESERVED6;			// RESERVED														0x5C
	__vo uint32_t APB1LPENR;			// APB1 peripheral clock enable in low power mode register		0x60
	__vo uint32_t APB2LPENR;			// APB2 peripheral clock enabled in low power mode register		0x64
	 	 uint32_t RESERVED7;			// RESERVED														0x68
	 	 uint32_t RESERVED8;			// RESERVED														0x6C
	__vo uint32_t BDCR;					// Backup domain control register								0x70
	__vo uint32_t CSR;					// Clock control & status register								0x74
	 	 uint32_t RESERVED9;			// RESERVED														0x78
	 	 uint32_t RESERVED10;			// RESERVED														0x7C
	__vo uint32_t SSCGR;				// Spread spectrum clock generation register					0x80
	__vo uint32_t PLLI2SCFGR;			// PLLI2S configuration register								0x84
	__vo uint32_t PLLSAICFGR;			// PLL configuration register									0x88
	__vo uint32_t DCKCFGR;				// Dedicated Clock Configuration Register						0x8C
} RCC_RegDef_t;


// EXTI Registers
typedef struct {
	__vo uint32_t IMR;					// Interrupt mask register										0x00
	__vo uint32_t EMR;					// Event mask register											0x04
	__vo uint32_t RTSR;					// Rising trigger selection register							0x08
	__vo uint32_t FTSR;					// Falling trigger selection register							0x0C
	__vo uint32_t SWIER;				// Software interrupt event register							0x10
	__vo uint32_t PR;					// Pending register												0x14
} EXTI_RegDef_t;

// SYSCFG Registers
typedef struct {
	__vo uint32_t MEMRMP;				// Memory remap register										0x00
	__vo uint32_t PMC;					// Peripheral mode configuration register						0x04
	__vo uint32_t EXTICR[4];			// External interrupt configuration registers 1-4				0x08-0x14
		 uint32_t RESERVED[2];
	__vo uint32_t CMPCR;				// Compensation cell control register							0x20
} SYSCFG_RegDef_t;

//************ PERIPHERAL DEFINITIONS ***************//

#define GPIOA		( (GPIO_RegDef_t*) GPIOA_BASEADDR )
#define GPIOB		( (GPIO_RegDef_t*) GPIOB_BASEADDR )
#define GPIOC		( (GPIO_RegDef_t*) GPIOC_BASEADDR )
#define GPIOD		( (GPIO_RegDef_t*) GPIOD_BASEADDR )
#define GPIOE		( (GPIO_RegDef_t*) GPIOE_BASEADDR )
#define GPIOF		( (GPIO_RegDef_t*) GPIOF_BASEADDR )
#define GPIOG		( (GPIO_RegDef_t*) GPIOG_BASEADDR )
#define GPIOH		( (GPIO_RegDef_t*) GPIOH_BASEADDR )
#define GPIOI		( (GPIO_RegDef_t*) GPIOI_BASEADDR )

#define RCC			( (RCC_RegDef_t*) RCC_BASEADDR )

#define SPI1		( (SPI_RegDef_t*) SPI1_BASEADDR )
#define SPI2		( (SPI_RegDef_t*) SPI2_BASEADDR )
#define SPI3		( (SPI_RegDef_t*) SPI3_BASEADDR )

#define I2C1		( (I2C_RegDef_t*) I2C1_BASEADDR )
#define I2C2		( (I2C_RegDef_t*) I2C2_BASEADDR )
#define I2C3		( (I2C_RegDef_t*) I2C3_BASEADDR )

#define USART1		( (USART_RegDef_t*) USART1_BASEADDR )
#define USART2		( (USART_RegDef_t*) USART2_BASEADDR )
#define USART3		( (USART_RegDef_t*) USART3_BASEADDR )
#define UART4		( (USART_RegDef_t*) UART4_BASEADDR )
#define UART5		( (USART_RegDef_t*) UART5_BASEADDR )
#define USART6		( (USART_RegDef_t*) USART6_BASEADDR )

//************* INTERRUPT DEFINITION ****************//

#define EXTI		( (EXTI_RegDef_t*) EXTI_BASEADDR )

//************* SYSTEM CONFIGURATION DEFINITION ****************//

#define SYSCFG		( (SYSCFG_RegDef_t*) SYSCFG_BASEADDR)

//************* SYSTEM CONFIGURATION DEFINITION ****************//

#define IPRReg		( (NVIC_IPR_RegDef_t*) NVIC_IPR_BASE_ADDR)

//************ CLOCK ENABLE MACROS *****************//

// GPIO ENABLE
#define GPIOA_PCLK_EN()		( RCC->AHB1ENR |= (1 << 0) )
#define GPIOB_PCLK_EN() 	( RCC->AHB1ENR |= (1 << 1) )
#define GPIOC_PCLK_EN() 	( RCC->AHB1ENR |= (1 << 2) )
#define GPIOD_PCLK_EN() 	( RCC->AHB1ENR |= (1 << 3) )
#define GPIOE_PCLK_EN() 	( RCC->AHB1ENR |= (1 << 4) )
#define GPIOF_PCLK_EN() 	( RCC->AHB1ENR |= (1 << 5) )
#define GPIOG_PCLK_EN() 	( RCC->AHB1ENR |= (1 << 6) )
#define GPIOH_PCLK_EN() 	( RCC->AHB1ENR |= (1 << 7) )
#define GPIOI_PCLK_EN() 	( RCC->AHB1ENR |= (1 << 8) )

// I2C ENABLE
#define I2C1_PCLK_EN()		( RCC->APB1ENR |= (1 << 21) )
#define I2C2_PCLK_EN()		( RCC->APB1ENR |= (1 << 22) )
#define I2C3_PCLK_EN()		( RCC->APB1ENR |= (1 << 23) )

// SPI ENABLE
#define SPI1_PCLK_EN()		( RCC->APB2ENR |= (1 << 12) )
#define SPI2_PCLK_EN()		( RCC->APB1ENR |= (1 << 14) )
#define SPI3_PCLK_EN()		( RCC->APB1ENR |= (1 << 15) )

// UART/USART ENABLE
#define USART1_PCLK_EN()	( RCC->APB2ENR |= (1 << 4) )
#define USART2_PCLK_EN()	( RCC->APB1ENR |= (1 << 17) )
#define USART3_PCLK_EN()	( RCC->APB1ENR |= (1 << 18) )
#define UART4_PCLK_EN()		( RCC->APB1ENR |= (1 << 19) )
#define UART5_PCLK_EN()		( RCC->APB1ENR |= (1 << 20) )
#define USART6_PCLK_EN()	( RCC->APB2ENR |= (1 << 5) )

// SYSCFG ENABLE
#define SYSCFG_PCLK_EN()	( RCC->APB2ENR |= (1 << 14) )

//************ CLOCK DISABLE MACROS *****************//

// GPIO DISABLE
#define GPIOA_PCLK_DI()		( RCC->AHB1ENR &= ~(1 << 0) )
#define GPIOB_PCLK_DI() 	( RCC->AHB1ENR &= ~(1 << 1) )
#define GPIOC_PCLK_DI() 	( RCC->AHB1ENR &= ~(1 << 2) )
#define GPIOD_PCLK_DI() 	( RCC->AHB1ENR &= ~(1 << 3) )
#define GPIOE_PCLK_DI() 	( RCC->AHB1ENR &= ~(1 << 4) )
#define GPIOF_PCLK_DI() 	( RCC->AHB1ENR &= ~(1 << 5) )
#define GPIOG_PCLK_DI() 	( RCC->AHB1ENR &= ~(1 << 6) )
#define GPIOH_PCLK_DI() 	( RCC->AHB1ENR &= ~(1 << 7) )
#define GPIOI_PCLK_DI() 	( RCC->AHB1ENR &= ~(1 << 8) )

// I2C DISABLE
#define I2C1_PCLK_DI()		( RCC->APB1ENR &= ~(1 << 21) )
#define I2C2_PCLK_DI()		( RCC->APB1ENR &= ~(1 << 22) )
#define I2C3_PCLK_DI()		( RCC->APB1ENR &= ~(1 << 23) )

// SPI DISABLE
#define SPI1_PCLK_DI()		( RCC->APB2ENR &= ~(1 << 12) )
#define SPI2_PCLK_DI()		( RCC->APB1ENR &= ~(1 << 14) )
#define SPI3_PCLK_DI()		( RCC->APB1ENR &= ~(1 << 15) )

// UART/USART DISABLE
#define USART1_PCLK_DI()	( RCC->APB2ENR &= ~(1 << 4) )
#define USART2_PCLK_DI()	( RCC->APB1ENR &= ~(1 << 17) )
#define USART3_PCLK_DI()	( RCC->APB1ENR &= ~(1 << 18) )
#define UART4_PCLK_DI()		( RCC->APB1ENR &= ~(1 << 19) )
#define UART5_PCLK_DI()		( RCC->APB1ENR &= ~(1 << 20) )
#define USART6_PCLK_DI()	( RCC->APB2ENR &= ~(1 << 5) )

// SYSCFG DISABLE
#define SYSCFG_PCLK_DI()	( RCC->APB2ENR &= ~(1 << 14) )

//************ MACROS TO RESET GPIOX PERIPHERALS *****************//
#define GPIOA_REG_RESET()		do{ (RCC->AHB1RSTR |= (1 << 0)); (RCC->AHB1RSTR &= ~(1 << 0)); } while(0)
#define GPIOB_REG_RESET()		do{ (RCC->AHB1RSTR |= (1 << 1)); (RCC->AHB1RSTR &= ~(1 << 1)); } while(0)
#define GPIOC_REG_RESET()		do{ (RCC->AHB1RSTR |= (1 << 2)); (RCC->AHB1RSTR &= ~(1 << 2)); } while(0)
#define GPIOD_REG_RESET()		do{ (RCC->AHB1RSTR |= (1 << 3)); (RCC->AHB1RSTR &= ~(1 << 3)); } while(0)
#define GPIOE_REG_RESET()		do{ (RCC->AHB1RSTR |= (1 << 4)); (RCC->AHB1RSTR &= ~(1 << 4)); } while(0)
#define GPIOF_REG_RESET()		do{ (RCC->AHB1RSTR |= (1 << 5)); (RCC->AHB1RSTR &= ~(1 << 5)); } while(0)
#define GPIOG_REG_RESET()		do{ (RCC->AHB1RSTR |= (1 << 6)); (RCC->AHB1RSTR &= ~(1 << 6)); } while(0)
#define GPIOH_REG_RESET()		do{ (RCC->AHB1RSTR |= (1 << 7)); (RCC->AHB1RSTR &= ~(1 << 7)); } while(0)
#define GPIOI_REG_RESET()		do{ (RCC->AHB1RSTR |= (1 << 8)); (RCC->AHB1RSTR &= ~(1 << 8)); } while(0)

//************ MACROS TO RESET SPIX PERIPHERALS *****************//
#define SPI1_REG_RESET()		do{ (RCC->APB2RSTR |= (1 << 12)); (RCC->APB2RSTR &= ~(1 << 12)); } while(0)
#define SPI2_REG_RESET()		do{ (RCC->APB1RSTR |= (1 << 14)); (RCC->APB1RSTR &= ~(1 << 14)); } while(0)
#define SPI3_REG_RESET()		do{ (RCC->APB1RSTR |= (1 << 15)); (RCC->AHB1RSTR &= ~(1 << 15)); } while(0)

//************ MACROS TO RESET I2CX PERIPHERALS *****************//
#define I2C1_REG_RESET()		do{ (RCC->APB1RSTR |= (1 << 21)); (RCC->APB1RSTR &= ~(1 << 21)); } while(0)
#define I2C2_REG_RESET()		do{ (RCC->APB1RSTR |= (1 << 22)); (RCC->APB1RSTR &= ~(1 << 22)); } while(0)
#define I2C3_REG_RESET()		do{ (RCC->APB1RSTR |= (1 << 23)); (RCC->AHB1RSTR &= ~(1 << 23)); } while(0)

//***************** GET PORT CODE ********************//

#define GPIO_BASEADDR_TO_CODE(x)   ((x == GPIOA) ? 0 :\
									(x == GPIOB) ? 1 :\
									(x == GPIOC) ? 2 :\
									(x == GPIOD) ? 3 :\
									(x == GPIOE) ? 4 :\
									(x == GPIOF) ? 5 :\
									(x == GPIOG) ? 6 :\
									(x == GPIOH) ? 7 :\
									(x == GPIOI) ? 8:0)

// GPIO Interrupt Numbers
#define IRQ_NO_EXTI0		6
#define IRQ_NO_EXTI1		7
#define IRQ_NO_EXTI2		8
#define IRQ_NO_EXTI3		9
#define IRQ_NO_EXTI4		10
#define IRQ_NO_EXTI9_5		23
#define IRQ_NO_EXTI15_10	40

// SPI Interrupt Number
#define IRQ_NO_SPI1			35
#define IRQ_NO_SPI2			36
#define IRQ_NO_SPI3			51

// I2C Interrupt Number
#define IRQ_NO_I2C1_EV		31
#define IRQ_NO_I2C1_ER		32
#define IRQ_NO_I2C2_EV		33
#define IRQ_NO_I2C2_ER		34
#define IRQ_NO_I2C3_EV		72
#define IRQ_NO_I2C3_ER		73

// USART Interrupt Number
#define IRQ_NO_USART1		37
#define IRQ_NO_USART2		38
#define IRQ_NO_USART3		39
#define IRQ_NO_UART4		52
#define IRQ_NO_UART5		53
#define IRQ_NO_USART6		71

// GENERIC MACROS
#define ENABLE 			1
#define DISABLE 		0
#define SET 			ENABLE
#define RESET 			DISABLE
#define GPIO_SET 		SET
#define GPIO_RESET 		RESET
#define GPIO_PIN_SET 	SET
#define GPIO_PIN_RESET 	RESET
#define FLAG_RESET 		RESET
#define FLAG_SET		SET
#define READ			SET
#define WRITE			RESET

// Bit position definitions of SPI Peripheral
#define SPI_CR1_CPHA			0
#define SPI_CR1_CPOL			1
#define SPI_CR1_MSTR			2
#define SPI_CR1_BR				3
#define SPI_CR1_SPE				6
#define SPI_CR1_LSB_FIRST		7
#define SPI_CR1_SSI				8
#define SPI_CR1_SSM				9
#define SPI_CR1_RX_ONLY			10
#define SPI_CR1_DFF				11
#define SPI_CR1_CRC_NEXT		12
#define SPI_CR1_CRC_EN			13
#define SPI_CR1_BIDI_OE			14
#define SPI_CR1_BIDI_MODE		15

#define SPI_CR2_RXDMAEN			0
#define SPI_CR2_TXDMAEN			1
#define SPI_CR2_SSOE			2
#define SPI_CR2_FRF				4
#define SPI_CR2_ERRIE			5
#define SPI_CR2_RXNEIE			6
#define SPI_CR2_TXEIE			7

#define SPI_SR_RXNE				0
#define SPI_SR_TXE				1
#define SPI_SR_CHSIDE			2
#define SPI_SR_UDR				3
#define SPI_SR_CRC_ERR			4
#define SPI_SR_MODF				5
#define SPI_SR_OVR				6
#define SPI_SR_BSY				7
#define SPI_SR_FRE				8

// Bit position definitions of I2C Peripheral
#define I2C_CR1_PE				0
#define I2C_CR1_SMBUS			1
#define I2C_CR1_SMB_TYPE		3
#define I2C_CR1_ENARP			4
#define I2C_CR1_ENPEC			5
#define I2C_CR1_ENGC			6
#define I2C_CR1_NO_STRETCH		7
#define I2C_CR1_START			8
#define I2C_CR1_STOP			9
#define I2C_CR1_ACK				10
#define I2C_CR1_POS				11
#define I2C_CR1_PEC				12
#define I2C_CR1_ALERT			13
#define I2C_CR1_SWRST			15

#define I2C_CR2_FREQ			0
#define I2C_CR2_ITERREN			8
#define I2C_CR2_ITEVTEN			9
#define I2C_CR2_ITBUFEN			10
#define I2C_CR2_DMAEN			11
#define I2C_CR2_LAST			12

#define I2C_OAR1_ADD10			0
#define I2C_OAR1_ADD7			1
#define I2C_OAR1_ADD_MODE		15

#define I2C_OAR2_ENDUAL			0
#define I2C_OAR2_ADD2			1

#define I2C_SR1_SB				0
#define I2C_SR1_ADDR			1
#define I2C_SR1_BTF				2
#define I2C_SR1_ADD10			3
#define I2C_SR1_STOPF			4
#define I2C_SR1_RxNE			6
#define I2C_SR1_TxE				7
#define I2C_SR1_BERR			8
#define I2C_SR1_ARLO			9
#define I2C_SR1_AF				10
#define I2C_SR1_OVR				11
#define I2C_SR1_PEC_ERR			12
#define I2C_SR1_TIME_OUT		14
#define I2C_SR1_SMB_ALERT		15

#define I2C_SR2_MSL				0
#define I2C_SR2_BUSY			1
#define I2C_SR2_TRA				2
#define I2C_SR2_GEN_CALL		3
#define I2C_SR2_SMBDE_FAULT		5
#define I2C_SR2_SMB_HOST		6
#define I2C_SR2_DUALF			7
#define I2C_SR2_PEC				8

#define I2C_CCR_CCR				0
#define I2C_CCR_DUTY			14
#define I2C_CCR_FS				15

// Bit position definitions of USART Peripheral
#define USART_SR_PE				0
#define USART_SR_FE				1
#define USART_SR_NF				2
#define USART_SR_ORE			3
#define USART_SR_IDLE			4
#define USART_SR_RxNE			5
#define USART_SR_TC				6
#define USART_SR_TxE			7
#define USART_SR_LBD			8
#define USART_SR_CTS			9

#define USART_BRR_DIV_FRACTION	0
#define USART_BRR_DIV_MANTISSA	4

#define USART_CR1_SBK			0
#define USART_CR1_RWU			1
#define USART_CR1_RE			2
#define USART_CR1_TE			3
#define USART_CR1_IDLEIE		4
#define USART_CR1_RXNEIE		5
#define USART_CR1_TCIE			6
#define USART_CR1_TXEIE			7
#define USART_CR1_PEIE			8
#define USART_CR1_PS			9
#define USART_CR1_PCE			10
#define USART_CR1_WAKE			11
#define USART_CR1_M				12
#define USART_CR1_UE			13
#define USART_CR1_OVER8			15

#define USART_CR2_ADD			0
#define USART_CR2_LBDL			5
#define USART_CR2_LBDIE			6
#define USART_CR2_LBCL			8
#define USART_CR2_CPHA			9
#define USART_CR2_CPOL			10
#define USART_CR2_CLKEN			11
#define USART_CR2_STOP			12
#define USART_CR2_LINEN			14

#define USART_CR3_EIE			0
#define USART_CR3_IREN			1
#define USART_CR3_IRLP			2
#define USART_CR3_HDSEL			3
#define USART_CR3_NACK			4
#define USART_CR3_SCEN			5
#define USART_CR3_DMAR			6
#define USART_CR3_DMAT			7
#define USART_CR3_RTSE			8
#define USART_CR3_CTSE			9
#define USART_CR3_CTSIE			10
#define USART_CR3_ONEBIT		11

#define USART_GTPR_PSC			0
#define USART_GTPR_GT			8

#include "stm32f407xx_gpio_driver.h"
#include "stm32f407xx_spi_driver.h"
#include "stm32f407xx_i2c_driver.h"
#include "stm32f407xx_usart_driver.h"
#include "stm32f407xx_rcc_driver.h"

#endif /* INC_STM32F407XX_H_ */
