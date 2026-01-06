#include "INA229.h"

static void INA229_Select(INA229_Handle *ina) {
    HAL_GPIO_WritePin(ina->cs_port, ina->cs_pin, GPIO_PIN_RESET);
}

static void INA229_Unselect(INA229_Handle *ina) {
    HAL_GPIO_WritePin(ina->cs_port, ina->cs_pin, GPIO_PIN_SET);
}

uint16_t INA229_Read16(INA229_Handle *ina, uint8_t reg) {
    uint8_t tx[3];
    uint8_t rx[3];

    tx[0] = (reg << 2) | 0x01;

    INA229_Select(ina);
    HAL_SPI_Transmit(ina->hspi, tx, 1, HAL_MAX_DELAY);
    HAL_SPI_Receive(ina->hspi, rx, 2, HAL_MAX_DELAY);
    INA229_Unselect(ina);

    return ((uint16_t)rx[0] << 8) | rx[1];
}

uint32_t INA229_Read24(INA229_Handle *ina, uint8_t reg)
{
    uint8_t tx = (reg << 2) | 0x01;   // poprawne kodowanie ramki
    uint8_t rx[3];

    INA229_Select(ina);
    HAL_SPI_Transmit(ina->hspi, &tx, 1, HAL_MAX_DELAY);
    HAL_SPI_Receive(ina->hspi, rx, 3, HAL_MAX_DELAY);
    INA229_Unselect(ina);

    return ((uint32_t)rx[0] << 16) |
           ((uint32_t)rx[1] << 8)  |
            (uint32_t)rx[2];
}


uint64_t INA229_Read48(INA229_Handle *ina, uint8_t reg) {
    uint8_t tx = (reg << 2) | 0x01;
    uint8_t rx[6];

    INA229_Select(ina);
    HAL_SPI_Transmit(ina->hspi, &tx, 1, HAL_MAX_DELAY);
    HAL_SPI_Receive(ina->hspi, rx, 6, HAL_MAX_DELAY);
    INA229_Unselect(ina);

    uint64_t value = 0;
    for (int i = 0; i < 6; i++) {
        value = (value << 8) | rx[i];
    }
    return value;
}

static void INA229_Write16(INA229_Handle *ina, uint8_t reg, uint16_t value)
{
    uint8_t tx[3];
    tx[0] = (reg << 2) | 0x00;   // adres + WRITE
    tx[1] = (value >> 8) & 0xFF;
    tx[2] = value & 0xFF;

    INA229_Select(ina);
    HAL_SPI_Transmit(ina->hspi, tx, 3, HAL_MAX_DELAY);
    INA229_Unselect(ina);
}


float INA229_ReadBusVoltage(INA229_Handle *ina) {
    uint32_t raw = INA229_Read24(ina, INA229_REG_VBUS);
    raw = raw & 0xFFFFF0;
    raw >>= 4;
    return raw * 0.0001953125f;
}

float INA229_ReadShuntVoltage(INA229_Handle *ina) {
    uint32_t raw = INA229_Read24(ina, INA229_REG_VSHUNT);
    raw = raw & 0xFFFFF0;
	raw >>= 4;
	 if (raw & 0x80000) {          // if sign bit (bit 19) set
		raw |= 0xFFF00000;        // sign-extend to 32-bit
	}
	return raw * 0.000000078125f;
}


void INA229_WriteConfig(INA229_Handle *ina, INA229_ConfigReg *cfg)
{
    uint16_t reg = 0;

    if (cfg->reset)     reg |= (1 << 15);
    if (cfg->resetAcc)  reg |= (1 << 14);

    uint8_t delaySteps = cfg->convDelay_ms / 2;
    if (delaySteps > 0xFF) delaySteps = 0xFF;

    reg |= ((uint16_t)delaySteps << 6);

    if (cfg->tempComp)  reg |= (1 << 5);
    if (cfg->adcRange)  reg |= (1 << 4);

    INA229_Write16(ina, INA229_REG_CONFIG, reg);
}


void INA229_WriteAdcConfig(INA229_Handle *ina, INA229_AdcConfigReg *cfg)
{
    uint16_t reg = 0;

    reg |= ((uint16_t)(cfg->mode  & 0x0F) << 12);
    reg |= ((uint16_t)(cfg->vbusCT & 0x07) << 9);
    reg |= ((uint16_t)(cfg->vshCT  & 0x07) << 6);
    reg |= ((uint16_t)(cfg->vtCT   & 0x07) << 3);
    reg |=  (cfg->avg & 0x07);

    INA229_Write16(ina, INA229_REG_ADC_CONFIG, reg);
}
