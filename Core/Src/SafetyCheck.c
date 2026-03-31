/*
 * SafetyCheck.c
 *
 *  Created on: Mar 31, 2026
 *      Author: ddaw7
 */

#include "SafetyCheck.h"
#include "math.h"


uint32_t adc_value[4];
float adc_voltage[4];
float thermistor_resistance[4];
float temperature[4];


void overheat(void) {

    HAL_ADC_Start(&hadc1);

    for(int i = 0; i < 4; i++) {
        if(HAL_ADC_PollForConversion(&hadc1, HAL_MAX_DELAY) == HAL_OK) {
            adc_value[i] = HAL_ADC_GetValue(&hadc1);
        }
    }

    HAL_ADC_Stop(&hadc1);

    for(int i = 0; i < 4; i++) {
            adc_voltage[i] = (3.3/4095)*adc_value[i];
            thermistor_resistance[i] = (10*adc_voltage[i]/(3.3-adc_voltage[i]));
        }

	const float R0 = 10;
	const float B  = 3950;
	const float T0 = 298.15;

	for(int i = 0; i < 4; i++) {
		temperature[i] = (1.0f / ((1.0f/T0) + (1.0f/B)*log(thermistor_resistance[i]/R0))) - 273.15;
	}

    if(temperature[0] > 80){
    	loadStatus = 0;
    }
}
