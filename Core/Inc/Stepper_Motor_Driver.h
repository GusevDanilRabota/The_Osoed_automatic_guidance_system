/*
 * Stepper_Motor_Driver.h
 *
 *  Created on: Jul 14, 2025
 *      Author: Danil
 */

#ifndef INC_STEPPER_MOTOR_DRIVER_H_
#define INC_STEPPER_MOTOR_DRIVER_H_

#include "stm32f4xx_hal.h"

//typedef enum
//{
//	Left = 0,
//	Right
//} Direction_rotation_AZ;
//
//typedef enum
//{
//	Up = 0,
//	Down
//} Direction_rotation_EL;
//
//typedef enum
//{
//	Work = 0,
//	Sleep
//} Operating_mode;
//
//typedef enum
//{
//	NotFunctioning = 0,
//	Functioning
//} Functioning_mode;

typedef enum
{
	Low = 0,
	Hight
} Levl_signal;

typedef struct {
  GPIO_TypeDef* Port;
  unsigned short Pin;
} Bundle_Port_Pin;

typedef struct {
  Bundle_Port_Pin ENA;
  Bundle_Port_Pin PUL;
  Bundle_Port_Pin DIR;
} Pins;

typedef struct {
  unsigned int Maximum_pulse;
  unsigned int Minimum_pulse;
  unsigned int Step_up_the_pulse;
} Frequency_settings;

typedef struct {
  TIM_HandleTypeDef *Number;
  unsigned int Channel;
} Timers;

typedef struct {
  Pins Pin;
  Frequency_settings Frequency;
  Timers Timer;
} Settings;

typedef struct {
  unsigned int Frequency;
  unsigned char Functioning;
  unsigned int Old_value_tik;
  unsigned int New_value_tik;
} States;

typedef struct {
  Settings Setting;
  States State;
} Motor;

extern Motor Motor_AZ;
extern Motor Motor_EL;

void Pin_SetLevl(Bundle_Port_Pin *Pin, int State);

void Motor_SetFrequency(Motor *Motor_xx, unsigned int frequency);
void Motor_UpFrequency(Motor *Motor_xx);
void Motor_DirRot(Motor *Motor_xx, GPIO_PinState direction_of_rotation);
void Motor_Start(Motor *Motor_xx);
void Motor_Stop(Motor *Motor_xx);

#endif /* INC_STEPPER_MOTOR_DRIVER_H_ */
