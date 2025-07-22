/*
 * Raspberry.c
 *
 *  Created on: Jul 16, 2025
 *      Author: u033008
 */

#include "Raspberry.h"

Raspberry RP_message =
{
  .Azimuth = 0,
  .Elevation = 0,
  .Rx_data = "Az+0000El-0000M0F0\r\n\0",
  .Tx_data = "T0000000000\r\n",
  .transmitting = 0
};

void Raspberry_DatPpars(void) {
  RP_message.Rx_data[20] = '\0';

  RP_message.Azimuth =
    (RP_message.Rx_data[3]  - '0') * 100.0f +
    (RP_message.Rx_data[4]  - '0') * 10.0f +
    (RP_message.Rx_data[5]  - '0') +
    (RP_message.Rx_data[6]  - '0') * 0.1f;
  if (RP_message.Rx_data[2] == '-') RP_message.Azimuth = -RP_message.Azimuth;

  RP_message.Elevation =
    (RP_message.Rx_data[10] - '0') * 100.0f +
    (RP_message.Rx_data[11] - '0') * 10.0f +
    (RP_message.Rx_data[12] - '0') +
    (RP_message.Rx_data[13] - '0') * 0.1f;
  if (RP_message.Rx_data[9] == '-') RP_message.Elevation = -RP_message.Elevation;
};

//void Raspberry_DatPpars(unsigned char *Rx_data[21]) {
//  RP_message.Azimuth =
//    (Rx_data[3]  - '0') * 100.0f +
//    (Rx_data[4]  - '0') * 10.0f +
//    (Rx_data[5]  - '0') +
//    (Rx_data[6]  - '0') * 0.1f;
//  if (Rx_data[2] == '-') RP_message.Azimuth = -RP_message.Azimuth;
//
//  RP_message.Elevation =
//    (Rx_data[10] - '0') * 100.0f +
//    (Rx_data[11] - '0') * 10.0f +
//    (Rx_data[12] - '0') +
//    (Rx_data[13] - '0') * 0.1f;
//  if (Rx_data[9] == '-') RP_message.Elevation = -RP_message.Elevation;
//};

void Raspberry_CreatMes(void) {
  if (!RP_message.transmitting) {
    uint16_t angular_az = (uint16_t)fabs(Systema_AZ.Actual_angular * 10);
    uint16_t angular_el = (uint16_t)fabs(Systema_EL.Actual_angular * 10);

    RP_message.Tx_data[1] = (Systema_AZ.Actual_angular >= 0) ? '1' : '0';
    RP_message.Tx_data[2] = '0' + (angular_az / 1000) % 10;
    RP_message.Tx_data[3] = '0' + (angular_az / 100) % 10;
    RP_message.Tx_data[4] = '0' + (angular_az / 10) % 10;
    RP_message.Tx_data[5] = '0' + (angular_az) % 10;

    RP_message.Tx_data[6] = (Systema_EL.Actual_angular >= 0) ? '1' : '0';
    RP_message.Tx_data[7] = '0' +  (angular_el / 1000) % 10;
    RP_message.Tx_data[8] = '0' +  (angular_el / 100) % 10;
    RP_message.Tx_data[9] = '0' +  (angular_el / 10) % 10;
    RP_message.Tx_data[10] = '0' + (angular_el) % 10;
  };
};
