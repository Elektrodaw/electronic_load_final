#ifndef INA_229_H
#define INA_229_H

#include "stm32f4xx_hal.h"
#include <stdint.h>

typedef struct {
    SPI_HandleTypeDef *hspi;
    GPIO_TypeDef *cs_port;
    uint16_t cs_pin;
    float shunt_resistance;
} INA229_Handle;

typedef struct {
    uint8_t reset;
    uint8_t resetAcc;
    uint8_t tempComp;
    uint8_t adcRange;
    uint16_t convDelay_ms;
} INA229_ConfigReg;


typedef struct {
    uint8_t mode;
    uint8_t vbusCT;
    uint8_t vshCT;
    uint8_t vtCT;
    uint8_t avg;
} INA229_AdcConfigReg;



#define INA229_REG_CONFIG              0x00
#define INA229_REG_ADC_CONFIG          0x01
#define INA229_REG_SHUNT_CAL           0x02
#define INA229_REG_VSHUNT              0x04
#define INA229_REG_VBUS                0x05
#define INA229_REG_DIETEMP             0x06
#define INA229_REG_CURRENT             0x07
#define INA229_REG_POWER               0x08

void 	 INA229_WriteConfig(INA229_Handle *ina, INA229_ConfigReg *cfg);
void 	 INA229_WriteAdcConfig(INA229_Handle *ina, INA229_AdcConfigReg *cfg);
void     INA229_Init(INA229_Handle *ina);
uint16_t INA229_Read16(INA229_Handle *ina, uint8_t reg);
uint32_t INA229_Read24(INA229_Handle *ina, uint8_t reg);
uint64_t INA229_Read48(INA229_Handle *ina, uint8_t reg);

float INA229_ReadBusVoltage(INA229_Handle *ina);
float INA229_ReadShuntVoltage(INA229_Handle *ina);
float INA229_ReadCurrent(INA229_Handle *ina);
float INA229_ReadPower(INA229_Handle *ina);

#endif
