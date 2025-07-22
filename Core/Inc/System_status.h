/*
 * System_status.h
 *
 *  Created on: Jul 15, 2025
 *      Author: u033008
 */

#ifndef INC_SYSTEM_STATUS_H_
#define INC_SYSTEM_STATUS_H_

#include "Macros.h"
#include "Typedef_pins.h"

typedef struct {
  float Actual_angular;
  float Maximum;
  float Minimum;
} Systems;

extern Systems Systema_AZ;
extern Systems Systema_EL;

int System_CheckLimit(Systems *Systema_xx);

#endif /* INC_SYSTEM_STATUS_H_ */
