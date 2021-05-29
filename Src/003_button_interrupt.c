/*
 * 003_button_interrupt.c
 *
 *  Created on: May 15, 2021
 *      Author: chira
 */

#include "stm32f407.h"

#include <stdint.h>
#include <stdio.h>



void delay(void){
	for(int i=0;i<50000;i++);
}

int main(void)
{
//SETTING PD12 as output LED
 uint32_t *RCC_AHB1ENR = (uint32_t *)(0x40023800+0x30);
 *RCC_AHB1ENR |= (1<<3);

 uint32_t *MODER_GPIOD = (uint32_t *)(0x40020C00);
 *MODER_GPIOD|=(1<<24);


 //button settings ON PD5 as interrupt mode

//clearing contents of ICER register
 uint32_t *NVIC_ICER =(uint32_t *)(0XE000E180);
 *NVIC_ICER=(1<<5);

 uint32_t *EXTI_FSR= (uint32_t *)(0x40013C00+0x0C); //SETTING THE interrupt to falling edge trigger
 *EXTI_FSR |=(1<<5);


 //setting the respective exti register to accept the interrupt
 uint32_t temp2=5%4;
 uint32_t *SYSCFG_EXTICR2 =(uint32_t *)(0x40013800+ 0x0C);
 *SYSCFG_EXTICR2= 3<<(temp2*4);

 //enable the exti interrupt delivery using IMR
uint32_t *EXTI_IMR =(uint32_t *)(0x40013C00);
*EXTI_IMR =(1<<5);

//setting pin 5 as interrupt in ISER
uint32_t *NVIC_ISER_0= (uint32_t *) (0xE000E100);
*NVIC_ISER_0=(1<<5);


//setting priority of the Button
uint32_t *NVIC_IPriSR=(uint32_t*)(0xE000E400);
uint8_t iprx = 5 / 4;
uint8_t iprx_section  = 5 %4 ; //5 is IRQ number
uint8_t shift_amount = ( 8 * iprx_section) + ( 8 - 4) ;
*(NVIC_IPriSR+ iprx ) |=  ( 8<< shift_amount );



 while(1);
}

void EXTI9_5_IRQHandler (void){
	delay();
	uint32_t *ODR = (uint32_t *)(0x40020C00 + 0x14);
	uint32_t *EXTI_PR=(uint32_t *)(0x40013800+0x14);
	printf("entered interrupt\n");

	if(*(EXTI_PR) & ( 1 << 5))
		{
			//clear
			*EXTI_PR |= ( 1 <<5);
		}

	*ODR |= (1<<12);
	delay();
	*ODR &= ~(1<<12);
}

