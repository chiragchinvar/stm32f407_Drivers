#include "stm32f407_gpio.h"
#include <stdio.h>


/*
 * Peripheral Clock setup
 */

/*********************************************************************
 * @fn      		  - GPIO_PeriClockControl
 *
 * @brief             - This function enables or disables peripheral clock for the given GPIO port
 *
 * @param[in]         - base address of the gpio peripheral
 * @param[in]         - ENABLE or DISABLE macros
 * @param[in]         -
 *
 * @return            -  none
 *
 * @Note              -  none

 */
void GPIO_PeriClockControl(GPIO_RegDef_t *pGPIOx, uint8_t EnorDi){
	if(EnorDi==ENABLE){
		if(pGPIOx==GPIOA)GPIOA_PCLK_EN();
		else if(pGPIOx==GPIOB)GPIOB_PCLK_EN();
		else if(pGPIOx==GPIOC)GPIOC_PCLK_EN();
		else if(pGPIOx==GPIOD)GPIOD_PCLK_EN();
		else if(pGPIOx==GPIOE)GPIOE_PCLK_EN();
		else if(pGPIOx==GPIOF)GPIOF_PCLK_EN();
		else if(pGPIOx==GPIOG)GPIOG_PCLK_EN();
		else if(pGPIOx==GPIOH)GPIOH_PCLK_EN();
		else if(pGPIOx==GPIOI)GPIOI_PCLK_EN();

	}
	else {
		if(pGPIOx==GPIOA)GPIOA_PCLK_EN();
				else if(pGPIOx==GPIOB)GPIOB_PCLK_DI();
				else if(pGPIOx==GPIOC)GPIOC_PCLK_DI();
				else if(pGPIOx==GPIOD)GPIOD_PCLK_DI();
				else if(pGPIOx==GPIOE)GPIOE_PCLK_DI();
				else if(pGPIOx==GPIOF)GPIOF_PCLK_DI();
				else if(pGPIOx==GPIOG)GPIOG_PCLK_DI();
				else if(pGPIOx==GPIOH)GPIOH_PCLK_DI();
				else if(pGPIOx==GPIOI)GPIOI_PCLK_DI();

	}
}

/*
 * Init and De-init
 */
/*********************************************************************
 * @fn      		  - GPIO_PeriClockControl
 *
 * @brief             - This function enables or disables peripheral clock for the given GPIO port
 *
 * @param[in]         - base address of the gpio peripheral
 * @param[in]         - ENABLE or DISABLE macros
 * @param[in]         -
 *
 * @return            -  none
 *
 * @Note              -  none

 */
void GPIO_Init(GPIO_Handle_t *pGPIOHandle){
//1.configure mode of GPIO pin
uint32_t temp=0;
GPIO_PeriClockControl(pGPIOHandle->pGPIOx, ENABLE);
	if(pGPIOHandle->GPIO_PinConfig.GPIO_PinMode <= GPIO_MODE_ANALOG) {
		//configuring for non interrupt mode
		printf("Entering control settings \n");
	temp=pGPIOHandle->GPIO_PinConfig.GPIO_PinMode<<(2*pGPIOHandle->GPIO_PinConfig.GPIO_PinNumber);
	pGPIOHandle->pGPIOx->MODER &= ~(0x03<< pGPIOHandle->GPIO_PinConfig.GPIO_PinNumber);
	pGPIOHandle->pGPIOx->MODER|=temp;
	temp=0;

	}
	else {
		if(pGPIOHandle->GPIO_PinConfig.GPIO_PinMode == GPIO_MODE_IT_FT){
			//setting that exti bit in ftsr register
			EXTI->FTSR|=(1<<pGPIOHandle->GPIO_PinConfig.GPIO_PinNumber);
			//clearing that bit in rtsr register for safety
			EXTI->RTSR &= ~(1<<pGPIOHandle->GPIO_PinConfig.GPIO_PinNumber);
		}
		else if(pGPIOHandle->GPIO_PinConfig.GPIO_PinMode == GPIO_MODE_IT_RT){
			//setting that exti bit in rtsr register
						EXTI->RTSR|=(1<<pGPIOHandle->GPIO_PinConfig.GPIO_PinNumber);
						//clearing that bit in ftsr register for safety
						EXTI->FTSR &= ~(1<<pGPIOHandle->GPIO_PinConfig.GPIO_PinNumber);
		}
		else if(pGPIOHandle->GPIO_PinConfig.GPIO_PinMode == GPIO_MODE_IT_RFT){

								EXTI->RTSR |=(1<<pGPIOHandle->GPIO_PinConfig.GPIO_PinNumber);
								EXTI->FTSR |(1<<pGPIOHandle->GPIO_PinConfig.GPIO_PinNumber);
				}
		//configure sysconfig in SYSCNFIG_EXTR
		uint8_t temp1;
		uint8_t temp2;
		temp1=pGPIOHandle->GPIO_PinConfig.GPIO_PinNumber /4;
		temp2=pGPIOHandle->GPIO_PinConfig.GPIO_PinNumber %4;
		uint8_t portcode =GPIO_BASEADDR_TO_CODE(pGPIOHandle->pGPIOx);
	    SYSCFG_PCLK_EN();
		SYSCFG->EXTICR[temp1]|=(portcode << temp2*4);
		//enable EXTI interrupt using IMR
		EXTI->IMR |(1<<pGPIOHandle->GPIO_PinConfig.GPIO_PinNumber);
	}
//2. configure speeds
	temp=pGPIOHandle->GPIO_PinConfig.GPIO_PinSpeed << (2*pGPIOHandle->GPIO_PinConfig.GPIO_PinNumber);
	pGPIOHandle->pGPIOx->OSPEEDR &= ~(0x03 << pGPIOHandle->GPIO_PinConfig.GPIO_PinNumber);

pGPIOHandle->pGPIOx->OSPEEDR|=temp;
temp=0;

//3.configure pull up and pull down

temp=pGPIOHandle->GPIO_PinConfig.GPIO_PinPuPdControl << (2 *pGPIOHandle->GPIO_PinConfig.GPIO_PinNumber);
pGPIOHandle->pGPIOx->PUPDR &= ~(0x03 << pGPIOHandle->GPIO_PinConfig.GPIO_PinNumber);
pGPIOHandle->pGPIOx->PUPDR|=temp;

temp=0;
//4.configure output type
temp=pGPIOHandle->GPIO_PinConfig.GPIO_PinOPType << (pGPIOHandle->GPIO_PinConfig.GPIO_PinNumber);
pGPIOHandle->pGPIOx->OTYPER &= ~(0x01 << pGPIOHandle->GPIO_PinConfig.GPIO_PinNumber);
pGPIOHandle->pGPIOx->OTYPER|=temp;
temp=0;


//4.alt function mode
if(pGPIOHandle->GPIO_PinConfig.GPIO_PinMode==GPIO_MODE_ALTFN){
	//configure alt fn mode
	uint8_t temp1,temp2;
	temp1=pGPIOHandle->GPIO_PinConfig.GPIO_PinNumber/8;
	temp2=pGPIOHandle->GPIO_PinConfig.GPIO_PinNumber%8;
	pGPIOHandle->pGPIOx->AFR[temp1]|= ~(0xF << (4* temp2));
	pGPIOHandle->pGPIOx->AFR[temp1]|=pGPIOHandle->GPIO_PinConfig.GPIO_PinAltFunMode<< (4* temp2);
}
}




/*********************************************************************
 * @fn      		  - GPIO_PeriClockControl
 *
 * @brief             - This function enables or disables peripheral clock for the given GPIO port
 *
 * @param[in]         - base address of the gpio peripheral
 * @param[in]         - ENABLE or DISABLE macros
 * @param[in]         -
 *
 * @return            -  none
 *
 * @Note              -  none

 */
void GPIO_DeInit(GPIO_RegDef_t *pGPIOx){
	if(pGPIOx==GPIOA)GPIOA_PCLK_RESET();
			else if(pGPIOx==GPIOB)GPIOB_PCLK_RESET();
			else if(pGPIOx==GPIOC)GPIOC_PCLK_RESET();
			else if(pGPIOx==GPIOD)GPIOD_PCLK_RESET();
			else if(pGPIOx==GPIOE)GPIOE_PCLK_RESET();
			else if(pGPIOx==GPIOF)GPIOF_PCLK_RESET();
			else if(pGPIOx==GPIOG)GPIOG_PCLK_RESET();
			else if(pGPIOx==GPIOH)GPIOH_PCLK_RESET();
			else if(pGPIOx==GPIOI)GPIOI_PCLK_RESET();
}


/*
 * Data read and write
 *//*********************************************************************
 * @fn      		  - GPIO_PeriClockControl
 *
 * @brief             - This function enables or disables peripheral clock for the given GPIO port
 *
 * @param[in]         - base address of the gpio peripheral
 * @param[in]         - ENABLE or DISABLE macros
 * @param[in]         -
 *
 * @return            -  none
 *
 * @Note              -  none

 */
uint8_t GPIO_ReadFromInputPin(GPIO_RegDef_t *pGPIOx, uint8_t PinNumber){

	uint8_t value;
	value=(uint8_t) ((pGPIOx->IDR>>PinNumber) & 0x00000001);
	return value;
}
/*********************************************************************
 * @fn      		  - GPIO_PeriClockControl
 *
 * @brief             - This function enables or disables peripheral clock for the given GPIO port
 *
 * @param[in]         - base address of the gpio peripheral
 * @param[in]         - ENABLE or DISABLE macros
 * @param[in]         -
 *
 * @return            -  none
 *
 * @Note              -  none

 */

uint16_t GPIO_ReadFromInputPort(GPIO_RegDef_t *pGPIOx){
	uint16_t value;
		value=(uint16_t)(pGPIOx->IDR);
		return value;

}



/*********************************************************************
 * @fn      		  - GPIO_PeriClockControl
 *
 * @brief             - This function enables or disables peripheral clock for the given GPIO port
 *
 * @param[in]         - base address of the gpio peripheral
 * @param[in]         - ENABLE or DISABLE macros
 * @param[in]         -
 *
 * @return            -  none
 *
 * @Note              -  none

 */

void GPIO_WriteToOutputPin(GPIO_RegDef_t *pGPIOx, uint8_t PinNumber, uint8_t Value){
	if(Value == SET)
		//WRITE 1 TO THAT BIT
		pGPIOx->ODR|=(1<<PinNumber);
	else //write 0 to that bit
		pGPIOx->ODR|=(1<<PinNumber);
}
void GPIO_WriteToOutputPort(GPIO_RegDef_t *pGPIOx, uint16_t Value){
	pGPIOx->ODR=Value;
}
void GPIO_ToggleOutputPin(GPIO_RegDef_t *pGPIOx, uint8_t PinNumber){
	pGPIOx->ODR ^=(1<<PinNumber);
}

/*
 * IRQ Configuration and ISR handling
 */
void GPIO_IRQInterruptConfig(uint8_t IRQNumber, uint8_t EnorDi){
	if(EnorDi == ENABLE)
		{
			if(IRQNumber <= 31)
			{
				//program ISER0 register
				*NVIC_ISER0 |= ( 1 << IRQNumber );

			}else if(IRQNumber > 31 && IRQNumber < 64 ) //32 to 63
			{
				//program ISER1 register
				*NVIC_ISER1 |= ( 1 << (IRQNumber % 32) );
			}
			else if(IRQNumber >= 64 && IRQNumber < 96 )
			{
				//program ISER2 register //64 to 95
				*NVIC_ISER2 |= ( 1 << (IRQNumber % 64) );
			}
		}else
		{
			if(IRQNumber <= 31)
			{
				//program ICER0 register
				*NVIC_ICER0 |= ( 1 << IRQNumber );
			}else if(IRQNumber > 31 && IRQNumber < 64 )
			{
				//program ICER1 register
				*NVIC_ICER1 |= ( 1 << (IRQNumber % 32) );
			}
			else if(IRQNumber >= 64 && IRQNumber < 96 )
			{
				//program ICER2 register
				*NVIC_ICER2 |= ( 1 << (IRQNumber % 64) );
			}
		}
}
/*********************************************************************
 * @fn      		  - SPI_IRQPriorityConfig
 *
 * @brief             -
 *
 * @param[in]         -
 * @param[in]         -
 * @param[in]         -
 *
 * @return            -
 *
 * @Note              -
 */
void GPIO_IRQPriorityConfig(uint8_t IRQNumber,uint32_t IRQPriority)
{
	//1. first lets find out the ipr register
	uint8_t iprx = IRQNumber / 4;
	uint8_t iprx_section  = IRQNumber %4 ;

	uint8_t shift_amount = ( 8 * iprx_section) + ( 8 - NO_PR_BITS_IMPLEMENTED) ;

	*(  NVIC_PR_BASE_ADDR + iprx ) |=  ( IRQPriority << shift_amount );

}

/*********************************************************************
 * @fn      		  - GPIO_IRQHandling
 *
 * @brief             -
 *
 * @param[in]         -
 * @param[in]         -
 * @param[in]         -
 *
 * @return            -
 *
 * @Note              -
 */
void GPIO_IRQHandling(uint8_t PinNumber)
{
	//clear the exti pr register corresponding to the pin number
	if(EXTI->PR & ( 1 << PinNumber))
	{
		//clear
		EXTI->PR |= ( 1 << PinNumber);
	}

}
