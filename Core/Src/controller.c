/*
 * controller.c
 *
 *  Created on: Jan 6, 2026
 *      Author: ddaw7
 */
#include "controller.h"
#include "main.h"

static uint16_t out_val = 0;
static float resistance_mes = 0;

static void CCmode()
{
	if(current_mes < SetValueGlobal){
		if(out_val < 4094){
			out_val++;
		}
	}else{
		if(out_val > 1){
			out_val--;
		}
	}

	if(loadStatus == 0){
		out_val = 0;
	}
	if(voltage_mes < 1){
		out_val = 0;
	}

}

static void CVmode()
{
	if(voltage_mes > SetValueGlobal){
		if(out_val < 4094){
			out_val++;
		}
	}else{
		if(out_val > 1){
			out_val--;
		}
	}

	if(loadStatus == 0){
		out_val = 0;
	}
	if(voltage_mes < 1){
		out_val = 0;
	}
}

static void CRmode()
{
	resistance_mes = voltage_mes / current_mes;

	if(resistance_mes > SetValueGlobal){
		if(out_val < 4094){
			out_val++;
		}
	}else{
		if(out_val > 1){
			out_val--;
		}
	}

	if(loadStatus == 0){
		out_val = 0;
	}
	if(voltage_mes < 1){
		out_val = 0;
	}
}

static void CPmode()
{
	if(power_mes < SetValueGlobal){
		if(out_val < 4094){
			out_val++;
		}
	}else{
		if(out_val > 1){
			out_val--;
		}
	}

	if(loadStatus == 0){
		out_val = 0;
	}
	if(voltage_mes < 1){
		out_val = 0;
	}
}


void controller()
{
	HAL_GPIO_WritePin(FREE3_GPIO_Port, FREE3_Pin, GPIO_PIN_SET);
	voltage_mes = INA229_ReadBusVoltage(&ina);
	current_mes = INA229_ReadShuntVoltage(&ina)/ina.shunt_resistance;
	power_mes = voltage_mes * current_mes;

	if(current_mes < 0){
		current_mes = 0;
	}
	if(current_mes > 80){
		current_mes = 0;
	}
	if(power_mes > 5000){
		power_mes = 0;
	}

	if(actualMode == 0){
		out_val = 0;
		loadStatus = 0;
		voltageSrcState = 0;
		SetValueGlobal = 0;
		NewValue = 0;
	}

	if(actualMode == 1){
		CCmode();
	}

	if(actualMode == 2){
		CVmode();
	}

	if(actualMode == 3){
		CRmode();
	}

	if(actualMode == 4){
		CPmode();
	}

	if(actualMode == 7){
		if(previousMode == 1){
			CCmode();
		}else if(previousMode == 2){
			CVmode();
		}else if(previousMode == 3){
			CRmode();
		}else if(previousMode == 4){
			CPmode();
		}
	}

	HAL_DAC_SetValue(&hdac, DAC_CHANNEL_1, DAC_ALIGN_12B_R, out_val);
	HAL_GPIO_WritePin(FREE3_GPIO_Port, FREE3_Pin, GPIO_PIN_RESET);
}


