/*
 * 002LEDButton.c
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

/*uint8_t button_press(){
	uint32_t value = *IDR&0x00000001;
	value=(uint8_t)value;
	return value;
}*/
int main(void)
{

 uint32_t *RCC_AHB1ENR = (uint32_t *)(0x40023800+0x30);
 *RCC_AHB1ENR |= (1<<3);

 uint32_t *MODER_GPIOD = (uint32_t *)(0x40020C00);
 *MODER_GPIOD|=(1<<24);

 uint32_t *ODR = (uint32_t *)(0x40020C00 + 0x14);

 //button settings ON PA0

 *RCC_AHB1ENR |= (1<<0);

 uint32_t *MODER_GPIOA = (uint32_t *)(0x40020000);
 *MODER_GPIOA|=(0<<0);

 uint32_t *IDR = (uint32_t *)(0x40020000+ 0x10);
 //*IDR=0x00;
 while(1){

	 if(*IDR&0x00000001){
		 *ODR |= (1<<12);
		 delay();//to avoid deboucning
		 printf("button presssed \n");

	 }
	 *ODR=0;
 }
}
