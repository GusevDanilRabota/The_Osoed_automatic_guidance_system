/*
 * Operating_mode.c
 *
 *  Created on: Jul 19, 2025
 *      Author: u033008
 */

#include "Operating_mode.h"

Mode_Automatic_data Automatic_data_AZ = {.Requirement_stop = 'N'};
Mode_Automatic_data Automatic_data_EL = {.Requirement_stop = 'N'};

void Mode_Manual(Motor *Motor_xx) {
  if (Motor_xx == &Motor_AZ ? RP_message.Azimuth : RP_message.Elevation) {
    if (Motor_xx->Status.Functioning) {
      Motor_xx->Status.Tik[0] = TIM5->CNT;
      if (abs(Motor_xx->Status.Tik[0] - Motor_xx->Status.Tik[1]) / TIM5->PSC > 2000) {
    	Motor_xx->Status.Tik[1] = Motor_xx->Status.Tik[0];
        Motor_UpFrequency(Motor_xx);
      }
    } else {
      Motor_DirRot(Motor_xx, (Motor_xx == &Motor_AZ ? RP_message.Azimuth : RP_message.Elevation) > 0 ? Hight : Low);
      Motor_Start(Motor_xx);
    };
  } else {Motor_Stop(Motor_xx);};
};

void Mode_Semi_automatic(Motor *Motor_xx) {
  if (Motor_xx == &Motor_AZ ? abs(RP_message.Azimuth - Systema_AZ.Actual_angular) != 0 : abs(RP_message.Elevation - Systema_EL.Actual_angular) != 0) {
    if (Motor_xx->Status.Functioning) {
      Motor_xx->Status.Tik[0] = TIM5->CNT;
      if (abs(Motor_xx->Status.Tik[0] - Motor_xx->Status.Tik[1]) / TIM5->PSC > 2000) {
    	Motor_xx->Status.Tik[1] = Motor_xx->Status.Tik[0];
        Motor_UpFrequency(Motor_xx);
      }
    } else {
      Motor_DirRot(Motor_xx, (Motor_xx == &Motor_AZ ? RP_message.Azimuth - Systema_AZ.Actual_angular : RP_message.Elevation - Systema_EL.Actual_angular) > 0 ? Hight : Low);
      Motor_Start(Motor_xx);
    };
  } else {Motor_Stop(Motor_xx);};
};

void Mode_Moving_away_from_borders(Motor *Motor_xx) {
  Systems *Systema_xx = Motor_xx == &Motor_AZ ? &Systema_AZ : &Systema_EL;

  if (System_CheckLimit(Systema_xx)) {
	Motor_xx->Status.Tik[0] = TIM5->CNT;
    if (abs(Motor_xx->Status.Tik[0] - Motor_xx->Status.Tik[1]) > 20000) {
      Motor_xx->Status.Tik[1] = Motor_xx->Status.Tik[0];
      Motor_Stop(Motor_xx);
    }
  } else {
    Motor_DirRot(Motor_xx, Systema_xx->Actual_angular > 0 ? Hight : Low);
    Motor_Start(Motor_xx);
  };
};

void Mode_Automatic(Motor *Motor_xx) {
  Mode_Automatic_data *Automatic_data_xx = Motor_xx == &Motor_AZ ? &Automatic_data_AZ : &Automatic_data_EL;

  if (Motor_xx->Status.Functioning == 0) {  // && Приказ от Raspberry на остановку == 'N'
    Motor_xx->Status.Direction_of_rotation = (Motor_xx == &Motor_AZ ? (RP_message.Azimuth > 0 ? Low : Hight) : (RP_message.Elevation > 0 ? Hight : Low)) ;
    Motor_DirRot(Motor_xx, Motor_xx->Status.Direction_of_rotation);
    Motor_Start(Motor_xx);
  } else {
	if (0) { // && Приказ от Raspberry на остановку == 'Y'
	  Motor_Stop(Motor_xx);
	} else {
	  unsigned int Frequensy;

	  Automatic_data_xx->Buffer_differens_angular[1] = Automatic_data_xx->Buffer_differens_angular[0];
	  Automatic_data_xx->Buffer_differens_angular[0] = Motor_xx == &Motor_AZ ? RP_message.Azimuth : RP_message.Elevation;

	  unsigned int Increment_frequensy =
	    100 * sqrt(Motor_xx == &Motor_AZ ? RP_message.Azimuth : RP_message.Elevation) +
		140 * (Automatic_data_xx->Buffer_differens_angular[0] - Automatic_data_xx->Buffer_differens_angular[1]);

	  if (Motor_xx == &Motor_AZ ? RP_message.Azimuth : RP_message.Elevation) {
	    if (Motor_xx->Status.Direction_of_rotation) {
	      if (Motor_xx->Status.Frequency - Increment_frequensy < Motor_xx->Parameters.Frequency.Minimum) {
	        Motor_Stop(Motor_xx);
	        Motor_xx->Status.Direction_of_rotation = Low;
	        Motor_DirRot(Motor_xx, Motor_xx->Status.Direction_of_rotation);
	        Motor_Start(Motor_xx);
	      } else {
	    	Frequensy = Motor_xx->Status.Frequency - Increment_frequensy;
	    	Motor_xx->Status.Tik[0] = TIM5->CNT;
	    	Motor_xx->Status.Tik[1] = Motor_xx->Status.Tik[0];
	    	Motor_SetFrequency(Motor_xx, Frequensy);
	    	HAL_TIM_PWM_Start(Motor_xx->Parameters.Timer.Number_timer, Motor_xx->Parameters.Timer.Channel);
	      }
	    } else {
	      Frequensy = Motor_xx->Status.Frequency + Increment_frequensy;
	      Motor_xx->Status.Tik[0] = TIM5->CNT;
	      if (abs(Motor_xx->Status.Tik[0] - Motor_xx->Status.Tik[1]) / TIM5->PSC > 5000) {
	        Motor_xx->Status.Tik[1] = Motor_xx->Status.Tik[0];
	        Motor_SetFrequency(Motor_xx, Frequensy);
	        HAL_TIM_PWM_Start(Motor_xx->Parameters.Timer.Number_timer, Motor_xx->Parameters.Timer.Channel);
	      };
	    };
	  } else {
	    if (Motor_xx->Status.Direction_of_rotation) {
	      Frequensy = Motor_xx->Status.Frequency + Increment_frequensy;
	      Motor_xx->Status.Tik[0] = TIM5->CNT;
	      if (abs(Motor_xx->Status.Tik[0] - Motor_xx->Status.Tik[1]) / TIM5->PSC > 5000) {
	        Motor_xx->Status.Tik[1] = Motor_xx->Status.Tik[0];
	        Motor_SetFrequency(Motor_xx, Frequensy);
	        HAL_TIM_PWM_Start(Motor_xx->Parameters.Timer.Number_timer, Motor_xx->Parameters.Timer.Channel);
	      };
	    } else {
	      if (Motor_xx->Status.Frequency - Increment_frequensy < Motor_xx->Parameters.Frequency.Minimum) {
	        Motor_Stop(Motor_xx);
	        Motor_xx->Status.Direction_of_rotation = Hight;
	        Motor_DirRot(Motor_xx, Motor_xx->Status.Direction_of_rotation);
	        Motor_Start(Motor_xx);
	      } else {
	        Frequensy = Motor_xx->Status.Frequency - Increment_frequensy;
	        Motor_xx->Status.Tik[0] = TIM5->CNT;
	        Motor_xx->Status.Tik[1] = Motor_xx->Status.Tik[0];
	        Motor_SetFrequency(Motor_xx, Frequensy);
	        HAL_TIM_PWM_Start(Motor_xx->Parameters.Timer.Number_timer, Motor_xx->Parameters.Timer.Channel);
	      }
	    };
	  };
	};
  };
};
