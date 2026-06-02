/*
 * PC_com.c
 *
 *  Created on: Jun 2, 2026
 *      Author: ddaw7
 */
#include "PC_com.h"

#define BUFF_LENGTH 32
static char tx_buff[BUFF_LENGTH] = {0};




void Transmit_Continius_Data(){

memset(tx_buff, 0 ,sizeof(tx_buff));

//sent mode, current in mA and voltage in mV
sprintf(tx_buff, "%1u;%05u;%05u\r\n",actualMode,(uint16_t)(current_mes*1000),(uint16_t)(voltage_mes*1000));

HAL_UART_Transmit_DMA(&huart2, (uint8_t*)tx_buff, strlen(tx_buff));

}


void Transmit_PV_Data(){
char pv_tx_buff[128] = {0};

sprintf(pv_tx_buff, "PV test mode\r\n");
HAL_UART_Transmit(&huart2, (uint8_t*)pv_tx_buff, strlen(pv_tx_buff), HAL_MAX_DELAY);

sprintf(pv_tx_buff,"MPP power = %4u,%02u W\r\n",(uint16_t)MPP_power,(uint8_t)((MPP_power-(uint16_t)MPP_power)*100));
HAL_UART_Transmit(&huart2, (uint8_t*)pv_tx_buff, strlen(pv_tx_buff), HAL_MAX_DELAY);

sprintf(pv_tx_buff,"MPP current = %2u,%02u W\r\n",(uint16_t)MPP_current,(uint8_t)((MPP_current-(uint16_t)MPP_current)*100));
HAL_UART_Transmit(&huart2, (uint8_t*)pv_tx_buff, strlen(pv_tx_buff), HAL_MAX_DELAY);

sprintf(pv_tx_buff,"MPP voltage = %2u,%02u W\r\n",(uint16_t)MPP_voltage,(uint8_t)((MPP_voltage-(uint16_t)MPP_voltage)*100));
HAL_UART_Transmit(&huart2, (uint8_t*)pv_tx_buff, strlen(pv_tx_buff), HAL_MAX_DELAY);

sprintf(pv_tx_buff, "RAW data: current[mA];voltage[mV]\r\n");
HAL_UART_Transmit(&huart2, (uint8_t*)pv_tx_buff, strlen(pv_tx_buff), HAL_MAX_DELAY);

HAL_Delay(1000);

for(uint16_t i = 0; i<2048 ; i++){
	sprintf(pv_tx_buff,"%05u;%05u\r\n",(uint16_t)(PVcurrent[i]*1000),(uint16_t)(PVvoltage[i]*1000));
	HAL_UART_Transmit(&huart2, (uint8_t*)pv_tx_buff, strlen(pv_tx_buff), HAL_MAX_DELAY);
}
}
