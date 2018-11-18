#include "gpio_alias.h"
#include "gpio.h"
#include <string.h>

const GPIO GPIO_TABLE[] = {
	
	/* Nucleo I/O */
	FILL_GPIO("LED",			GPIOA, 5, OUTPUT, LOW_SPEED, NONE, true, LED)

	/* Nucleo USB USART */
	FILL_AFIO("UART TX",		GPIOA, 2, ALT, 7, HIGH_SPEED, NONE, true, OTHER)
	FILL_AFIO("UART RX",		GPIOA, 3, ALT, 7, HIGH_SPEED, NONE, true, OTHER)

	/* Application RADIO */
	/* USART */
	FILL_AFIO("RADIO UART TX",		GPIOA, 9, ALT, 7, HIGH_SPEED, NONE, true, RADIO)
	FILL_AFIO("RADIO UART RX",		GPIOA, 10, ALT, 7, HIGH_SPEED, NONE, true, RADIO)
    /* Mode 0 */
	FILL_GPIO("RADIO MODE 0",		GPIOC,  4, OUTPUT, LOW_SPEED, NONE, true, RADIO)
    /* Mode 1 */
	FILL_GPIO("RADIO MODE 1",		GPIOB,  1, OUTPUT, LOW_SPEED, NONE, true, RADIO)

	/* Application PWM ESC*/
	/* TIM 3 Ch 1 */
	FILL_AFIO("MOTOR 1",		GPIOB,  4, ALT, 2, LOW_SPEED, NONE, true, MOTOR)
	/* TIM 3 Ch 2 */
	FILL_AFIO("MOTOR 2",		GPIOB,  10, ALT, 1, LOW_SPEED, NONE, true, MOTOR)
	/* TIM 2 Ch 2 */
	FILL_AFIO("MOTOR 3",		GPIOB,  3, ALT, 1, LOW_SPEED, NONE, true, MOTOR)
	/* TIM 2 Ch 3*/
	FILL_AFIO("MOTOR 4",		GPIOC,  7, ALT, 2, LOW_SPEED, NONE, true, MOTOR)
	
	/* Application I2C */
	FILL_AFIO("SCL",		GPIOB, 8, ALT, 4, LOW_SPEED, PULL_UP, true, I2C)
	FILL_AFIO("SDA",		GPIOB, 9, ALT, 4, LOW_SPEED, PULL_UP, true, I2C)

};

const uint8_t NUM_GPIO_ALIAS = sizeof(GPIO_TABLE)/sizeof(GPIO);
const char * GROUP_NAMES[] = {"OTHER", "SENSOR", "MOTOR", "RADIO", "LED", "I2C", "PRESSURE", "BRAKE", "PROP"};
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

