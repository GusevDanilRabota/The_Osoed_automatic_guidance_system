/*
 * Stepper_Motor_Driver.h
 *
 *  Created on: Jul 14, 2025
 *      Author: Danil
 */

#ifndef INC_STEPPER_MOTOR_DRIVER_H_
#define INC_STEPPER_MOTOR_DRIVER_H_

#include "Macros.h"
#include "Typedef_pins.h"

typedef struct {
  Bundle_Port_Pin ENA;
  Bundle_Port_Pin DIR;
} Pins;

typedef struct {
  TIM_HandleTypeDef *Number_timer;
  unsigned int       Channel;
} Timer;

typedef struct {
  unsigned int Maximum;
  unsigned int Minimum;
  unsigned int StepUp;
} Frequency_parameters;

typedef struct {
  Pins                 Pins;
  Timer                Timer;
  Frequency_parameters Frequency;
} System_parameters;

typedef struct {
  unsigned int  Frequency;
  unsigned char Functioning;
  unsigned char Direction_of_rotation;
  unsigned int  Tik[2];
} System_status;

typedef struct {
  System_parameters Parameters;
  System_status     Status;
} Motor;

extern Motor Motor_AZ;
extern Motor Motor_EL;

void Pin_SetLevl(Bundle_Port_Pin *Pin, Levl_signal State);

void Motor_SetFrequency(Motor *Motor_xx, unsigned int frequency);
void Motor_UpFrequency(Motor *Motor_xx);
void Motor_DirRot(Motor *Motor_xx, Levl_signal direction_of_rotation);
void Motor_Start(Motor *Motor_xx);
void Motor_Stop(Motor *Motor_xx);

#endif /* INC_STEPPER_MOTOR_DRIVER_H_ */
