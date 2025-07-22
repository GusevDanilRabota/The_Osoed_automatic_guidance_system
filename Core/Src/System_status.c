/*
 * System_status.c
 *
 *  Created on: Jul 15, 2025
 *      Author: u033008
 */

#include "System_status.h"

Systems Systema_AZ = {
  .Maximum = 270.0f,
  .Minimum = -270.0f
};
Systems Systema_EL = {
  .Maximum = 90.0f,
  .Minimum = -20.0f
};

int System_CheckLimit(Systems *Systema_xx) {
  float lower_limit = Systema_xx->Minimum + 5.0f;
  float upper_limit = Systema_xx->Maximum - 5.0f;

  /*
   * Не удалять так как значение угла может выйти за границы
   * В этом случае условие равенства границам нарушится
   * */
  float current_angle = CLAMP(Systema_xx->Actual_angular, lower_limit, upper_limit);
  return
    (current_angle != lower_limit) && (current_angle != upper_limit) ?
      Hight:
	  Low;
};
