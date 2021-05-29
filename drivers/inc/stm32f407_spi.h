/*
 * stm32407_spi.h
 *
 *  Created on: May 17, 2021
 *      Author: chira
 */

#ifndef INC_STM32F407_SPI_H_
#define INC_STM32F407_SPI_H_

#include "stm32f407.h"
//HANDLE STRUCTURE
typedef struct{
	uint8_t SPI_DeviceMode; //@SPI_MODES
	uint8_t SPI_BusConfig; //@BUS CONFIG
	uint8_t SPI_DFF;      //@DATA FRAME
	uint8_t SPI_CPHA;    //@CLOCK PHASE
	uint8_t SPI_SclkSpeed; //@SERIAL CLK SPEEDS
	uint8_t SPI_CPOL;     //@CLOCK POLARITY
	uint8_t SPI_SSM;     //@SOFTWARE/HARDWARE SLAVE MGMT

}SPI_Config_t;

//CONFIGURATION STRUCTURE
typedef struct{
	SPI_RegDef_t *pSPIx;
	SPI_Config_t SPIConfig;

}SPI_Handle_t;

//----------------------------------------------------------------------

//@SPI_MODES
#define SPI_DEVICE_MODE_MASTER 1
#define SPI_DEVICE_MODE_SLAVE  0


//@BUS CONFIG
#define SPI_BUS_CONFIG_FD                1
#define SPI_BUS_CONFIG_HD                2
#define SPI_BUS_CONFIG_SIMPLEX_RXONLY    3


//@SERIAL CLK SPEEDS

#define SPI_SCLK_SPEED_DIV2             	0
#define SPI_SCLK_SPEED_DIV4             	1
#define SPI_SCLK_SPEED_DIV8             	2
#define SPI_SCLK_SPEED_DIV16             	3
#define SPI_SCLK_SPEED_DIV32             	4
#define SPI_SCLK_SPEED_DIV64             	5
#define SPI_SCLK_SPEED_DIV128             	6
#define SPI_SCLK_SPEED_DIV256             	7


//@DATA FRAME RATE
#define SPI_DFF_8BITS 	0
#define SPI_DFF_16BITS  1

//@CLOCK POLARITY
#define SPI_CPOL_HIGH 1
#define SPI_CPOL_LOW 0

//@CLOCK PHASE
#define SPI_CPHA_HIGH 1
#define SPI_CPHA_LOW 0

//@SOFTWARE/HARDWARE SLAVE MGMT
#define SPI_SSM_EN     1
#define SPI_SSM_DI     0

/*
 * Bit position definitions SPI_SR
 */
#define SPI_SR_RXNE						0
#define SPI_SR_TXE				 		1
#define SPI_SR_CHSIDE				 	2
#define SPI_SR_UDR					 	3
#define SPI_SR_CRCERR				 	4
#define SPI_SR_MODF					 	5
#define SPI_SR_OVR					 	6
#define SPI_SR_BSY					 	7
#define SPI_SR_FRE					 	8


/*
 * SPI related status flags definitions
 */
#define SPI_TXE_FLAG    ( 1 << SPI_SR_TXE)
#define SPI_RXNE_FLAG   ( 1 << SPI_SR_RXNE)
#define SPI_BUSY_FLAG   ( 1 << SPI_SR_BSY)

/******************************************************************************************
 *								APIs supported by this driver
 *		 For more information about the APIs check the function definitions
 ******************************************************************************************/
/*
 * Peripheral Clock setup
 */
void SPI_PeriClockControl(SPI_RegDef_t *pSPIx, uint8_t EnorDi);

/*
 * Init and De-init
 */
void SPI_Init(SPI_Handle_t *pSPIHandle);
void SPI_DeInit(SPI_RegDef_t *pSPIx);


/*
 * Data Send and Receive
 */
void SPI_SendData(SPI_RegDef_t *pSPIx,uint8_t *pTxBuffer, uint32_t Len);
void SPI_ReceiveData(SPI_RegDef_t *pSPIx, uint8_t *pRxBuffer, uint32_t Len);


/*
 * IRQ Configuration and ISR handling
 */
void SPI_IRQInterruptConfig(uint8_t IRQNumber, uint8_t EnorDi);
void SPI_IRQPriorityConfig(uint8_t IRQNumber, uint32_t IRQPriority);
void SPI_IRQHandling(SPI_Handle_t *pHandle);

/*
 * Other Peripheral Control APIs
 */

void SPI_PeripheralControl(SPI_RegDef_t *pSPIx, uint8_t EnOrDi);
void  SPI_SSOEConfig(SPI_RegDef_t *pSPIx, uint8_t EnOrDi);
void  SPI_SSIConfig(SPI_RegDef_t *pSPIx, uint8_t EnOrDi);
uint8_t SPI_GetFlagStatus(SPI_RegDef_t *pSPIx , uint32_t FlagName);

#endif /* INC_STM32F407_SPI_H_ */
