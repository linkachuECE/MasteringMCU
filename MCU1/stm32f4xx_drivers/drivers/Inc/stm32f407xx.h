/*
 * stm32f407xx.h
 *
 *  Created on: Aug 31, 2022
 *      Author: egbra
 */

#ifndef INC_STM32F407XX_H_
#define INC_STM32F407XX_H_

#include <stdint.h>

#define __vo volatile

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
} SPI_I2S_RegDef_t;

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
#define SPI4_PCLK_EN()		( RCC->APB2ENR |= (1 << 13) )
#define SPI5_PCLK_EN()		( RCC->APB2ENR |= (1 << 20) )
#define SPI6_PCLK_EN()		( RCC->APB2ENR |= (1 << 21) )

// UART/USART ENABLE
#define USART1_PCLK_EN()	( RCC->APB2ENR |= (1 << 4) )
#define USART2_PCLK_EN()	( RCC->APB1ENR |= (1 << 17) )
#define USART3_PCLK_EN()	( RCC->APB1ENR |= (1 << 18) )
#define UART4_PCLK_EN()		( RCC->APB1ENR |= (1 << 19) )
#define UART5_PCLK_EN()		( RCC->APB1ENR |= (1 << 20) )

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
#define SPI4_PCLK_DI()		( RCC->APB2ENR &= ~(1 << 13) )
#define SPI5_PCLK_DI()		( RCC->APB2ENR &= ~(1 << 20) )
#define SPI6_PCLK_DI()		( RCC->APB2ENR &= ~(1 << 21) )

// UART/USART DISABLE
#define USART1_PCLK_DI()	( RCC->APB2ENR &= ~(1 << 4) )
#define USART2_PCLK_DI()	( RCC->APB1ENR &= ~(1 << 17) )
#define USART3_PCLK_DI()	( RCC->APB1ENR &= ~(1 << 18) )
#define UART4_PCLK_DI()		( RCC->APB1ENR &= ~(1 << 19) )
#define UART5_PCLK_DI()		( RCC->APB1ENR &= ~(1 << 20) )

// SYSCFG DISABLE
#define SYSCFG_PCLK_DI()	( RCC->APB2ENR &= ~(1 << 14) )


// GENERIC MACROS
#define ENABLE 1
#define DISABLE 0
#define SET ENABLE
#define RESET DISABLE
#define GPIO_SET SET
#define GPIO_RESET RESET

#endif /* INC_STM32F407XX_H_ */
