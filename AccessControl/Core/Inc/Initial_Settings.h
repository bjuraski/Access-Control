/*
 * Initial_Settings.h
 *
 *  Created on: Aug 10, 2020
 *      Author: Borna
 */

#ifndef INC_INITIAL_SETTINGS_H_
#define INC_INITIAL_SETTINGS_H_

/* External Peripheral Handlers */
extern UART_HandleTypeDef huart1;
extern UART_HandleTypeDef huart3;
extern TIM_HandleTypeDef htim7;
extern RTC_HandleTypeDef hrtc;

/* Function Prototypes */
uint8_t DecToBcd(int val);
void RTC_ConfigurationDateTime(void);
void Get_PhoneNumber(void);

#endif /* INC_INITIAL_SETTINGS_H_ */
