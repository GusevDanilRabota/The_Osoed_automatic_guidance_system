/*
 * System_status.c
 *
 *  Created on: Jul 15, 2025
 *      Author: u033008
 */

#include "System_status.h"

Systems Systema_AZ = {
  .Config_angular = {
    .Maximum = 270.0f,
    .Minimum = -270.0f
  }
};
Systems Systema_EL = {
  .Config_angular = {
    .Maximum = 90.0f,
    .Minimum = -20.0f
  }
};

char System_CheckLimit(Systems *Systema_xx, float Incremtnt_limit) {
  float lower_limit = Systema_xx->Config_angular.Minimum + Incremtnt_limit;
  float upper_limit = Systema_xx->Config_angular.Maximum - Incremtnt_limit;
  float current_angle = CLAMP(Systema_xx->Actual_angular, lower_limit, upper_limit);
  return ((current_angle != lower_limit) && (current_angle != upper_limit)) ? 1 : 0;
};
