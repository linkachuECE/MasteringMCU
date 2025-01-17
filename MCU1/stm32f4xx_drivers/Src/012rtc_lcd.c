/*
 * 012rtc_lcd.c
 *
 *  Created on: Oct 24, 2022
 *      Author: egbra
 */

#include "stm32f407xx.h"
#include "ds1307.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

char* getDayOfWeek(RTC_date_t* rtc_date);
char* getMonth(RTC_date_t* rtc_date);

char* dateToString(RTC_date_t* rtc_date);
char* timeToString(RTC_time_t* rtc_time);

int main(void){
	RTC_time_t rtc_time = { .seconds = 0,
							.minutes = 0,
							.hours = 0,
							.time_format = TIME_FORMAT_24HRS };
	RTC_date_t rtc_date = { .dayOfWeek = SUNDAY,
							.date = 1,
							.month = JANUARY,
							.year = 2000 };

	printf("RTC Test:\n");

	if(ds1307_init()){
		printf("RTC Init has failed...\n");
		while(1);
	}

	ds1307_set_current_time(&rtc_time);
	ds1307_set_current_date(&rtc_date);

	ds1307_get_current_time(&rtc_time);
	ds1307_get_current_date(&rtc_date);

	printf("Current time: %s\n", timeToString(&rtc_time));

	printf("Current date: %s\n", dateToString(&rtc_date));

	return 0;
}

char* timeToString(RTC_time_t* rtc_time){
	char* ampm;
	if (rtc_time->time_format == TIME_FORMAT_12HRS)
		ampm = (rtc_time->hours < 12) ? " PM" : " AM";
	else
		ampm = "";

	char* output = (char*)malloc(sizeof(char) * 10);
	sprintf(output, "%d:%d:%d%s",	rtc_time->hours,
									rtc_time->minutes,
									rtc_time->seconds,
									ampm);

	return output;
}

char* dateToString(RTC_date_t* rtc_date){
	char* output = (char*)malloc(sizeof(char) * 20);

	sprintf(output, "%s, %s %d, %d",	getDayOfWeek(rtc_date),
										getMonth(rtc_date),
										rtc_date->date,
										rtc_date->year);

	return output;
}

char* getDayOfWeek(RTC_date_t* rtc_date){

	char* dayOfWeek = (char*)malloc(sizeof(char) * 15);

	switch (rtc_date->dayOfWeek) {
		case SUNDAY:
			dayOfWeek = "Sunday";
			break;
		case MONDAY:
			dayOfWeek = "Monday";
			break;
		case TUESDAY:
			dayOfWeek = "Tuesday";
			break;
		case WEDNESDAY:
			dayOfWeek = "Wednesday";
			break;
		case THURSDAY:
			dayOfWeek = "Thursday";
			break;
		case FRIDAY:
			dayOfWeek = "Friday";
			break;
		case SATURDAY:
			dayOfWeek = "Saturday";
			break;
	}

	return dayOfWeek;
}

char* getMonth(RTC_date_t* rtc_date){
	char* month = (char*)malloc(sizeof(char) * 15);

	switch (rtc_date->month) {
		case JANUARY:
			month = "January";
			break;
		case FEBRUARY:
			month = "February";
			break;
		case MARCH:
			month = "March";
			break;
		case APRIL:
			month = "April";
			break;
		case MAY:
			month = "May";
			break;
		case JUNE:
			month = "June";
			break;
		case JULY:
			month = "July";
			break;
		case AUGUST:
			month = "August";
			break;
		case SEPTEMBER:
			month = "September";
			break;
		case OCTOBER:
			month = "October";
			break;
		case NOVEMBER:
			month = "November";
			break;
		case DECEMBER:
			month = "December";
			break;
	}

	return month;
}
