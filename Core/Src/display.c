/*
 * didplay.c
 *
 *  Created on: Jan 6, 2026
 *      Author: ddaw7
 */
#include "display.h"
#include "main.h"
#include "nextion.h"

void display()
{
	HAL_GPIO_WritePin(FREE4_GPIO_Port, FREE4_Pin, GPIO_PIN_SET);

	if(actualMode == 1){
		NextionSetText(&nextion, &ModeName, "CC mode");
		NextionSetText(&nextion, &SetValueUnit, "A");
	}

	if(actualMode == 2){
		NextionSetText(&nextion, &ModeName, "CV mode");
		NextionSetText(&nextion, &SetValueUnit, "V");
	}

	if(actualMode == 3){
		NextionSetText(&nextion, &ModeName, "CR mode");
		NextionSetText(&nextion, &SetValueUnit, "Ohm");
	}

	if(actualMode == 4){
		NextionSetText(&nextion, &ModeName, "CP mode");
		NextionSetText(&nextion, &SetValueUnit, "W");
	}

	if(actualMode == 7){
		NextionSetVal(&nextion, &NewValueDisp, NewValue*100);

		if(previousMode == 1){
			NextionSetText(&nextion, &NewValueUnit, "A");
		}else if(previousMode == 2){
			NextionSetText(&nextion, &NewValueUnit, "V");
		}else if(previousMode == 3){
			NextionSetText(&nextion, &NewValueUnit, "Ohm");
		}else if(previousMode == 4){
			NextionSetText(&nextion, &NewValueUnit, "W");
		}
	}




	NextionSetVal(&nextion, &SetValue, SetValueGlobal*100);

	NextionSetVal(&nextion, &Current, current_mes*100);
	NextionSetVal(&nextion, &Voltage, voltage_mes*100);
	NextionSetVal(&nextion, &Power, power_mes*100);

	if(loadStatus == 0){
		NextionSetText(&nextion, &LoadSW, "Load OFF");

		NextionSetButtonColor(&nextion, &LoadSW, "RED");

	}else if(loadStatus == 1){
		NextionSetText(&nextion, &LoadSW, "Load ON");

		NextionSetButtonColor(&nextion, &LoadSW, "GREEN");

	}

	if(voltageSrcState == 0){
		NextionSetText(&nextion, &VoltageSrc, "V INT");

	}else if(voltageSrcState == 1){
		NextionSetText(&nextion, &VoltageSrc, "V EXT");

	}

	TransmitBuffer(&nextion);

	HAL_GPIO_WritePin(FREE4_GPIO_Port, FREE4_Pin, GPIO_PIN_RESET);
}

