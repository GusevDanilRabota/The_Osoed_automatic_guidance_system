/*
 * Encoder.c
 *
 *  Created on: Jul 15, 2025
 *      Author: u033008
 */

#include "Encoder.h"

Encoder Encoder_AZ = {
  .Port_A = {
    .Port = GPIOC,
	.Pin = GPIO_PIN_7
  },
  .Port_B = {
    .Port = GPIOC,
	.Pin = GPIO_PIN_6
  },
  .Puls = {
    .Pulses_per_revolution = 3000,
  }
};

Encoder Encoder_EL = {
  .Port_A = {
    .Port = GPIOB,
    .Pin = GPIO_PIN_7
  },
  .Port_B = {
    .Port = GPIOB,
    .Pin = GPIO_PIN_6
  },
  .Puls = {
    .Pulses_per_revolution = 611,
  }
};

/**
  * @brief Функция инициализации энкодера
  * @param Encoder_xx Указатель на структуру энкодера, содержащую:
  *                   - Timer: указатель на структуру таймера, используемого для энкодера
  *                   (с доступом к регистру счетчика CNT)
  * @retval None
  * @note Функция выполняет следующие операции:
  *       1. Ожидает стабилизации показаний энкодера в течение минимум 1 секунды
  *       2. Проверяет, что значение счетчика не изменяется (движение отсутствует)
  *       3. Сбрасывает счетчик энкодера в 0 после стабилизации
  *
  * @warning Перед вызовом этой функции необходимо:
  *          - Убедиться, что двигатели уже запущены
  *          - Обеспечить движение системы в сторону отрицательных углов
  *          (это важно для правильной последующей калибровки)
  *
  * @details Алгоритм работы:
  *          - В цикле do-while сравниваются текущее и предыдущее значения счетчика
  *          - Задержка 500 мс между измерениями гарантирует, что система неподвижна
  *          - Цикл продолжается до тех пор, пока два последовательных измерения
  *            не дадут одинаковый результат
  *          - После стабилизации счетчик сбрасывается в нулевое положение
  *
  * @attention Функция содержит блокирующую задержку (HAL_Delay)
  * @attention Время инициализации зависит от времени стабилизации системы
  *
  * @example Пример вызова:
  *          Encoder encoder_AZ;
  *          Encoder_Init(&encoder_AZ); // Инициализация энкодера азимута
  */
void Encoder_Init(Encoder *Encoder_xx) { // Запуск моторов должен быть обязательно до начала инициализации Encoder. Движение в сторону минусовых углов
  unsigned int New_value = 0;
  unsigned int Old_value = 0;

  Motor *Motor_xx = (Encoder_xx == &Encoder_AZ) ? &Motor_AZ : &Motor_EL;

  HAL_GPIO_WritePin(GPIOD, GPIO_PIN_15, GPIO_PIN_SET); // Светодиодная индикация
  Motor_DirRot(Motor_xx, GPIO_PIN_RESET);
  Motor_Start(Motor_xx);

  do {
    Old_value = New_value;
    HAL_Delay(1000);
    New_value = Encoder_xx->Timer->Instance->CNT;
  } while (New_value != Old_value);

  Encoder_xx->Timer->Instance->CNT = 0;

  Motor_Stop(Motor_xx);
  HAL_GPIO_WritePin(GPIOD, GPIO_PIN_15, GPIO_PIN_RESET);
};


void Encoder_GetAngular(Encoder *Encoder_xx) {
  unsigned short Encoder_value = Encoder_xx->Timer->Instance->CNT / 4;
  (Encoder_xx == &Encoder_AZ) ?
		  (Systema_AZ.Actual_angular =
				  Systema_AZ.Config_angular.Minimum +
				  (float)Encoder_value *
				  (Systema_AZ.Config_angular.Maximum - Systema_AZ.Config_angular.Minimum) /
				  Encoder_xx->Puls.Pulses_per_revolution) :
		  (Systema_EL.Actual_angular =
				  Systema_EL.Config_angular.Minimum +
				  (float)Encoder_value *
				  (Systema_EL.Config_angular.Maximum - Systema_EL.Config_angular.Minimum) /
				  Encoder_xx->Puls.Pulses_per_revolution);
};
