/*
 * Macros.h
 *
 *  Created on: Jul 15, 2025
 *      Author: u033008
 */

#ifndef INC_MACROS_H_
#define INC_MACROS_H_

#define MIN(a, b) ((a) < (b) ? (a) : (b))
#define MAX(a, b) ((a) > (b) ? (a) : (b))

#define CLAMP(x, min, max) MAX(min, MIN(x, max))

#endif /* INC_MACROS_H_ */
