#include "main.h"
#include "i2c.h"
#include "pca9633.h"
#include <stdlib.h>

static void Write8(pca9633_t *pca, uint8_t cmd, uint8_t data)
{
	HAL_I2C_Mem_Write(pca->i2c, (pca->address << 1), cmd, 1, &data, 1, 1000);
}

static void Write32(pca9633_t *pca, uint8_t cmd, uint8_t *data)
{
	HAL_I2C_Mem_Write(pca->i2c, (pca->address << 1), cmd, 1, data, 4, 1000);
}

static void Write16(pca9633_t *pca, uint8_t cmd, uint8_t *data)
{
	HAL_I2C_Mem_Write(pca->i2c, (pca->address << 1), cmd, 1, data, 2, 1000);
}

static uint8_t Read8(pca9633_t *pca, uint8_t cmd)
{
	uint8_t data;
	HAL_I2C_Mem_Read(pca->i2c, (pca->address << 1), cmd, 1, &data, 1, 1000);
	return data;
}

void pca9633Init(pca9633_t *pca, I2C_HandleTypeDef *i2c, uint8_t address)
{
	pca->address = address;
	pca->i2c = i2c;
	pca->blinkingFlag = 0;
	Write8(pca, MODE1, 0x00);
	Write8(pca, MODE2, 0x00);
	Write8(pca, LEDOUT, 0xFF);
}

void pca9633SetPWM(pca9633_t *pca, uint8_t PWMChannel, uint8_t value)
{
	if (PWMChannel >= 0 && PWMChannel <= 3)
		Write8(pca, PWMChannel, value);
}

void pca9633SetAllPWM(pca9633_t *pca, uint8_t channel0, uint8_t channel1,
		uint8_t channel2, uint8_t channel3)
{
	uint8_t data[4] =
	{ channel0, channel1, channel2, channel3 };

	Write32(pca, INCIND | PWM0, data);
}

void pca9633SetBrightness(pca9633_t *pca, uint8_t value)
{
	if (!pca->blinkingFlag)
		Write8(pca, GRPPWM, value);
	else
		return;
}

void pca9633ChangeBrightnessLinearly(pca9633_t *pca, uint8_t startValue,
		uint8_t endValue, uint32_t time)
{
	uint32_t oldTime = HAL_GetTick();
	uint8_t diff = abs((int) endValue - (int) startValue);
	float step = (float) diff / (float) time;
	float temp = startValue;

	if (diff == 0)
	{
		return;
	}
	else if (startValue > endValue)
	{
		pca9633SetBrightness(pca, startValue);

		while ((uint8_t) temp != endValue)
		{
			while (HAL_GetTick() - oldTime > 1)
			{
				temp -= (step * 2);
				pca9633SetBrightness(pca, temp);
				oldTime = HAL_GetTick();
			}
		}
	}
	else if (startValue < endValue)
	{
		pca9633SetBrightness(pca, startValue);

		while ((uint8_t) temp != endValue)
		{
			while (HAL_GetTick() - oldTime > 1)
			{
				temp += (step * 2);
				pca9633SetBrightness(pca, temp);
				oldTime = HAL_GetTick();
			}
		}
	}
}

void pca9633TurnOnBlinking(pca9633_t *pca)
{
	uint8_t temp = Read8(pca, MODE2);
	Write8(pca, MODE2, temp | DMBLINK);
	pca->blinkingFlag = 1;
}

void pca9633TurnOffBlinking(pca9633_t *pca)
{
	uint8_t temp = Read8(pca, MODE2);
	Write8(pca, MODE2, temp & 0xDF);
	pca9633SetBrightness(pca, 255);
	pca->blinkingFlag = 1;
}

void pca9633SetBlinking(pca9633_t *pca, uint8_t period, uint8_t dutyCycle)
{
	if(pca->blinkingFlag)
	{
		uint8_t data[2] = {dutyCycle, period};

		Write16(pca, INCGBL | GRPPWM, data);
	}
	else
		return;
}
