/*
 * Encoder.h
 *
 *  Created on: Jul 14, 2025
 *      Author: Danil
 */

#ifndef INC_ENCODER_H_
#define INC_ENCODER_H_

//#include "stm32f4xx_hal.h"
#include "System_status.h"
#include "Stepper_Motor_Driver.h"

typedef struct {
  GPIO_TypeDef* Port;
  unsigned short Pin;
} Bundle_pirt_Pin;

typedef struct {
  unsigned short Pulses_per_revolution;
  unsigned short Maximum;
  unsigned short Minimum;
} Pulses;

typedef struct {
  Bundle_pirt_Pin Port_A;
  Bundle_pirt_Pin Port_B;
  TIM_HandleTypeDef *Timer;
  Pulses Puls;
} Encoder;

extern Encoder Encoder_AZ;
extern Encoder Encoder_EL;

void Encoder_Init(Encoder *Encoder_xx);
void Encoder_GetAngular(Encoder *Encoder_xx);

#endif /* INC_ENCODER_H_ */
