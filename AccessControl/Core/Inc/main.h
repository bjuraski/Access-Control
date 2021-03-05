/*
 * main.h
 *
 *  Created on: Jul 14, 2020
 *      Author: Borna
 */

#ifndef INC_MAIN_H_
#define INC_MAIN_H_

#include "stm32f4xx_hal.h"

#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>

#define FALSE	0
#define TRUE	1

/************************ System Clock Frequencies *****************************/
#define SYS_CLOCK_FREQ_50_MHZ	50
#define SYS_CLOCK_FREQ_84_MHZ	84
#define SYS_CLOCK_FREQ_120_MHZ	120
#define SYS_CLOCK_FREQ_180_MHZ	180
/******************************************************************************/

/************************ Received Data Constants *****************************/
#define SOUND_BEEP			2
#define RANDOMIZE			3
#define STATE_UNLOCKED		4
#define STATE_LOCKED		5
#define ALARM_ACTIVE 		6
#define ALARM_INACTIVE		7
#define SOUND_ON_VALID		8
#define ACCESS_DENIED		9
#define ALARM_ALERT			10
#define SETTINGS_CHANGED	11
#define ACCESS_PASS_SET		12
#define ALARM_PASS_SET		13
#define ACCESS_PASS_CHANGE	14
#define ALARM_PASS_CHANGE	15
//#define SOUND_OFF			16
/******************************************************************************/

/********************* RTC Configuration Constants ****************************/
#define RTC_CLOCK_SOURCE_LSE
//#define RTC_CLOCK_SOURCE_LSI

#define RTC_ASYNCH_PREDIV	0x7F
#define RTC_SYNCH_PREDIV	0xFF
/******************************************************************************/

/************************* GPIO Pins Definitions ******************************/
#define BUZZER_GPIO_PORT	GPIOA
#define BUZZER_GPIO_PIN		GPIO_PIN_0

#define PIR_GPIO_PORT		GPIOA
#define PIR_GPIO_PIN		GPIO_PIN_1

#define LED_GPIO_PORT		GPIOA
#define LED_GPIO_PIN		GPIO_PIN_5

#define RELAY_GPIO_PORT		GPIOA
#define RELAY_GPIO_PIN		GPIO_PIN_6

#define BUTTON_GPIO_PORT	GPIOC
#define BUTTON_GPIO_PIN		GPIO_PIN_13
/******************************************************************************/

/* Function Prototypes */
void Error_Handler(void);
void PWM_TIM_GPIO_Init(TIM_HandleTypeDef *htim);

#endif /* INC_MAIN_H_ */
