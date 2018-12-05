/*
 * main.c
 *
 *  Created on: 2018. 12. 5.
 *      Author: Wed team 10
 */

#include "stm32f10x_rcc.h"
#include "stm32f10x_gpio.h"
#include "stm32f10x_adc.h"
#include "stm32f10x_dma.h"
#include "stm32f10x.h"
#include "misc.h"

void RCC_configure(void) {
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOD | RCC_APB2Periph_GPIOC | RCC_APB2Periph_GPIOB |RCC_APB2Periph_GPIOA, ENABLE);
}


void GPIO_configure(void) {
    GPIO_InitTypeDef GPIOx;

    GPIOx.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIOx.GPIO_Pin  = (GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_2 | GPIO_Pin_13 | GPIO_Pin_14 | GPIO_Pin_15);
    GPIOx.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOC, &GPIOx);
    GPIOx.GPIO_Pin = (GPIO_Pin_2);
    GPIO_Init(GPIOD, &GPIOx);
    GPIOx.GPIO_Pin  = (GPIO_Pin_2 | GPIO_Pin_3 | GPIO_Pin_4 | GPIO_Pin_5 | GPIO_Pin_6);
    GPIO_Init(GPIOE, &GPIOx);


    GPIOx.GPIO_Mode = GPIO_Mode_IPD;
    GPIOx.GPIO_Pin = (GPIO_Pin_8 | GPIO_Pin_9 | GPIO_Pin_10 | GPIO_Pin_10 | GPIO_Pin_11 | GPIO_Pin_12 | GPIO_Pin_13 | GPIO_Pin_14 | GPIO_Pin_15);
    GPIO_Init(GPIOA, &GPIOx);
    GPIOx.GPIO_Pin = (GPIO_Pin_3 | GPIO_Pin_4 | GPIO_Pin_5 | GPIO_Pin_6 | GPIO_Pin_7 | GPIO_Pin_8 | GPIO_Pin_9);
    GPIO_Init(GPIOB, &GPIOx);
    GPIOx.GPIO_Pin = (GPIO_Pin_2 | GPIO_Pin_6 | GPIO_Pin_7 | GPIO_Pin_8 | GPIO_Pin_9);
	GPIO_Init(GPIOC, &GPIOx);
	GPIOx.GPIO_Pin = (GPIO_Pin_3 | GPIO_Pin_4 | GPIO_Pin_5 | GPIO_Pin_6 | GPIO_Pin_7);
	GPIO_Init(GPIOD, &GPIOx);
	GPIOx.GPIO_Pin = (GPIO_Pin_0 | GPIO_Pin_1);
	GPIO_Init(GPIOE, &GPIOx);
}

int main(void) {
	int i=0;
	SystemInit();
	RCC_configure();
	GPIO_configure();
	while(1) {
		if((!GPIO_ReadInputDataBit(GPIOC,GPIO_Pin_2)) || GPIO_ReadInputDataBit(GPIOC,GPIO_Pin_6)) {
			GPIO_SetBits(GPIOD,GPIO_Pin_2);
			for(i=1; i<=1000; i++);
		}
		else {
			GPIO_ResetBits(GPIOD,GPIO_Pin_2);
		}
	}

	return 0;
}
