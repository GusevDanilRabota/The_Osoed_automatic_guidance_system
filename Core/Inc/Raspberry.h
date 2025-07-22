/*
 * Raspberry.h
 *
 *  Created on: Jul 16, 2025
 *      Author: u033008
 */

#ifndef INC_RASPBERRY_H_
#define INC_RASPBERRY_H_

//#include "stm32f4xx_hal.h"
#include "System_status.h"
#include "math.h"

typedef struct
{
  UART_HandleTypeDef *UART_port;

  unsigned char transmitting;

  unsigned char Rx_data[21];
  unsigned char Tx_data[13];

  float Azimuth;
  float Elevation;
} Raspberry;

extern Raspberry RP_message;

void Raspberry_DatPpars(unsigned char Rx_data[21]);
void Raspberry_CreatMes(void);

#endif /* INC_RASPBERRY_H_ */
