/*
 * 001toggleLED.c
 *
 *  Created on: May 13, 2021
 *      Author: chira
 */
#include "stm32f407.h"

#include <stdint.h>
#include <stdio.h>

void delay(void){
	for(int i=0;i<5000000;i++);
}
int main(void)
{
 /*GPIO_Handle_t toggleLED;
 toggleLED.pGPIOx =GPIOD;
 toggleLED.GPIO_PinConfig.GPIO_PinNumber=GPIO_PIN_NO_12;
 toggleLED.GPIO_PinConfig.GPIO_PinMode=GPIO_MODE_OUT;
 toggleLED.GPIO_PinConfig.GPIO_PinSpeed=GPIO_SPEED_FAST;
 toggleLED.GPIO_PinConfig.GPIO_PinOPType=GPIO_OP_TYPE_OD;
 toggleLED.GPIO_PinConfig.GPIO_PinPuPdControl=GPIO_NO_PUPD;
 GPIO_PeriClockControl(GPIOD, ENABLE);
 GPIO_Init(&toggleLED);
*/
 uint32_t *RCC_AHB1ENR = (uint32_t *)(0x40023800+0x30);
 *RCC_AHB1ENR |= (1<<3);

 uint32_t *MODER = (uint32_t *)(0x40020C00);
 *MODER|=(1<<24);

 uint32_t *ODR = (uint32_t *)(0x40020C00 + 0x14);

 while(1){
	 *ODR |= (1<<12);
	 printf("toggling\n");
	 delay();
	 printf("return from delay\n");
	 *ODR = 0x00;

 }

}
