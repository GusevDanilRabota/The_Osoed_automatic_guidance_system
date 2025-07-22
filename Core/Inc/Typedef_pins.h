/*
 * Typedef_pins.h
 *
 *  Created on: Jul 22, 2025
 *      Author: u033008
 */

#ifndef INC_TYPEDEF_PINS_H_
#define INC_TYPEDEF_PINS_H_

#include "stm32f4xx_hal.h"

typedef enum
{
  Low = 0,
  Hight
} Levl_signal;

typedef struct {
  GPIO_TypeDef* Port;
  unsigned short Pin;
} Bundle_Port_Pin;

#endif /* INC_TYPEDEF_PINS_H_ */
