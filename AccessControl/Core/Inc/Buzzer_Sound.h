/*
 * Buzzer_Sound.h
 *
 *  Created on: Aug 10, 2020
 *      Author: Borna
 */

#ifndef INC_BUZZER_SOUND_H_
#define INC_BUZZER_SOUND_H_

extern TIM_HandleTypeDef htim2;

void Sound_Off(void);
void Sound_Press(void);
void Sound_Password_Correct(void);
void Sound_Password_Invalid(void);
void Sound_Alarm_Alert(void);

#endif /* INC_BUZZER_SOUND_H_ */
