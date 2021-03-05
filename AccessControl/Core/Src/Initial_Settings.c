/*
 * Initial_Settings.c
 *
 *  Created on: Aug 10, 2020
 *      Author: Borna
 */

#include "main.h"
#include "Initial_Settings.h"
#include "UART_Circular_Buffer.h"

/* External Date and Time structures */
extern RTC_DateTypeDef gDateStruct;
extern RTC_TimeTypeDef gTimeStruct;

/* External Arrays */
extern char dummyValue[3];
extern char showDateTime_date[15];
extern char showDateTime_mon[15];
extern char showDateTime_year[15];
extern char showDateTime_hr[15];
extern char showDateTime_min[15];
extern char phoneNumber[9];


/**
  * @brief Decimal number to Binary Coded Dedcimal conversion function
  * @param Decimal number
  * @retval BCD number
  */
uint8_t DecToBcd(int val)
{
    return (uint8_t)((val / 10 * 16) + (val % 10));
}

/**
  * @brief RTC set the Time and Date function
  * @param None
  * @retval None
  */
void RTC_ConfigurationDateTime(void)
{
	uint8_t dateTimeReceive[21] = {0};

	/* Wait until Date and Time are received */
	while (1)
	{
		HAL_UART_Receive(&huart1, dateTimeReceive, 21, 100);

		if (dateTimeReceive[0] != 0)
		{
			break;
		}
	}

	/* Date and Time Set Structures */
	RTC_DateTypeDef  sDateStruct = {0};
	RTC_TimeTypeDef  sTimeStruct = {0};

	/* Converting received data to set Date */
	sDateStruct.Date = DecToBcd(dateTimeReceive[0]);
	sDateStruct.Month = DecToBcd(dateTimeReceive[4]);
	sDateStruct.Year = DecToBcd(dateTimeReceive[8]);
	HAL_RTC_SetDate(&hrtc, &sDateStruct, RTC_FORMAT_BCD);

	/* Converting received data to set Time */
	sTimeStruct.Hours = DecToBcd(dateTimeReceive[12]);
	sTimeStruct.Minutes = DecToBcd(dateTimeReceive[16]);
	sTimeStruct.TimeFormat = RTC_HOURFORMAT_24;
	sTimeStruct.DayLightSaving = RTC_DAYLIGHTSAVING_NONE ;
	sTimeStruct.StoreOperation = RTC_STOREOPERATION_RESET;
	HAL_RTC_SetTime(&hrtc, &sTimeStruct, RTC_FORMAT_BCD);

	memset(dateTimeReceive, 0, sizeof(dateTimeReceive));	//Cleaning Array

	__HAL_RTC_CLEAR_FLAG(RTC_BKP_DR1);
	HAL_RTCEx_BKUPWrite(&hrtc, RTC_BKP_DR1, 0x32F2);  // Writing Date and Time to Backup Register in case of reset

	Get_PhoneNumber();
}

/**
  * @brief Phone Number Storing Function
  * @param None
  * @retval None
  */
void Get_PhoneNumber(void)
{
	char number[10] = {'\0'};

	/* Wait until Phone Number is received */
	while (1)
	{
		HAL_UART_Receive(&huart1, (uint8_t *)number, 10, 100);

		if (number[0] != '\0')
		{
			break;
		}
	}

	/* Removing the first element of the array - starting zero */
	for(uint8_t i = 0; i < 9; ++i)
	{
		phoneNumber[i] = number[i+1];
	}
}




