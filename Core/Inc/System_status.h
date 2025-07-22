/*
 * System_status.h
 *
 *  Created on: Jul 15, 2025
 *      Author: u033008
 */

#ifndef INC_SYSTEM_STATUS_H_
#define INC_SYSTEM_STATUS_H_

#include "Macros.h"
#include "stm32f4xx_hal.h"

typedef struct {
  float Maximum;
  float Minimum;
} Angulars;

typedef struct {
  Angulars Config_angular;
  float Actual_angular;
} Systems;

extern Systems Systema_AZ;
extern Systems Systema_EL;

char System_CheckLimit(Systems *Systema_xx, float Incremtnt_limit);

#endif /* INC_SYSTEM_STATUS_H_ */
