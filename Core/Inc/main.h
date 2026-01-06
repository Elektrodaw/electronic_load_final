/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2026 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __MAIN_H
#define __MAIN_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32f4xx_hal.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "Nextion.h"
#include "INA229.h"
/* USER CODE END Includes */

/* Exported types ------------------------------------------------------------*/
/* USER CODE BEGIN ET */

/* USER CODE END ET */

/* Exported constants --------------------------------------------------------*/
/* USER CODE BEGIN EC */

/* USER CODE END EC */

/* Exported macro ------------------------------------------------------------*/
/* USER CODE BEGIN EM */

/* USER CODE END EM */

/* Exported functions prototypes ---------------------------------------------*/
void Error_Handler(void);

/* USER CODE BEGIN EFP */
extern SPI_HandleTypeDef hspi1;
extern DAC_HandleTypeDef hdac;

extern Nextion nextion;

//Objects for the components form main menu
extern NexComp ConstantCurrent;
extern NexComp ConstantVoltage;
extern NexComp ConstantResistance;
extern NexComp ConstantPower;
extern NexComp PVtest;
extern NexComp BatteryCapacity;

//Objects for the components from CC CV CR CP page
extern NexComp LoadSW;
extern NexComp VoltageSrc;
extern NexComp BackToMainMenu;
extern NexComp SetNewValue;
extern NexComp ModeName;
extern NexComp SetValue;
extern NexComp SetValueUnit;
extern NexComp Current;
extern NexComp Voltage;
extern NexComp Power;

//Objects for the components from set new value page
extern NexComp plus10;
extern NexComp plus1;
extern NexComp plus01;
extern NexComp plus001;
extern NexComp minus10;
extern NexComp minus1;
extern NexComp minus01;
extern NexComp minus001;
extern NexComp SaveValue;
extern NexComp NewValueDisp;
extern NexComp NewValueUnit;

//Objects for the components from PV page
extern NexComp PVchart;
extern NexComp PVstartTest;
extern NexComp PVback;
extern NexComp MPPpower;
extern NexComp MPPcurrent;
extern NexComp MPPvoltage;

extern uint8_t actualMode;
extern uint8_t previousMode;
extern uint8_t loadStatus;
extern uint8_t voltageSrcState;
extern float SetValueGlobal;
extern float NewValue;

extern float current_mes;
extern float voltage_mes;
extern float power_mes;



extern INA229_Handle ina;
/* USER CODE END EFP */

/* Private defines -----------------------------------------------------------*/
#define FREE4_Pin GPIO_PIN_2
#define FREE4_GPIO_Port GPIOC
#define INA_CS_Pin GPIO_PIN_4
#define INA_CS_GPIO_Port GPIOC
#define INA_ALERT_Pin GPIO_PIN_0
#define INA_ALERT_GPIO_Port GPIOB
#define IN_EN_Pin GPIO_PIN_12
#define IN_EN_GPIO_Port GPIOB
#define FREE3_Pin GPIO_PIN_13
#define FREE3_GPIO_Port GPIOB
#define LED_Pin GPIO_PIN_2
#define LED_GPIO_Port GPIOD
#define VoltageSrcControl_Pin GPIO_PIN_6
#define VoltageSrcControl_GPIO_Port GPIOB

/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */
