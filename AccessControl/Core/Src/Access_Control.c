/*
 * Access_Control.c
 *
 *  Created on: Aug 10, 2020
 *      Author: Borna
 */

#include "main.h"
#include "UART_Circular_Buffer.h"
#include "Access_Control.h"

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
extern char randomNumbersBuffer[15];
extern char dataGSMAlert[3];
extern char phoneNumber[9];
extern uint8_t ALARM;


/**
  * @brief  Swap numbers function
  * @param  pointers to two unsigned integers
  * @retval None
  */
void Swap(uint8_t *a, uint8_t *b)
{
	uint8_t temp = *a;
	*a = *b;
	*b = temp;
}

/**
  * @brief  Generating a random permutation of a finite sequence function
  * @param  array members
  * @param	size of array
  * @retval None
  */
void ShuffleNumbers(uint8_t arr[], uint8_t n)
{
	srand(time(NULL));
	for (uint8_t i = n - 1; i > 0; i--)
	{
		uint8_t j = rand() % (i + 1);
		Swap(&arr[i], &arr[j]);
	}
}

/**
  * @brief  Sending message of shuffled numbers to Nextion
  * @param  array members
  * @param	size of array
  * @retval None
  */
void PrintArray(uint8_t arr[], uint8_t n)
{
	for (uint8_t i = 0; i < n; i++)
	{
		memset(randomNumbersBuffer, '\0', sizeof(randomNumbersBuffer));
		sprintf(randomNumbersBuffer, "b%d.txt=\"%d\"", i, arr[i]);
		strcat((char *)randomNumbersBuffer, (char *)dummyValue);
		UART_String_Transmit(randomNumbersBuffer, &huart1);
	}
}

/**
  * @brief  Printing Date and Time to Data Logger Terminal
  * @param  None
  * @retval None
  */
void Print_TimeInfo(void)
{
	char showTime[50];		//Array for Date and Time message

	memset(&gDateStruct, 0, sizeof(gDateStruct));
	memset(&gTimeStruct, 0, sizeof(gTimeStruct));

	/* Get the RTC current Time */
	HAL_RTC_GetTime(&hrtc, &gTimeStruct, RTC_FORMAT_BIN);

	/* Get the RTC current Date */
	HAL_RTC_GetDate(&hrtc, &gDateStruct, RTC_FORMAT_BIN);

	/* Printing Date to terminal */
	sprintf((char *)showTime, "%02d.%02d.%2d.\t", gDateStruct.Date, gDateStruct.Month, 2000 + gDateStruct.Year);
	UART_String_Transmit(showTime, &huart2);

	memset(showTime, '\0', sizeof(showTime));	//Cleaning array message

	/* Printing Time to terminal */
	sprintf((char *)showTime, "%02d:%02d:%02d\t", gTimeStruct.Hours, gTimeStruct.Minutes, gTimeStruct.Seconds);
	UART_String_Transmit(showTime, &huart2);
}

/**
  * @brief  Printing Message to Data Logger Terminal
  * @param  Received message
  * @retval None
  */
void Print_Log_Terminal(uint8_t value)
{
	char data[35] = {'\0'};		//Array for message

	switch (value)
	{
		case STATE_UNLOCKED:
		{
			memset(data, '\0', sizeof(data));
			Print_TimeInfo();
			sprintf((char *)data, "ACCESS GRANTED\n\r");
			UART_String_Transmit(data, &huart2);
			break;
		}
		case STATE_LOCKED:
		{
			memset(data, '\0', sizeof(data));
			Print_TimeInfo();
			sprintf((char *)data, "STATE UNLOCKED\n\r");
			UART_String_Transmit(data, &huart2);
			break;
		}
		case ALARM_ACTIVE:
		{
			memset(data, '\0', sizeof(data));
			Print_TimeInfo();
			sprintf((char *)data, "ALARM ACTIVATED\n\r");
			UART_String_Transmit(data, &huart2);
			break;
		}
		case ALARM_INACTIVE:
		{
			memset(data, '\0', sizeof(data));
			Print_TimeInfo();
			sprintf((char *)data, "ALARM DEACTIVATED\n\r");
			UART_String_Transmit(data, &huart2);
			break;
		}
		case ACCESS_DENIED:
		{
			memset(data, '\0', sizeof(data));
			Print_TimeInfo();
			sprintf((char *)data, "ACCESS DENIED\n\r");
			UART_String_Transmit(data, &huart2);
			break;
		}
		case ALARM_ALERT:
		{
			memset(data, '\0', sizeof(data));
			Print_TimeInfo();
			sprintf((char *)data, "ALARM - ALERT!!!!\n\r");
			UART_String_Transmit(data, &huart2);
			UART_String_Transmit(dataGSMAlert, &huart3);
			break;
		}
		case SETTINGS_CHANGED:
		{
			memset(data, '\0', sizeof(data));
			Print_TimeInfo();
			sprintf((char *)data, "SETTINGS CHANGED\n\r");
			UART_String_Transmit(data, &huart2);
			UART_String_Transmit(phoneNumber, &huart3);
			break;
		}
		case ACCESS_PASS_SET:
		{
			memset(data, '\0', sizeof(data));
			Print_TimeInfo();
			sprintf((char *)data, "ACCESS PASSWORD - SET\n\r");
			UART_String_Transmit(data, &huart2);
			break;
		}
		case ALARM_PASS_SET:
		{
			memset(data, '\0', sizeof(data));
			Print_TimeInfo();
			sprintf((char *)data, "ALARM PASSWORD - SET\n\r");
			UART_String_Transmit(data, &huart2);
			break;
		}
		case ACCESS_PASS_CHANGE:
		{
			memset(data, '\0', sizeof(data));
			Print_TimeInfo();
			sprintf((char *)data, "ACCESS PASSWORD - CHANGED\n\r");
			UART_String_Transmit(data, &huart2);
			break;
		}
		case ALARM_PASS_CHANGE:
		{
			memset(data, '\0', sizeof(data));
			Print_TimeInfo();
			sprintf((char *)data, "ALARM PASSWORD - CHANGED\n\r");
			UART_String_Transmit(data, &huart2);
			break;
		}
		default:
			return;
	 }
}
