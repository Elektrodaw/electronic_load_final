/*
 * SafetyCheck.h
 *
 *  Created on: Mar 31, 2026
 *      Author: ddaw7
 */

#ifndef INC_SAFETYCHECK_H_
#define INC_SAFETYCHECK_H_

#include "stm32f4xx_hal.h"
#include <stdint.h>

extern ADC_HandleTypeDef hadc1;
extern uint8_t loadStatus;

void overheat();


#endif /* INC_SAFETYCHECK_H_ */
