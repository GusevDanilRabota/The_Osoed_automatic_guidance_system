/*
 * Stepper_Motor_Driver.c
 *
 *  Created on: Jul 14, 2025
 *      Author: Danil
 */

#include "Stepper_Motor_Driver.h"

Motor Motor_AZ = {
  .Setting = {
    .Pin = {
      .ENA = {.Port = GPIOE, .Pin = GPIO_PIN_11},
	  .PUL = {.Port = GPIOA, .Pin = GPIO_PIN_6},
	  .DIR = {.Port = GPIOE, .Pin = GPIO_PIN_8}
    },
	.Frequency = {
      .Maximum_pulse = 200000,
	  .Minimum_pulse = 30000,
	  .Step_up_the_pulse = 10000
	}
  },
  .State = {
    .Frequency = 0,
	.Functioning = 0
  }
};

Motor Motor_EL = {
  .Setting = {
    .Pin = {
      .ENA = {.Port = GPIOE, .Pin = GPIO_PIN_12},
	  .PUL = {.Port = GPIOA, .Pin = GPIO_PIN_5},
	  .DIR = {.Port = GPIOE, .Pin = GPIO_PIN_9}
    },
	.Frequency = {
      .Maximum_pulse = 200000,
	  .Minimum_pulse = 30000,
	  .Step_up_the_pulse = 5000
	}
  },
  .State = {
    .Frequency = 0,
	.Functioning = 0
  }
};

void Pin_SetLevl(Bundle_Port_Pin *Pin, int State) {
//  HAL_GPIO_WritePin(Pin->Port, Pin->Pin, State);

  assert_param(IS_GPIO_PIN(Pin->Pin));
  assert_param(IS_GPIO_PIN_ACTION(State));

  if(State != GPIO_PIN_RESET) {Pin->Port->BSRR = Pin->Pin;}
  else {Pin->Port->BSRR = (uint32_t)Pin->Pin << 16U;};
};

void Motor_DirRot(Motor *Motor_xx, GPIO_PinState Direction_of_rotation) {
//  HAL_GPIO_WritePin(Motor_xx->Setting.Pin.DIR.Port, Motor_xx->Setting.Pin.DIR.Pin, Direction_of_rotation);

  assert_param(IS_GPIO_PIN(Motor_xx->Setting.Pin.DIR.Pin));
  assert_param(IS_GPIO_PIN_ACTION(Direction_of_rotation));

  if(Direction_of_rotation != GPIO_PIN_RESET) {Motor_xx->Setting.Pin.DIR.Port->BSRR = Motor_xx->Setting.Pin.DIR.Pin;}
  else {Motor_xx->Setting.Pin.DIR.Port->BSRR = (uint32_t)Motor_xx->Setting.Pin.DIR.Pin << 16U;};
};

void Motor_SetFrequency(Motor *Motor_xx, unsigned int frequency) {
  TIM_HandleTypeDef *Number_xx = Motor_xx->Setting.Timer.Number;

  static unsigned int prescaler = 0;
//  unsigned int period = (HAL_RCC_GetPCLK1Freq() * 2 / (frequency * (prescaler + 1))) - 1;
  unsigned int period = ((SystemCoreClock >> APBPrescTable[(RCC->CFGR & RCC_CFGR_PPRE1) >> RCC_CFGR_PPRE1_Pos]) * 2 / (frequency * (prescaler + 1))) - 1;

  while (period > 0xFFFF) {
    prescaler++;
    period = ((SystemCoreClock >> APBPrescTable[(RCC->CFGR & RCC_CFGR_PPRE1) >> RCC_CFGR_PPRE1_Pos]) * 2 / (frequency * (prescaler + 1))) - 1;
  };

  HAL_TIM_PWM_Stop(Number_xx, Motor_xx->Setting.Timer.Channel);
  Number_xx->Instance->PSC = prescaler;
  Number_xx->Instance->ARR = period;
  Number_xx->Instance->CCR1 = Number_xx->Instance->ARR / 2;

  Motor_xx->State.Frequency = frequency;
};

void Motor_UpFrequency(Motor *Motor_xx) {
  if (Motor_xx->State.Frequency < Motor_xx->Setting.Frequency.Maximum_pulse) {
    Motor_SetFrequency(
	  Motor_xx,
	  Motor_xx->State.Frequency + Motor_xx->Setting.Frequency.Step_up_the_pulse < Motor_xx->Setting.Frequency.Maximum_pulse ?
	    Motor_xx->State.Frequency + Motor_xx->Setting.Frequency.Step_up_the_pulse :
	    Motor_xx->Setting.Frequency.Maximum_pulse
	);
    HAL_TIM_PWM_Start(Motor_xx->Setting.Timer.Number, Motor_xx->Setting.Timer.Channel);
  };
};

void Motor_Start(Motor *Motor_xx) {
  Settings *Setting_xx = &Motor_xx->Setting;

  Motor_SetFrequency(Motor_xx, Setting_xx->Frequency.Minimum_pulse);
  HAL_TIM_PWM_Start(Setting_xx->Timer.Number, Setting_xx->Timer.Channel);

//  TIM_HandleTypeDef *Number_xx = Motor_xx->Setting.Timer.Number;
//  unsigned int Channel = Motor_xx->Setting.Timer.Channel;
//  uint32_t tmpsmcr;
//  assert_param(IS_TIM_CCX_INSTANCE(Number_xx->Instance, Channel)); /* Check the parameters */
//  if (TIM_CHANNEL_STATE_GET(Number_xx, Channel) != HAL_TIM_CHANNEL_STATE_READY) return /* Check the TIM channel state */
//  TIM_CHANNEL_STATE_SET(Number_xx, Channel, HAL_TIM_CHANNEL_STATE_BUSY); /* Set the TIM channel state */
//  TIM_CCxChannelCmd(Number_xx->Instance, Channel, TIM_CCx_ENABLE); /* Enable the Capture compare channel */
//  if (IS_TIM_BREAK_INSTANCE(Number_xx->Instance) != RESET) __HAL_TIM_MOE_ENABLE(Number_xx); /* Enable the main output */
//  if (IS_TIM_SLAVE_INSTANCE(Number_xx->Instance)) { /* Enable the Peripheral, except in trigger mode where enable is automatically done with trigger */
//    tmpsmcr = Number_xx->Instance->SMCR & TIM_SMCR_SMS;
//    if (!IS_TIM_SLAVEMODE_TRIGGER_ENABLED(tmpsmcr)) __HAL_TIM_ENABLE(Number_xx);
//  }
//  else __HAL_TIM_ENABLE(Number_xx);

  Pin_SetLevl(&Setting_xx->Pin.ENA, Low);
  Motor_xx->State.Functioning = Hight;
};

//HAL_StatusTypeDef HAL_TIM_PWM_Stop(TIM_HandleTypeDef *htim, uint32_t Channel)
//{
//  assert_param(IS_TIM_CCX_INSTANCE(htim->Instance, Channel)); /* Check the parameters */
//  TIM_CCxChannelCmd(htim->Instance, Channel, TIM_CCx_DISABLE); /* Disable the Capture compare channel */
//  if (IS_TIM_BREAK_INSTANCE(htim->Instance) != RESET) __HAL_TIM_MOE_DISABLE(htim); /* Disable the Main Output */
//  __HAL_TIM_DISABLE(htim); /* Disable the Peripheral */
//  TIM_CHANNEL_STATE_SET(htim, Channel, HAL_TIM_CHANNEL_STATE_READY); /* Set the TIM channel state */
//}

void Motor_Stop(Motor *Motor_xx) {
  Settings *Setting_xx = &Motor_xx->Setting;

  Pin_SetLevl(&Setting_xx->Pin.ENA, Hight);
  Motor_SetFrequency(Motor_xx, Setting_xx->Frequency.Minimum_pulse);
  HAL_TIM_PWM_Stop(Setting_xx->Timer.Number, Setting_xx->Timer.Channel);

//  assert_param(IS_TIM_CCX_INSTANCE(Setting_xx->Timer.Number->Instance, Setting_xx->Timer.Channel)); /* Check the parameters */
//  TIM_CCxChannelCmd(Setting_xx->Timer.Number->Instance, Setting_xx->Timer.Channel, TIM_CCx_DISABLE); /* Disable the Capture compare channel */
//  if (IS_TIM_BREAK_INSTANCE(Setting_xx->Timer.Number) != RESET)
//	  __HAL_TIM_MOE_DISABLE(Setting_xx->Timer.Number); /* Disable the Main Output */
//  __HAL_TIM_DISABLE(Setting_xx->Timer.Number); /* Disable the Peripheral */
//  TIM_CHANNEL_STATE_SET(Setting_xx->Timer.Number, Setting_xx->Timer.Channel, HAL_TIM_CHANNEL_STATE_READY); /* Set the TIM channel state */

  Motor_xx->State.Functioning = Low;
};
