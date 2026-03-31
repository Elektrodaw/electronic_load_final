/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
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
/* Includes ------------------------------------------------------------------*/
#include "main.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include"nextion.h"
#include "controller.h"
#include "display.h"
#include "INA229.h"
#include "button_callbacks.h"
#include "SafetyCheck.h"
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
ADC_HandleTypeDef hadc1;

DAC_HandleTypeDef hdac;

SPI_HandleTypeDef hspi1;

TIM_HandleTypeDef htim2;

UART_HandleTypeDef huart4;
DMA_HandleTypeDef hdma_uart4_tx;

/* USER CODE BEGIN PV */
// Flags
uint8_t ControllerEN = 0;
uint8_t DisplayEN = 0;
uint8_t SaftyCheckEM = 0;
uint8_t SafetyCheck = 0;

//Global Variables
float SetValueGlobal = 0;
float NewValue = 0;
uint8_t actualMode = 0;
uint8_t previousMode = 0;
uint8_t loadStatus = 0;
uint8_t voltageSrcState = 0;
float current_mes = 0;
float voltage_mes = 0;
float power_mes = 0;

//privat variables
uint16_t tim_count = 0;


/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_DMA_Init(void);
static void MX_UART4_Init(void);
static void MX_SPI1_Init(void);
static void MX_DAC_Init(void);
static void MX_TIM2_Init(void);
static void MX_ADC1_Init(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */
//Object for the Nextion display
Nextion nextion;

//Objects for the components form main menu
NexComp ConstantCurrent;
NexComp ConstantVoltage;
NexComp ConstantResistance;
NexComp ConstantPower;
NexComp PVtest;
NexComp BatteryCapacity;

//Objects for the components from CC CV CR CP page
NexComp LoadSW;
NexComp VoltageSrc;
NexComp BackToMainMenu;
NexComp SetNewValue;
NexComp ModeName;
NexComp SetValue;
NexComp SetValueUnit;
NexComp Current;
NexComp Voltage;
NexComp Power;

//Objects for the components from set new value page
NexComp plus10;
NexComp plus1;
NexComp plus01;
NexComp plus001;
NexComp minus10;
NexComp minus1;
NexComp minus01;
NexComp minus001;
NexComp SaveValue;
NexComp NewValueDisp;
NexComp NewValueUnit;

//Objects for the components from PV page
NexComp PVchart;
NexComp PVstartTest;
NexComp PVback;
NexComp MPPpower;
NexComp MPPcurrent;
NexComp MPPvoltage;

//INA229 object
INA229_Handle ina;

//Nection UART RX interrupt
void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
	NextionUpdate(huart, &nextion);
}


//Interrupts callback
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
  if (htim == &htim2) {
	  ControllerEN = 1;
	  tim_count ++;
  }
  if(tim_count == 10){
	  SafetyCheck = 1;
  }
  if(tim_count == 100){
	  DisplayEN = 1;
	  tim_count = 0;
  }
}


/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{

  /* USER CODE BEGIN 1 */

  /* USER CODE END 1 */

  /* MCU Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_DMA_Init();
  MX_UART4_Init();
  MX_SPI1_Init();
  MX_DAC_Init();
  MX_TIM2_Init();
  MX_ADC1_Init();
  /* USER CODE BEGIN 2 */
  HAL_TIM_Base_Start_IT(&htim2);


  NextionInit(&nextion, &huart4);

  //main menu page components
	NextionAddComp(&nextion, &ConstantCurrent, "CC", 1, 2, buttonCallbackCC, NULL);
	NextionAddComp(&nextion, &ConstantVoltage, "CV", 1, 3, buttonCallbackCV, NULL);
	NextionAddComp(&nextion, &ConstantResistance, "CR", 1, 4, buttonCallbackCR, NULL);
	NextionAddComp(&nextion, &ConstantPower, "CP", 1, 1, buttonCallbackCP, NULL);


	//CC CV CR CP page components
	NextionAddComp(&nextion, &ModeName, "modeName", 2, 1, NULL, NULL);
	NextionAddComp(&nextion, &SetValue, "setValue", 2, 6, NULL, NULL);
	NextionAddComp(&nextion, &SetValueUnit, "setValueUnit", 2, 7, NULL, NULL);
	NextionAddComp(&nextion, &Voltage, "voltage", 2, 10, NULL, NULL);
	NextionAddComp(&nextion, &Current, "current", 2, 8, NULL, NULL);
	NextionAddComp(&nextion, &Power, "power", 2, 12, NULL, NULL);
	NextionAddComp(&nextion, &SetNewValue, "setNewSW", 2, 4, buttonCallbackSetNew, NULL);
	NextionAddComp(&nextion, &LoadSW, "loadSW", 2, 14, buttonCallbackLoadSW, NULL);
	NextionAddComp(&nextion, &VoltageSrc, "VsrcSW", 2, 15, buttonCallbackVolatageSrc, NULL);
	NextionAddComp(&nextion, &BackToMainMenu, "back", 2, 5, buttonCallbackBack, NULL);

	//Set Value page component
	NextionAddComp(&nextion, &SaveValue, "save", 5, 9, buttonCallbackSave, NULL);
	NextionAddComp(&nextion, &plus10, "p10", 5, 1, buttonCallbackPlus10, NULL);
	NextionAddComp(&nextion, &plus1, "p1", 5, 2, buttonCallbackPlus1, NULL);
	NextionAddComp(&nextion, &plus01, "p01", 5, 3, buttonCallbackPlus01, NULL);
	NextionAddComp(&nextion, &plus001, "p001", 5, 4, buttonCallbackPlus001, NULL);
	NextionAddComp(&nextion, &minus10, "m10", 5, 5, buttonCallbackMinus10, NULL);
	NextionAddComp(&nextion, &minus1, "m1", 5, 6, buttonCallbackMinsu1, NULL);
	NextionAddComp(&nextion, &minus01, "m01", 5, 7, buttonCallbackMinus01, NULL);
	NextionAddComp(&nextion, &minus001, "m001", 5, 8, buttonCallbackMinus001, NULL);
	NextionAddComp(&nextion, &NewValueDisp, "value", 5, 10, NULL, NULL);
	NextionAddComp(&nextion, &NewValueUnit, "unit", 5, 11, NULL, NULL);

	//PV moad page
	NextionAddComp(&nextion, &PVchart, "PVchart", 3, 2, NULL, NULL);

	  ina.hspi = &hspi1;
	  ina.cs_port = INA_CS_GPIO_Port;
	  ina.cs_pin = INA_CS_Pin;
	  ina.shunt_resistance = 0.001;

	  INA229_ConfigReg cfg = {
	      .reset = 0,
	      .resetAcc = 1,
	      .tempComp = 0,
	      .adcRange = 1,
	      .convDelay_ms = 0
	  };
	  INA229_WriteConfig(&ina, &cfg);


	  INA229_AdcConfigReg adc = {
	      .mode = 0x0f,
	      .vbusCT = 5,
	      .vshCT  = 5,
	      .vtCT   = 5,
	      .avg = 2
	  };
	  INA229_WriteAdcConfig(&ina, &adc);


	  //DAC init
	  HAL_DAC_Start(&hdac, DAC_CHANNEL_1);


	  NextionSetPage(&nextion, 0);
	  TransmitBuffer(&nextion);
	  HAL_Delay(1000);
	  NextionSetPage(&nextion, 1);
	  TransmitBuffer(&nextion);
	  HAL_Delay(100);
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
	  if (ControllerEN == 1){
		  controller();
		  ControllerEN = 0;
	  }
	  if (DisplayEN == 1){
		  display();
		  DisplayEN = 0;
	  }
	  if(SafetyCheck == 1){
	  	overheat();
	  	SafetyCheck = 0;
	  }

	  if(voltageSrcState == 1){
		  HAL_GPIO_WritePin(VoltageSrcControl_GPIO_Port, VoltageSrcControl_Pin, GPIO_PIN_SET);
	  }else{
		  HAL_GPIO_WritePin(VoltageSrcControl_GPIO_Port, VoltageSrcControl_Pin, GPIO_PIN_RESET);
	  }

	  if(loadStatus == 1){
		  HAL_GPIO_WritePin(IN_EN_GPIO_Port, IN_EN_Pin, GPIO_PIN_SET);
	  }else{
		  HAL_GPIO_WritePin(IN_EN_GPIO_Port, IN_EN_Pin, GPIO_PIN_RESET);
	  }


    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
  }
  /* USER CODE END 3 */
}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

  /** Configure the main internal regulator output voltage
  */
  __HAL_RCC_PWR_CLK_ENABLE();
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE2);

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSI;
  RCC_OscInitStruct.PLL.PLLM = 8;
  RCC_OscInitStruct.PLL.PLLN = 128;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
  RCC_OscInitStruct.PLL.PLLQ = 2;
  RCC_OscInitStruct.PLL.PLLR = 2;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV4;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV2;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_4) != HAL_OK)
  {
    Error_Handler();
  }
}

/**
  * @brief ADC1 Initialization Function
  * @param None
  * @retval None
  */
static void MX_ADC1_Init(void)
{

  /* USER CODE BEGIN ADC1_Init 0 */

  /* USER CODE END ADC1_Init 0 */

  ADC_ChannelConfTypeDef sConfig = {0};

  /* USER CODE BEGIN ADC1_Init 1 */

  /* USER CODE END ADC1_Init 1 */

  /** Configure the global features of the ADC (Clock, Resolution, Data Alignment and number of conversion)
  */
  hadc1.Instance = ADC1;
  hadc1.Init.ClockPrescaler = ADC_CLOCK_SYNC_PCLK_DIV2;
  hadc1.Init.Resolution = ADC_RESOLUTION_12B;
  hadc1.Init.ScanConvMode = ENABLE;
  hadc1.Init.ContinuousConvMode = DISABLE;
  hadc1.Init.DiscontinuousConvMode = DISABLE;
  hadc1.Init.ExternalTrigConvEdge = ADC_EXTERNALTRIGCONVEDGE_NONE;
  hadc1.Init.ExternalTrigConv = ADC_SOFTWARE_START;
  hadc1.Init.DataAlign = ADC_DATAALIGN_RIGHT;
  hadc1.Init.NbrOfConversion = 4;
  hadc1.Init.DMAContinuousRequests = DISABLE;
  hadc1.Init.EOCSelection = ADC_EOC_SINGLE_CONV;
  if (HAL_ADC_Init(&hadc1) != HAL_OK)
  {
    Error_Handler();
  }

  /** Configure for the selected ADC regular channel its corresponding rank in the sequencer and its sample time.
  */
  sConfig.Channel = ADC_CHANNEL_0;
  sConfig.Rank = 1;
  sConfig.SamplingTime = ADC_SAMPLETIME_480CYCLES;
  if (HAL_ADC_ConfigChannel(&hadc1, &sConfig) != HAL_OK)
  {
    Error_Handler();
  }

  /** Configure for the selected ADC regular channel its corresponding rank in the sequencer and its sample time.
  */
  sConfig.Channel = ADC_CHANNEL_1;
  sConfig.Rank = 2;
  if (HAL_ADC_ConfigChannel(&hadc1, &sConfig) != HAL_OK)
  {
    Error_Handler();
  }

  /** Configure for the selected ADC regular channel its corresponding rank in the sequencer and its sample time.
  */
  sConfig.Channel = ADC_CHANNEL_10;
  sConfig.Rank = 3;
  if (HAL_ADC_ConfigChannel(&hadc1, &sConfig) != HAL_OK)
  {
    Error_Handler();
  }

  /** Configure for the selected ADC regular channel its corresponding rank in the sequencer and its sample time.
  */
  sConfig.Channel = ADC_CHANNEL_11;
  sConfig.Rank = 4;
  if (HAL_ADC_ConfigChannel(&hadc1, &sConfig) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN ADC1_Init 2 */

  /* USER CODE END ADC1_Init 2 */

}

/**
  * @brief DAC Initialization Function
  * @param None
  * @retval None
  */
static void MX_DAC_Init(void)
{

  /* USER CODE BEGIN DAC_Init 0 */

  /* USER CODE END DAC_Init 0 */

  DAC_ChannelConfTypeDef sConfig = {0};

  /* USER CODE BEGIN DAC_Init 1 */

  /* USER CODE END DAC_Init 1 */

  /** DAC Initialization
  */
  hdac.Instance = DAC;
  if (HAL_DAC_Init(&hdac) != HAL_OK)
  {
    Error_Handler();
  }

  /** DAC channel OUT1 config
  */
  sConfig.DAC_Trigger = DAC_TRIGGER_NONE;
  sConfig.DAC_OutputBuffer = DAC_OUTPUTBUFFER_ENABLE;
  if (HAL_DAC_ConfigChannel(&hdac, &sConfig, DAC_CHANNEL_1) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN DAC_Init 2 */

  /* USER CODE END DAC_Init 2 */

}

/**
  * @brief SPI1 Initialization Function
  * @param None
  * @retval None
  */
static void MX_SPI1_Init(void)
{

  /* USER CODE BEGIN SPI1_Init 0 */

  /* USER CODE END SPI1_Init 0 */

  /* USER CODE BEGIN SPI1_Init 1 */

  /* USER CODE END SPI1_Init 1 */
  /* SPI1 parameter configuration*/
  hspi1.Instance = SPI1;
  hspi1.Init.Mode = SPI_MODE_MASTER;
  hspi1.Init.Direction = SPI_DIRECTION_2LINES;
  hspi1.Init.DataSize = SPI_DATASIZE_8BIT;
  hspi1.Init.CLKPolarity = SPI_POLARITY_LOW;
  hspi1.Init.CLKPhase = SPI_PHASE_2EDGE;
  hspi1.Init.NSS = SPI_NSS_SOFT;
  hspi1.Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_256;
  hspi1.Init.FirstBit = SPI_FIRSTBIT_MSB;
  hspi1.Init.TIMode = SPI_TIMODE_DISABLE;
  hspi1.Init.CRCCalculation = SPI_CRCCALCULATION_DISABLE;
  hspi1.Init.CRCPolynomial = 10;
  if (HAL_SPI_Init(&hspi1) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN SPI1_Init 2 */

  /* USER CODE END SPI1_Init 2 */

}

/**
  * @brief TIM2 Initialization Function
  * @param None
  * @retval None
  */
static void MX_TIM2_Init(void)
{

  /* USER CODE BEGIN TIM2_Init 0 */

  /* USER CODE END TIM2_Init 0 */

  TIM_ClockConfigTypeDef sClockSourceConfig = {0};
  TIM_MasterConfigTypeDef sMasterConfig = {0};

  /* USER CODE BEGIN TIM2_Init 1 */

  /* USER CODE END TIM2_Init 1 */
  htim2.Instance = TIM2;
  htim2.Init.Prescaler = 64;
  htim2.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim2.Init.Period = 9999;
  htim2.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
  htim2.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
  if (HAL_TIM_Base_Init(&htim2) != HAL_OK)
  {
    Error_Handler();
  }
  sClockSourceConfig.ClockSource = TIM_CLOCKSOURCE_INTERNAL;
  if (HAL_TIM_ConfigClockSource(&htim2, &sClockSourceConfig) != HAL_OK)
  {
    Error_Handler();
  }
  sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
  sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
  if (HAL_TIMEx_MasterConfigSynchronization(&htim2, &sMasterConfig) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN TIM2_Init 2 */

  /* USER CODE END TIM2_Init 2 */

}

/**
  * @brief UART4 Initialization Function
  * @param None
  * @retval None
  */
static void MX_UART4_Init(void)
{

  /* USER CODE BEGIN UART4_Init 0 */

  /* USER CODE END UART4_Init 0 */

  /* USER CODE BEGIN UART4_Init 1 */

  /* USER CODE END UART4_Init 1 */
  huart4.Instance = UART4;
  huart4.Init.BaudRate = 9600;
  huart4.Init.WordLength = UART_WORDLENGTH_8B;
  huart4.Init.StopBits = UART_STOPBITS_1;
  huart4.Init.Parity = UART_PARITY_NONE;
  huart4.Init.Mode = UART_MODE_TX_RX;
  huart4.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart4.Init.OverSampling = UART_OVERSAMPLING_16;
  if (HAL_UART_Init(&huart4) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN UART4_Init 2 */

  /* USER CODE END UART4_Init 2 */

}

/**
  * Enable DMA controller clock
  */
static void MX_DMA_Init(void)
{

  /* DMA controller clock enable */
  __HAL_RCC_DMA1_CLK_ENABLE();

  /* DMA interrupt init */
  /* DMA1_Stream4_IRQn interrupt configuration */
  HAL_NVIC_SetPriority(DMA1_Stream4_IRQn, 0, 0);
  HAL_NVIC_EnableIRQ(DMA1_Stream4_IRQn);

}

/**
  * @brief GPIO Initialization Function
  * @param None
  * @retval None
  */
static void MX_GPIO_Init(void)
{
  GPIO_InitTypeDef GPIO_InitStruct = {0};
  /* USER CODE BEGIN MX_GPIO_Init_1 */

  /* USER CODE END MX_GPIO_Init_1 */

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOC_CLK_ENABLE();
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();
  __HAL_RCC_GPIOD_CLK_ENABLE();

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(FREE4_GPIO_Port, FREE4_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(INA_CS_GPIO_Port, INA_CS_Pin, GPIO_PIN_SET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOB, IN_EN_Pin|FREE3_Pin|VoltageSrcControl_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(LED_GPIO_Port, LED_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pins : FREE4_Pin INA_CS_Pin */
  GPIO_InitStruct.Pin = FREE4_Pin|INA_CS_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

  /*Configure GPIO pin : INA_ALERT_Pin */
  GPIO_InitStruct.Pin = INA_ALERT_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(INA_ALERT_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pins : IN_EN_Pin FREE3_Pin VoltageSrcControl_Pin */
  GPIO_InitStruct.Pin = IN_EN_Pin|FREE3_Pin|VoltageSrcControl_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

  /*Configure GPIO pin : LED_Pin */
  GPIO_InitStruct.Pin = LED_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(LED_GPIO_Port, &GPIO_InitStruct);

  /* USER CODE BEGIN MX_GPIO_Init_2 */

  /* USER CODE END MX_GPIO_Init_2 */
}

/* USER CODE BEGIN 4 */

/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */
  __disable_irq();
  while (1)
  {
  }
  /* USER CODE END Error_Handler_Debug */
}
#ifdef USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */
