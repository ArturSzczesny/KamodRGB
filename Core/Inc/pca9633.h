#ifndef INC_PCA9633_H_
#define INC_PCA9633_H_

// auto increment definitions Datasheet page 10
#define AI2			(0x80) // mask for AI2
#define AI1			(0x40) // mask for AI1
#define AI0			(0x20) // mask for AI0

#define INCOFF			(0x00) // AI2:0 000 no auto increment
#define INCALL			(0x80) // AI2:0 100 auto inc all registers
#define INCIND			(0xA0) // AI2:0 101 auto inc individual pwm registers
#define INCGBL			(0xC0) // AI2:0 110 auto inc global registers
#define INCINDGBL		(0xE0) // AI2:0 111 auto inc individual and global registers

// register definitions Datasheet page 11
#define MODE1			(0x00) // mode register 1
#define MODE2			(0x01) // mode register 2
#define PWM0			(0x02) // PWM0 brightness control led0
#define PWM1			(0x03) // PWM0 brightness control led0
#define PWM2			(0x04) // PWM0 brightness control led0
#define PWM3			(0x05) // PWM0 brightness control led0
#define GRPPWM			(0x06) // group brightness (duty cycle)
#define GRPFREQ			(0x07) // group frequency
#define LEDOUT			(0x08) // LED output state
#define SUBADR1			(0x09) // i2c bus sub address 1
#define SUBADR2			(0x0A) // i2c bus sub address 1
#define SUBADR3			(0x0B) // i2c bus sub address 1
#define ALLCALLADR		(0x0C) // LED All Call i2c address

// MODE1 definitions
#define SLEEP			(0x10) // bit 4, low power mode enable, RW
#define SUB1			(0x08) // bit 3, PCA9633 responds to sub address 1
#define SUB2			(0x04) // bit 2, PCA9633 responds to sub address 2
#define SUB3			(0x02) // bit 1, PCA9633 responds to sub address 3
#define ALLCALL			(0x01) // bit 0, PCA9633 responds to all call address

// MODE2 definitions
#define DMBLINK			(0x20) // bit 5, group control dim or blink
#define INVRT			(0x10) // bit 4, output logic invert (1=yes, 0=no)
#define OCH				(0x08) // bit 3, 0=output change on stop, 1=output change on ACK
#define OUTDRV			(0x04) // bit 2, output drivers 0=open drain, 1=totem poll - push pull
#define OUTNE1			(0x02) // bit 1, 2 bits see page 13, 16 pin device only
#define OUTNE0			(0x01) // bit 0, see above


typedef struct
{
	uint8_t address;
	I2C_HandleTypeDef *i2c;
	uint8_t blinkingFlag;
} pca9633_t;


void pca9633Init(pca9633_t *pca, I2C_HandleTypeDef *i2c, uint8_t address);
void pca9633SetPWM(pca9633_t *pca, uint8_t PWMChannel, uint8_t value);
void pca9633SetAllPWM(pca9633_t *pca, uint8_t channel0, uint8_t channel1, uint8_t channel2, uint8_t channel3);
void pca9633SetBrightness(pca9633_t *pca, uint8_t Value);
void pca9633ChangeBrightnessLinearly(pca9633_t *pca, uint8_t startValue, uint8_t endValue, uint32_t time);
void pca9633TurnOnBlinking(pca9633_t *pca);
void pca9633TurnOffBlinking(pca9633_t *pca);
void pca9633SetBlinking(pca9633_t *pca, uint8_t period, uint8_t dutyCycle);

#endif /* INC_PCA9633_H_ */
