#include "stm32f407xx.h"
#include "ds1307.h"

#include <stdint.h>
#include <string.h>

static void ds1307_i2c_pin_config(void);
static void ds1307_i2c_config(void);

static uint8_t ds1307_read(uint8_t regAddress);
static void ds1307_write(uint8_t value, uint8_t regAddress);

static uint8_t binaryToBCD(uint8_t bin);
static uint8_t BCDToBinary(uint8_t bcd);

I2C_Handle_t g_ds1307I2cHandle;

/*********************************************************************
 * @fn      		  - ds1307_init
 *
 * @brief             - Initializes the ds1307 RTC
 *
 * @return            - None
 *
 * @Note              - None
 */
uint8_t ds1307_init(){
	// 1. Inititialize the I2C pins
	ds1307_i2c_pin_config();

	// 2. Initialize the I2C peripheral
	ds1307_i2c_config();

	// 3. Enable the I2C peripheral
	I2C_PeripheralControl(&g_ds1307I2cHandle, ENABLE);

	// 4. Make clock halt = 0
	ds1307_write(0X00, DS1307_ADDR_SEC);

	// 5. Read back clock halt bit
	uint8_t clockState = ds1307_read(DS1307_ADDR_SEC);

	return (clockState >> 7) & 0x01;
}

/*********************************************************************
 * @fn      		  - ds1307_set_current_time
 *
 * @brief             - Sets the time info for the RTC
 *
 * @param[in]		  - Pointer to RTC time handle
 *
 * @return            - None
 *
 * @Note              - None
 */
void ds1307_set_current_time(RTC_time_t* rtc_time){
	uint8_t tens, ones;
	uint8_t AMorPM, timeFormat;

	uint8_t tempReg = 0x00;

	// Set seconds
	tempReg = binaryToBCD(rtc_time->seconds);
	ds1307_write(tempReg, DS1307_ADDR_SEC);

	// Set minutes
	tempReg = binaryToBCD(rtc_time->minutes);
	ds1307_write(tempReg, DS1307_ADDR_MIN);
	tempReg = 0x00;

	// Set hours
	timeFormat = rtc_time->time_format;

	// 12-hour format
	if(timeFormat == TIME_FORMAT_12HRS){
		AMorPM = rtc_time->hours / 12;
		tens = (rtc_time->hours % 12) / 10;
		ones = (rtc_time->hours % 12) % 10;

		tempReg |= ones;
		tempReg |= (tens << 4);
		tempReg |= (AMorPM << 5);
		tempReg |= (timeFormat << 6);
	}
	// 24-hour format
	else {
		tens = rtc_time->hours / 10;
		ones = rtc_time->hours % 10;

		tempReg |= ones;
		tempReg |= (tens << 4);
		tempReg |= (timeFormat << 6);
	}

	ds1307_write(tempReg, DS1307_ADDR_HRS);
}

/*********************************************************************
 * @fn      		  - ds1307_get_current_time
 *
 * @brief             - Retrieves the time info from the RTC
 *
 * @param[in]		  - Pointer to RTC time handle
 *
 * @return            - None
 *
 * @Note              - None
 */
void ds1307_get_current_time(RTC_time_t* rtc_time){
	uint8_t tens, ones;
	uint8_t AMorPM, timeFormat;

	uint8_t tempReg = 0x00;

	// Retrieve seconds
	tempReg = ds1307_read(DS1307_ADDR_SEC);
	rtc_time->seconds = BCDToBinary(tempReg);

	// Retrieve minutes
	tempReg = ds1307_read(DS1307_ADDR_MIN);
	rtc_time->minutes = BCDToBinary(tempReg);

	// Retrieve time format
	tempReg = ds1307_read(DS1307_ADDR_HRS);
	timeFormat = (tempReg >> 6) & 0x01;
	rtc_time->time_format = timeFormat;

	// Retrieve hours
	// 12-hour format
	if(timeFormat == TIME_FORMAT_12HRS){
		ones = tempReg & 0x0F;
		tens = (tempReg >> 4) & 0x01;
		AMorPM = (tempReg >> 5) & 0x01;

		// Convert to 24-hour format for storage
		rtc_time->hours = ((tens * 10) + ones) + (12 * AMorPM);
	}
	// 24-hour format
	else {
		ones = tempReg & 0x0F;
		tens = (tempReg >> 4) & 0x03;

		rtc_time->hours = (tens * 10) + ones;
	}
}

/*********************************************************************
 * @fn      		  - ds1307_set_current_date
 *
 * @brief             - Sets the date info for the RTC
 *
 * @param[in]		  - Pointer to RTC time handle
 *
 * @return            - None
 *
 * @Note              - None
 */
void ds1307_set_current_date(RTC_date_t* rtc_date){
	uint8_t tempReg = 0x00;

	// Set day of the week
	tempReg |= rtc_date->dayOfWeek;
	ds1307_write(tempReg, DS1307_ADDR_DAY);

	// Set date of the month
	tempReg = binaryToBCD(rtc_date->date);
	ds1307_write(tempReg, DS1307_ADDR_DATE);

	// Set month
	tempReg = binaryToBCD(rtc_date->month);
	ds1307_write(tempReg, DS1307_ADDR_MONTH);

	// Set year (subtract 2000 to store just the tens and ones)
	tempReg = binaryToBCD(rtc_date->date - 2000);
	ds1307_write(tempReg, DS1307_ADDR_YEAR);
}

/*********************************************************************
 * @fn      		  - ds1307_get_current_date
 *
 * @brief             - Retrieves the date info from the RTC
 *
 * @param[in]		  - Pointer to RTC time handle
 *
 * @return            - None
 *
 * @Note              - None
 */
void ds1307_get_current_date(RTC_date_t* rtc_date){
	uint8_t tempReg = 0x00;

	// Get day of the week
	tempReg = ds1307_read(DS1307_ADDR_DAY);
	rtc_date->dayOfWeek = tempReg;

	// Get date of the month
	tempReg = ds1307_read(DS1307_ADDR_DATE);
	rtc_date->date = BCDToBinary(tempReg);

	// Get month
	tempReg = ds1307_read(DS1307_ADDR_MONTH);
	rtc_date->month = BCDToBinary(tempReg);

	// Get year
	tempReg = ds1307_read(DS1307_ADDR_YEAR);
	rtc_date->year = BCDToBinary(tempReg) + 2000;
}

/*********************************************************************
 * @fn      		  - ds1307_i2c_pin_config
 *
 * @brief             - Configures the GPIO pins for I2C
 *
 * @param[in]		  - None
 *
 * @return            - None
 *
 * @Note              - None
 */
static void ds1307_i2c_pin_config(void){
	GPIO_Handle_t i2c_sda, i2c_scl;

	memset(&i2c_sda,0,sizeof(i2c_sda));
	memset(&i2c_scl,0,sizeof(i2c_scl));

	i2c_sda.pGPIOx = DS1307_I2C_GPIO_PORT;
	i2c_sda.GPIO_PinConfig.GPIO_PinAltFunMode = 4;
	i2c_sda.GPIO_PinConfig.GPIO_PinMode = GPIO_MODE_ALTFN;
	i2c_sda.GPIO_PinConfig.GPIO_PinNumber = DS1307_I2C_SDA_PIN;
	i2c_sda.GPIO_PinConfig.GPIO_PinOPType = GPIO_OP_TYPE_OD;
	i2c_sda.GPIO_PinConfig.GPIO_PinPuPdControl = DS1307_I2C_PUPD;
	i2c_sda.GPIO_PinConfig.GPIO_PinSpeed = GPIO_SPEED_FAST;

	GPIO_Init(&i2c_sda);
	GPIO_Init(&i2c_scl);
}

/*********************************************************************
 * @fn      		  - ds1307_i2c_config
 *
 * @brief             - Configures the I2C Peripheral
 *
 * @param[in]		  - None
 *
 * @return            - None
 *
 * @Note              - None
 */
static void ds1307_i2c_config(void){
	g_ds1307I2cHandle.pI2Cx = DS1307_I2C;
	g_ds1307I2cHandle.I2C_Config.AckControl = I2C_ACK_ENABLE;
	g_ds1307I2cHandle.I2C_Config.SCLSpeed = DS1307_I2C_SPEED;
	I2C_Init(&g_ds1307I2cHandle);
}

/*********************************************************************
 * @fn      		  - ds1307_write
 *
 * @brief             - Writes info to the RTC
 *
 * @param[in]		  - Value to write
 * @param[in]		  - Register to write to
 *
 * @return            - None
 *
 * @Note              - None
 */
static void ds1307_write(uint8_t value, uint8_t reg_address){
	uint8_t tx[2];
	tx[0] = reg_address;
	tx[1] = value;

	I2C_MasterSendData(&g_ds1307I2cHandle, tx, 2, DS1307_I2C_ADDRESS, 0);
}

/*********************************************************************
 * @fn      		  - ds1307_read
 *
 * @brief             - Reads info from a register in the RTC
 *
 * @param[in]		  - Register to read from
 *
 * @return            - Retrieved value
 *
 * @Note              - None
 */
static uint8_t ds1307_read(uint8_t regAddress){
	uint8_t data;
	// 1. Send word address to read from
	I2C_MasterSendData(&g_ds1307I2cHandle, &regAddress, 1, DS1307_I2C_ADDRESS, 1);

	// 2. Read data from that address
	I2C_MasterReceiveData(&g_ds1307I2cHandle, &data, 1, DS1307_I2C_ADDRESS, 0);

	return data;
}

/*********************************************************************
 * @fn      		  - binaryToBCD
 *
 * @brief             - Converts a binary value to BCD format
 *
 * @param[in]		  - Binary value
 *
 * @return            - Corresponding BCD value
 *
 * @Note              - None
 */
static uint8_t binaryToBCD(uint8_t bin){
	uint8_t tens, ones;
	uint8_t result = 0x00;
	tens = bin / 10;
	ones = bin % 10;

	result |= ones;
	result |= (tens << 4);

	return result;
}

/*********************************************************************
 * @fn      		  - BCDToBinary
 *
 * @brief             - Converts a BCD value to binary format
 *
 * @param[in]		  - BCD value
 *
 * @return            - Corresponding binary value
 *
 * @Note              - None
 */
static uint8_t BCDToBinary(uint8_t bcd){
	uint8_t tens, ones;
	uint8_t result = 0x00;
	ones = bcd & 0x0F;
	tens = (bcd >> 4) & 0x0F;

	result = (tens * 10) + ones;
	return result;
}
