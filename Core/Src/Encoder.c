/*
 * Encoder.c
 *
 *  Created on: Jul 15, 2025
 *      Author: u033008
 */

#include "Encoder.h"

Encoder Encoder_AZ = {
  .Pulses_per_revolution = 3000,
  .Safe_start_value = 30
};

Encoder Encoder_EL = {
  .Pulses_per_revolution = 611,
  .Safe_start_value = 30
};

void Encoder_Init(Encoder *Encoder_xx) { // Запуск моторов должен быть обязательно до начала инициализации Encoder. Движение в сторону минусовых углов
  unsigned int New_value = 0;
  unsigned int Old_value = 0;
  Motor *Motor_xx = (Encoder_xx == &Encoder_AZ) ? &Motor_AZ : &Motor_EL;

  Motor_DirRot(Motor_xx, GPIO_PIN_RESET);
  Motor_Start(Motor_xx);
  do {
    Old_value = New_value;

    unsigned int tickstart = HAL_GetTick();
    unsigned int wait = 1000;
    if (wait < HAL_MAX_DELAY) wait += (uint32_t)(uwTickFreq); /* Add a freq to guarantee minimum wait */
    while((HAL_GetTick() - tickstart) < wait) {}

    New_value = Encoder_xx->Timer_number->Instance->CNT;
  } while (New_value != Old_value);

  Encoder_xx->Timer_number->Instance->CNT = Encoder_xx->Safe_start_value;

  Motor_Stop(Motor_xx);
};


void Encoder_GetAngular(Encoder *Encoder_xx) {
  unsigned short Encoder_value = Encoder_xx->Timer_number->Instance->CNT / 4;
  (Encoder_xx == &Encoder_AZ) ?
    (Systema_AZ.Actual_angular = Systema_AZ.Minimum + (float)(Encoder_value - Encoder_xx->Safe_start_value) * (Systema_AZ.Maximum - Systema_AZ.Minimum) / Encoder_xx->Pulses_per_revolution) :
    (Systema_EL.Actual_angular = Systema_EL.Minimum + (float)(Encoder_value - Encoder_xx->Safe_start_value) * (Systema_EL.Maximum - Systema_EL.Minimum) / Encoder_xx->Pulses_per_revolution);
};
