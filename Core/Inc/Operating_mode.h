/*
 * Operating_mode.h
 *
 *  Created on: Jul 19, 2025
 *      Author: u033008
 */

#ifndef INC_OPERATING_MODE_H_
#define INC_OPERATING_MODE_H_

#include "Raspberry.h"
#include "Encoder.h"

#include "stm32f4xx_hal.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

typedef struct {
  float Buffer_differens_angular[2];
  char Requirement_stop;
} Mode_Automatic_data;

extern Mode_Automatic_data Automatic_data_AZ;
extern Mode_Automatic_data Automatic_data_EL;

void Mode_Manual(Motor *Motor_xx);
void Mode_Semi_automatic(Motor *Motor_xx);
void Mode_Moving_away_from_borders(Motor *Motor_xx);
void Mode_Automatic(Motor *Motor_xx);

#endif /* INC_OPERATING_MODE_H_ */
