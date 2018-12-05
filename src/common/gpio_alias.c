#include "gpio_alias.h"
#include "gpio.h"
#include <string.h>

const GPIO GPIO_TABLE[] = {

	/* LED */
	FILL_GPIO("ACT LED",			GPIOA, 0, OUTPUT, LOW_SPEED, NONE, true, LED)
	FILL_GPIO("STAT LED",			GPIOA, 1, OUTPUT, LOW_SPEED, NONE, true, LED)
	FILL_GPIO("FAULT LED",			GPIOC, 3, OUTPUT, LOW_SPEED, NONE, true, LED)

	/* USB UART */
	FILL_AFIO("UART TX",		GPIOC, 10, ALT, 5, HIGH_SPEED, NONE, true, OTHER)
	FILL_AFIO("UART RX",		GPIOC, 11, ALT, 5, HIGH_SPEED, NONE, true, OTHER)

	/* LIDAR1 USART2 */
	FILL_AFIO("LIDAR1 UART TX",		GPIOA, 2, ALT, 7, HIGH_SPEED, NONE, true, OTHER)
	FILL_AFIO("LIDAR1 UART RX",		GPIOA, 3, ALT, 7, HIGH_SPEED, NONE, true, OTHER)

	/* LIDAR2 USART3 */
	FILL_AFIO("LIDAR2 UART TX",		GPIOB, 9, ALT, 7, HIGH_SPEED, NONE, true, OTHER)
	FILL_AFIO("LIDAR2 UART RX",		GPIOB, 8, ALT, 7, HIGH_SPEED, NONE, true, OTHER)

	/* Application RADIO */
	/* USART */
	FILL_AFIO("RADIO USART1 TX",		GPIOC, 4, ALT, 7, HIGH_SPEED, NONE, true, RADIO)
	FILL_AFIO("RADIO USART1 RX",		GPIOC, 5, ALT, 7, HIGH_SPEED, NONE, true, RADIO)
    /* Mode 0 */
	FILL_GPIO("RADIO MODE 0",		GPIOB,  11, OUTPUT, LOW_SPEED, NONE, true, RADIO)
    /* Mode 1 */
	FILL_GPIO("RADIO MODE 1",		GPIOB,  10, OUTPUT, LOW_SPEED, NONE, true, RADIO)
    /* AUX */
	FILL_GPIO("RADIO AUX",		GPIOB,  2, INPUT, LOW_SPEED, NONE, true, RADIO)

	/* Application PWM ESC*/
	/* TIM 3 Ch 1 */
	FILL_AFIO("POSX PWM",		GPIOB,  4, ALT, 2, LOW_SPEED, NONE, true, MOTOR)
	/* TIM 3 Ch 2 */
	FILL_AFIO("POSY PWM",		GPIOB,  5, ALT, 2, LOW_SPEED, NONE, true, MOTOR)
	/* TIM 2 Ch 2 */
	FILL_AFIO("NEGX PWM",		GPIOB,  0, ALT, 2, LOW_SPEED, NONE, true, MOTOR)
	/* TIM 2 Ch 3*/
	FILL_AFIO("NEGY PWM",		GPIOC,  1, ALT, 2, LOW_SPEED, NONE, true, MOTOR)

	/* Application I2C */
	FILL_AFIO("SCL",		GPIOA, 9, ALT, 4, LOW_SPEED, PULL_UP, true, I2C)
	FILL_AFIO("SDA",		GPIOA, 10, ALT, 4, LOW_SPEED, PULL_UP, true, I2C)

	/* Application SPI*/
	FILL_AFIO("SPI1_NSS",		GPIOA, 4, ALT, 5, HIGH_SPEED, NONE, true, SPI)
	FILL_AFIO("SPI1_SCK",		GPIOA, 5, ALT, 5, HIGH_SPEED, NONE, true, SPI)
	FILL_AFIO("SPI1_MISO",		GPIOA, 6, ALT, 5, HIGH_SPEED, NONE, true, SPI)
	FILL_AFIO("SPI1_MOSI",		GPIOA, 7, ALT, 5, HIGH_SPEED, NONE, true, SPI)

	/* Application Analog */
	/* Analog1 */
	FILL_GPIO("VBATT 1S",		GPIOB,  12, ANALOG, LOW_SPEED, NONE, true, SENSOR)
	/* Analog2 */
	FILL_GPIO("VBATT 2S",		GPIOB,  13, ANALOG, LOW_SPEED, NONE, true, SENSOR)
	/* Analog3 */
	FILL_GPIO("VBATT 3S",		GPIOB,  14, ANALOG, LOW_SPEED, NONE, true, SENSOR)
	/* Analog4 */
	FILL_GPIO("CURR MEAS",		GPIOB,  15, ANALOG, LOW_SPEED, NONE, true, SENSOR)

};

const uint8_t NUM_GPIO_ALIAS = sizeof(GPIO_TABLE)/sizeof(GPIO);
const char * GROUP_NAMES[] = {"OTHER", "SENSOR", "MOTOR", "RADIO", "LED", "I2C", "SPI", "BRAKE", "PROP"};
const uint8_t NUM_GROUPS = sizeof(GROUP_NAMES)/sizeof(char *);

int gpioAliasInit(){
	int i;
	GPIO_TypeDef * port;
	uint8_t pin;

	for(i = 0; i < NUM_GPIO_ALIAS; i++){
		if (GPIO_TABLE[i].usable){
			port = GPIO_TABLE[i].port;
			pin = GPIO_TABLE[i].pin;

			gpio_setClock(port, true);
			gpio_setMode(port, pin, GPIO_TABLE[i].mode);
			gpio_setSpeed(port, pin, GPIO_TABLE[i].speed);

			if (GPIO_TABLE[i].mode == ALT)
				gpio_setAlternateFunc(port, pin, GPIO_TABLE[i].af_val);

			if (GPIO_TABLE[i].group == I2C)
				gpio_openDrainState(port, pin, true);

			if (GPIO_TABLE[i].mode == OUTPUT)
				gpio_writePin(port, pin, 0);
		}
	}
	return 0;
}

int hasGpioAlias(GPIO_TypeDef ** port, uint8_t * pin, char * name) {
	int i;
	for (i = 0; i<NUM_GPIO_ALIAS;i++){
		if (strcmp(GPIO_TABLE[i].name, name)==0) {
			*pin = GPIO_TABLE[i].pin;
			*port = GPIO_TABLE[i].port;
			return 1;
		}
	}
	return 0;
}

int getGpioAlias(GPIO_TypeDef ** port, uint8_t * pin, GPIO * alias) {
	int i; 
	for (i = 0; i<NUM_GPIO_ALIAS; i++){
		if (GPIO_TABLE[i].pin == *pin && GPIO_TABLE[i].port == *port) {
			*alias=	GPIO_TABLE[i];
			return 1;
		}	
	}
	return 0;
}

int getGpioAliasIndex(const char *str) {
	unsigned int i;
	for (i = 0; i < NUM_GROUPS; i++) {
		if (!strcmp(str, GROUP_NAMES[i]))
			return i;
	}
	return -1;
}
