/*
 * stm32f407_spi.c
 *
 *  Created on: May 17, 2021
 *      Author: chira
 */



#include "stm32f407_spi.h"

/*
 * Peripheral Clock setup
 */
void SPI_PeriClockControl(SPI_RegDef_t *pSPIx, uint8_t EnorDi){
	if(EnorDi==ENABLE){
			if(pSPIx==SPI1)SPI1_PCLK_EN();
			else if(pSPIx==SPI2)SPI2_PCLK_EN();
			else if(pSPIx==SPI3)SPI3_PCLK_EN();


		}
		else {
			//TODO REPLACE WITH SPI DISABLE CLOCK MACROS
			/*if(pSPIx==SPI1)SPI1_PCLK_DI();
			else if(pSPIx==SPI2)SPI2_PCLK_DI();
			else if(pSPIx==SPI3)SPI3_PCLK_DI();
*/
		}
}

/*
 * Init and De-init
 */
void SPI_Init(SPI_Handle_t *pSPIHandle)
{

	//peripheral clock enable

	SPI_PeriClockControl(pSPIHandle->pSPIx, ENABLE);

	//first lets configure the SPI_CR1 register

	uint32_t tempreg = 0;

	//1. configure the device mode
	tempreg |= pSPIHandle->SPIConfig.SPI_DeviceMode << SPI_CR1_MSTR ;

	//2. Configure the bus config
	if(pSPIHandle->SPIConfig.SPI_BusConfig == SPI_BUS_CONFIG_FD)
	{
		//bidi mode should be cleared
		tempreg &= ~( 1 << SPI_CR1_BIDIMODE);

	}else if (pSPIHandle->SPIConfig.SPI_BusConfig == SPI_BUS_CONFIG_HD)
	{
		//bidi mode should be set
		tempreg |= ( 1 << SPI_CR1_BIDIMODE);
	}else if (pSPIHandle->SPIConfig.SPI_BusConfig == SPI_BUS_CONFIG_SIMPLEX_RXONLY)
	{
		//BIDI mode should be cleared
		tempreg &= ~( 1 << SPI_CR1_BIDIMODE);
		//RXONLY bit must be set
		tempreg |= ( 1 << SPI_CR1_RXONLY);
	}

	// 3. Configure the spi serial clock speed (baud rate)
	tempreg |= pSPIHandle->SPIConfig.SPI_SclkSpeed << SPI_CR1_BR;

	//4.  Configure the DFF
	tempreg |= pSPIHandle->SPIConfig.SPI_DFF << SPI_CR1_DFF;

	//5. configure the CPOL
	tempreg |= pSPIHandle->SPIConfig.SPI_CPOL << SPI_CR1_CPOL;

	//6 . configure the CPHA
	tempreg |= pSPIHandle->SPIConfig.SPI_CPHA << SPI_CR1_CPHA;

	tempreg |= pSPIHandle->SPIConfig.SPI_SSM << SPI_CR1_SSM;

	pSPIHandle->pSPIx->CR1 = tempreg;

}
void SPI_DeInit(SPI_RegDef_t *pSPIx){

}


/*
 * Data Send and Receive
 */

//blocking call
void SPI_SendData(SPI_RegDef_t *pSPIx,uint8_t *pTxBuffer, uint32_t Len){
	while (Len >0){
		//wait until the TXE bit is set indicating the buffer is full
		while(pSPIx->SR & (1<<1));
		//check the data frame format ie 8 or 16 bit
		if(pSPIx->CR1 & (1<<SPI_CR1_DFF)){
			//16 BIT FORMAT
			pSPIx->DR = *((uint16_t *)pTxBuffer);
			Len--;
			Len--; //reducing length by 16 bits
			//now after reading data we must point to next address to fetch data
			(uint16_t)pTxBuffer++;
		}
		else{
			//8 bit data format
			pSPIx->DR =*pTxBuffer;
			Len--;
			pTxBuffer++;
		}
	}
}
void SPI_ReceiveData(SPI_RegDef_t *pSPIx, uint8_t *pRxBuffer, uint32_t Len){
	while (Len >0){
			//wait until the TXE bit is set indicating the buffer is full
			while(pSPIx->SR & (1<<1));
			//check the data frame format ie 8 or 16 bit
			if(pSPIx->CR1 & (1<<SPI_CR1_DFF)){
				//16 BIT FORMAT

				//writing data to data reg
				  *((uint16_t *)pRxBuffer)=pSPIx->DR;

				Len--;
				Len--; //reducing length by 16 bits
				//now after reading data we must point to next address to fetch data
				(uint16_t)pRxBuffer++;
			}
			else{
				//8 bit data format
				 *pRxBuffer=pSPIx->DR;
				Len--;
				pRxBuffer++;
			}
		}
}


/*
 * IRQ Configuration and ISR handling
 */
void SPI_IRQInterruptConfig(uint8_t IRQNumber, uint8_t EnorDi);
void SPI_IRQPriorityConfig(uint8_t IRQNumber, uint32_t IRQPriority);
void SPI_IRQHandling(SPI_Handle_t *pHandle);


/*
 * other apis
 */

void SPI_PeripheralControl(SPI_RegDef_t *pSPIx, uint8_t EnOrDi)
{
	if(EnOrDi == ENABLE)
	{
		pSPIx->CR1 |=  (1 << SPI_CR1_SPE);
	}else
	{
		pSPIx->CR1 &=  ~(1 << SPI_CR1_SPE);
	}


}



/*********************************************************************
 * @fn      		  - SPI_SSIConfig
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
void  SPI_SSIConfig(SPI_RegDef_t *pSPIx, uint8_t EnOrDi)
{
	if(EnOrDi == ENABLE)
	{
		pSPIx->CR1 |=  (1 << SPI_CR1_SSI);
	}else
	{
		pSPIx->CR1 &=  ~(1 << SPI_CR1_SSI);
	}


}

/*********************************************************************
 * @fn      		  - SPI_SSIConfig
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
void  SPI_SSOEConfig(SPI_RegDef_t *pSPIx, uint8_t EnOrDi)
{
	if(EnOrDi == ENABLE)
	{
		pSPIx->CR2 |=  (1 << SPI_CR2_SSOE);
	}else
	{
		pSPIx->CR2 &=  ~(1 << SPI_CR2_SSOE);
	}


}

/*********************************************************************
 * @fn      		  - SPI_SSIConfig
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
uint8_t SPI_GetFlagStatus(SPI_RegDef_t *pSPIx , uint32_t FlagName)
{
	if(pSPIx->SR & FlagName)
	{
		return 1;
	}
	return 0;
}








