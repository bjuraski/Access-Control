/*
 * main.c
 *
 *  Created on: Aug 10, 2020
 *      Author: Borna
 */

#include "main.h"
#include "System_Configuration.h"
#include "Peripheral_Initialization.h"
#include "Initial_Settings.h"
#include "Access_Control.h"
#include "Buzzer_Sound.h"
#include "UART_Circular_Buffer.h"

/* Peripheral Handlers */
UART_HandleTypeDef huart1;
UART_HandleTypeDef huart2;
UART_HandleTypeDef huart3;
TIM_HandleTypeDef htim2;
TIM_HandleTypeDef htim7;
RTC_HandleTypeDef hrtc;

/* Date and Time structures */
RTC_DateTypeDef gDateStruct = {0};
RTC_TimeTypeDef gTimeStruct = {0};

/* Values for transmitting messages to Nextion */
char dummyValue[3] = {0xff, 0xff, 0xff};

/* Arrays for transmitting Date and Time */
char showDateTime_date[15] = {'\0'};
char showDateTime_mon[15] = {'\0'};
char showDateTime_year[15] = {'\0'};
char showDateTime_hr[15] = {'\0'};
char showDateTime_min[15] = {'\0'};

char randomNumbersBuffer[15] = {'\0'};
char buttonExit[20] = {'\0'};
char motionDetector[15] = {'\0'};
char phoneNumber[9] = {'\0'};
char receivedData[4] = {'\0'};
char dataGSMAlert[3] = "GSM";
uint8_t arr[10] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9};
uint8_t n = sizeof(arr) / sizeof(arr[0]);
uint8_t ALARM = FALSE;

int main(void)
{
	/* Reset of all peripherals, Initializes the Flash interface and the Systick. */

	HAL_Init();

	/* Configure the system clock */
	SystemClock_Config_HSE(SYS_CLOCK_FREQ_180_MHZ);

	/* Initialize all configured peripherals */
	GPIO_Init();
	UART1_Init();
	UART2_Init();
	UART3_Init();
	TIMER2_Init();
	TIMER7_Init();
	RTC_Init();

	RTC_ConfigurationDateTime();	//Set Date and Time

	if (HAL_RTCEx_BKUPRead(&hrtc, RTC_BKP_DR1) != 0x32F2)
	{
		RTC_ConfigurationDateTime();	//Get correct Date and Time after system reset
	}

	RingBuffer_Init();
	HAL_TIM_Base_Start_IT(&htim7);

	/* Infinite loop */
	while (1)
	{
		while (Is_Data_Received(&huart1))
		{
			receivedData[0] = UART_Read(&huart1);
		}

		switch (receivedData[0])
		{
			case SOUND_BEEP:	//Sound during touch-screen press
			{
				Sound_Press();
				receivedData[0] = 0;
				break;
			}
			case RANDOMIZE:		//Generate Random numbers of finite sequence
			{
				ShuffleNumbers(arr, n);
				PrintArray(arr, n);
				receivedData[0] = 0;
				break;
			}
			case STATE_UNLOCKED:	//Access Granted, Unlock
			{
				Sound_Password_Correct();
				Print_Log_Terminal(STATE_UNLOCKED);
				HAL_GPIO_WritePin(RELAY_GPIO_PORT, RELAY_GPIO_PIN, GPIO_PIN_RESET);
				receivedData[0] = 0;
				break;
			}
			case STATE_LOCKED:	//Access Denied, Lock
			{
				Sound_Password_Correct();
				Print_Log_Terminal(STATE_LOCKED);
				HAL_GPIO_WritePin(RELAY_GPIO_PORT, RELAY_GPIO_PIN, GPIO_PIN_SET);
				receivedData[0] = 0;
				break;
			}
			case ALARM_ACTIVE:	//Alarm Activation, Turn on motion detector
			{
				Sound_Password_Correct();
				Print_Log_Terminal(ALARM_ACTIVE);
				ALARM = TRUE;
				UART_String_Transmit(phoneNumber, &huart3);
				receivedData[0] = 0;
				break;
			}
			case ALARM_INACTIVE:	//Alarm Deactivation, Turn off motion detector
			{
				Sound_Password_Correct();
				Print_Log_Terminal(ALARM_INACTIVE);
				ALARM = FALSE;
				Sound_Off();
				receivedData[0] = 0;
				break;
			}
			case SOUND_ON_VALID:	//Correct Password Sound
			{
				Sound_Password_Correct();
				receivedData[0] = 0;
				break;
			}
			case ACCESS_DENIED:	//Invalid Password Sound
			{
				Sound_Password_Invalid();
				Print_Log_Terminal(ACCESS_DENIED);
				receivedData[0] = 0;
				break;
			}
			case ALARM_ALERT:	//Alarm triggered
			{
				Print_Log_Terminal(ALARM_ALERT);
				Sound_Alarm_Alert();
				HAL_TIM_PWM_Start(&htim2, TIM_CHANNEL_1);
				htim2.Instance->CCR1 = 800;
				receivedData[0] = 0;
				break;
			}
			case SETTINGS_CHANGED:	//General Settings changed
			{
				receivedData[0] = 0;
				HAL_TIM_Base_Stop_IT(&htim7);
				HAL_Delay(500);
				//Get_PhoneNumber();
				Print_Log_Terminal(SETTINGS_CHANGED);
				HAL_TIM_Base_Start_IT(&htim7);
				break;
			}
			case ACCESS_PASS_SET:	//Data logger - Access password set
			{
				receivedData[0] = 0;
				Print_Log_Terminal(ACCESS_PASS_SET);
				break;
			}
			case ALARM_PASS_SET:	//Data logger - Alarm password set
			{
				receivedData[0] = 0;
				Print_Log_Terminal(ALARM_PASS_SET);
				break;
			}
			case ACCESS_PASS_CHANGE:	//Data logger - Access password changed
			{
				receivedData[0] = 0;
				Print_Log_Terminal(ACCESS_PASS_CHANGE);
				break;
			}
			case ALARM_PASS_CHANGE:	//Data logger - Alarm password changed
			{
				receivedData[0] = 0;
				Print_Log_Terminal(ALARM_PASS_CHANGE);
				break;
			}
		}
	}

	return 0;
}


/**
  * @brief  Period elapsed callback in non-blocking mode
  * @param  htim TIM handle
  * @retval None
  */
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
	if (htim->Instance == TIM7)
	{
		memset(&gDateStruct, 0, sizeof(gDateStruct));
		memset(&gTimeStruct, 0, sizeof(gTimeStruct));

		HAL_RTC_GetTime(&hrtc, &gTimeStruct, RTC_FORMAT_BIN);
		HAL_RTC_GetDate(&hrtc, &gDateStruct, RTC_FORMAT_BIN);

		/* Sending Date to Nextion Display for Date display */
		memset(showDateTime_date, '\0', sizeof(showDateTime_date));
		sprintf(showDateTime_date, "date.val=%d", gDateStruct.Date);
		strcat((char *)showDateTime_date, (char *)dummyValue);
		UART_String_Transmit(showDateTime_date, &huart1);

		/* Sending Month to Nextion Display for Date display */
		memset(showDateTime_mon, '\0', sizeof(showDateTime_mon));
		sprintf(showDateTime_mon, "mon.val=%d", gDateStruct.Month);
		strcat((char *)showDateTime_mon, (char *)dummyValue);
		UART_String_Transmit(showDateTime_mon, &huart1);

		/* Sending Year to Nextion Display for Date display */
		memset(showDateTime_year, '\0', sizeof(showDateTime_year));
		sprintf(showDateTime_year, "year.val=%d", 2000 + gDateStruct.Year);
		strcat((char *)showDateTime_year, (char *)dummyValue);
		UART_String_Transmit(showDateTime_year, &huart1);

		/* Sending Hours to Nextion Display for Time display */
		memset(showDateTime_hr, '\0', sizeof(showDateTime_hr));
		sprintf(showDateTime_hr, "hr.val=%d", gTimeStruct.Hours);
		strcat((char *)showDateTime_hr, (char *)dummyValue);
		UART_String_Transmit(showDateTime_hr, &huart1);

		/* Sending Minutes to Nextion Display for Time display */
		memset(showDateTime_min, '\0', sizeof(showDateTime_min));
		sprintf(showDateTime_min, "min.val=%d", gTimeStruct.Minutes);
		strcat((char *)showDateTime_min, (char *)dummyValue);
		UART_String_Transmit(showDateTime_min, &huart1);
	}
}

void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{
	if (GPIO_Pin == BUTTON_GPIO_PIN)	//Exit Button Pressed
	{
		memset(buttonExit, '\0', sizeof(buttonExit));
		sprintf(buttonExit, "page page13");
		strcat((char *)buttonExit, (char *)dummyValue);
		UART_String_Transmit(buttonExit, &huart1);
		memset(buttonExit, '\0', sizeof(buttonExit));
		sprintf(buttonExit, "BUTTON-EXIT\n\r");
		Print_TimeInfo();
		UART_String_Transmit(buttonExit, &huart2);
	}
	else if (GPIO_Pin == PIR_GPIO_PIN && ALARM == TRUE)	//Alarm Triggered
	{
		HAL_GPIO_WritePin(LED_GPIO_PORT, LED_GPIO_PIN, GPIO_PIN_SET);
		memset(motionDetector, '\0', sizeof(motionDetector));
		sprintf(motionDetector, "page page37");
		strcat((char *)motionDetector, (char *)dummyValue);
		UART_String_Transmit(motionDetector, &huart1);
		while (HAL_GPIO_ReadPin(PIR_GPIO_PORT, PIR_GPIO_PIN) != GPIO_PIN_RESET);
		HAL_GPIO_WritePin(LED_GPIO_PORT, LED_GPIO_PIN, GPIO_PIN_RESET);
	}
}
