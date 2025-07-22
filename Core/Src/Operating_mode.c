/*
 * Operating_mode.c
 *
 *  Created on: Jul 19, 2025
 *      Author: u033008
 */

#include "Operating_mode.h"

Mode_Automatic_data Automatic_data_AZ = {
  .Buffer_differens_angular = {0},
  .Index_buffer = 0,
  .Requirement_stop = 'N'
};
Mode_Automatic_data Automatic_data_EL = {
  .Buffer_differens_angular = {0},
  .Index_buffer = 0,
  .Requirement_stop = 'N'
};

void Mode_Manual(Motor *Motor_xx) {
  if (Motor_xx == &Motor_AZ ? RP_message.Azimuth : RP_message.Elevation) {
    if (Motor_xx->State.Functioning) {
      Motor_xx->State.New_value_tik = TIM5->CNT;
      if (abs(Motor_xx->State.New_value_tik - Motor_xx->State.Old_value_tik) / TIM5->PSC > 2000) {
    	Motor_xx->State.Old_value_tik = Motor_xx->State.New_value_tik;
        Motor_UpFrequency(Motor_xx);}
    } else {
      Motor_DirRot(Motor_xx, (Motor_xx == &Motor_AZ ? RP_message.Azimuth : RP_message.Elevation) > 0 ? GPIO_PIN_SET : GPIO_PIN_RESET);
      Motor_Start(Motor_xx);
    };
  } else {Motor_Stop(Motor_xx);};
};

void Mode_Semi_automatic(Motor *Motor_xx) {
  if (Motor_xx == &Motor_AZ ? abs(RP_message.Azimuth - Systema_AZ.Actual_angular) != 0 : abs(RP_message.Elevation - Systema_EL.Actual_angular) != 0) {
    if (Motor_xx->State.Functioning) {
      Motor_xx->State.New_value_tik = TIM5->CNT;
      if (abs(Motor_xx->State.New_value_tik - Motor_xx->State.Old_value_tik) / TIM5->PSC > 2000) {
    	Motor_xx->State.Old_value_tik = Motor_xx->State.New_value_tik;
        Motor_UpFrequency(Motor_xx);}
    } else {
      Motor_DirRot(Motor_xx, (Motor_xx == &Motor_AZ ? RP_message.Azimuth - Systema_AZ.Actual_angular : RP_message.Elevation - Systema_EL.Actual_angular) > 0 ? GPIO_PIN_SET : GPIO_PIN_RESET);
      Motor_Start(Motor_xx);
    };
  } else {Motor_Stop(Motor_xx);};
};

void Mode_Moving_away_from_borders(Motor *Motor_xx, float Incremtnt_limit) {
  Encoder *Encoder_xx = Motor_xx == &Motor_AZ ? &Encoder_AZ : &Encoder_EL;
  Systems *Systema_xx = Motor_xx == &Motor_AZ ? &Systema_AZ : &Systema_EL;
  Motor_DirRot(Motor_xx, Systema_xx->Actual_angular > 0 ? GPIO_PIN_RESET : GPIO_PIN_SET);
  Motor_Start(Motor_xx);
  do {Encoder_GetAngular(Encoder_xx);} while (!System_CheckLimit(Systema_xx, Incremtnt_limit));
  Motor_Stop(Motor_xx);
};

void Mode_Automatic(Motor *Motor_xx) {
  Mode_Automatic_data *Automatic_data_xx = Motor_xx == &Motor_AZ ? &Automatic_data_AZ : &Automatic_data_EL;

  if (Motor_xx->State.Functioning == 0 && Automatic_data_xx->Requirement_stop == 'N') {
    Motor_DirRot(Motor_xx, (Motor_xx == &Motor_AZ ? RP_message.Azimuth : RP_message.Elevation) > 0 ? GPIO_PIN_SET : GPIO_PIN_RESET);
    Motor_Start(Motor_xx);
  } else {
	if (Automatic_data_xx->Requirement_stop == 'Y') {
	  Motor_Stop(Motor_xx);
	}
	else {
	  unsigned int Frequency_range =
	    Motor_xx == &Motor_AZ ?
	    (Motor_AZ.Setting.Frequency.Maximum_pulse) - (Motor_AZ.Setting.Frequency.Minimum_pulse) - 500:
		(Motor_EL.Setting.Frequency.Maximum_pulse) - (Motor_EL.Setting.Frequency.Minimum_pulse) - 500;
	  float Error_angular_range = Motor_xx == &Motor_AZ ? 60.0f : 20.0f;

	  Automatic_data_xx->Buffer_differens_angular[1] = Automatic_data_xx->Buffer_differens_angular[0];
	  Automatic_data_xx->Buffer_differens_angular[0] = Motor_xx == &Motor_AZ ? RP_message.Azimuth : RP_message.Elevation;

	  Automatic_data_xx->Buffer_speed[1] = Automatic_data_xx->Buffer_speed[0];
	  Automatic_data_xx->Buffer_speed[0] = (Automatic_data_xx->Buffer_differens_angular[0] - Automatic_data_xx->Buffer_differens_angular[1]) / 0.05;

	  float Increment_speed = Automatic_data_xx->Buffer_speed[0] - Automatic_data_xx->Buffer_speed[1];
	  unsigned int Increment_frequensy = (int)Increment_speed * Frequency_range / Error_angular_range;
//	  unsigned int Increment_frequensy =
//	    100 * sqrt(Motor_xx == &Motor_AZ ? RP_message.Azimuth : RP_message.Elevation) +
//		140 * (Automatic_data_xx->Buffer_differens_angular[0] - Automatic_data_xx->Buffer_differens_angular[1]);
	  unsigned int Frequensy = Motor_xx->State.Frequency + Increment_frequensy;


	  Motor_xx->State.New_value_tik = TIM5->CNT;
	  if (abs(Motor_xx->State.New_value_tik - Motor_xx->State.Old_value_tik) / TIM5->PSC > 5000) {
	    Motor_xx->State.Old_value_tik = Motor_xx->State.New_value_tik;
	    Motor_SetFrequency(Motor_xx, Frequensy);
	  };
	};
  };
};





















