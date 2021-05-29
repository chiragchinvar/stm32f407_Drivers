/*
 * 004_SPI_Tx.c
 *
 *  Created on: May 18, 2021
 *      Author: chira
 */
#include "stm32f407.h"
#include <string.h>

uint32_t *gpiob=(uint32_t *)(0x40020400);
uint32_t *RCC_AHB1ENR = (uint32_t *)(0x40023800+0x30);


void SPI2_GPIOInit(){
	/*PB14-SPI2 MISO
		 * PB15-SPI2 MOSI
		 * PB13 - SCLK
		 * PB12 -NSS
		 * ALT FUNCTION MODE =5
		 */
//turning clock of gpiob

	 *RCC_AHB1ENR |= (1<<3);

	//setting all the above pins as alternate functionality mode
	*gpiob|=(3<<28);
	*gpiob|=(3<<30);
	*gpiob|=(3<<26);
	*gpiob|=(3<<24);


	//setting the gpiob as push pull config
	*(gpiob+0x04)|=(3<<24);

	//setting alternate functionality high register
	*(gpiob+0x24) |= (5<<16);
	*(gpiob+0x24) |= (5<<20);
	*(gpiob+0x24) |= (5<<24);
	*(gpiob+0x24) |= (5<<28);




}
void SPI2_INIT(){

	SPI_Handle_t SPI2handle;
	SPI2handle.pSPIx=SPI2_BASEADDR;
	SPI2handle.SPIConfig.SPI_BusConfig=SPI_BUS_CONFIG_FD;
	SPI2handle.SPIConfig.SPI_CPHA=SPI_CPHA_LOW;
	SPI2handle.SPIConfig.SPI_CPOL=SPI_CPOL_LOW;
	SPI2handle.SPIConfig.SPI_DFF=SPI_DFF_8BITS;
	SPI2handle.SPIConfig.SPI_SSM=SPI_SSM_EN;
	SPI2handle.SPIConfig.SPI_DeviceMode=SPI_DEVICE_MODE_MASTER;
	SPI2handle.SPIConfig.SPI_SclkSpeed=SPI_SCLK_SPEED_DIV2;

	SPI_Init(&SPI2handle);



}
int main(){
	/*PB14-SPI2 MISO
	 * PB15-SPI2 MOSI
	 * PB13 - SCLK
	 * PB12 -NSS
	 * ALT FUNCTION MODE =5
	 */
	char user_data[]="hello world";

	SPI2_GPIOInit();
	SPI2_INIT();

	//SENDING DATA
	SPI_SendData(SPI2,(uint8_t*)user_data,strlen(user_data));

	//enabling spi peripheral which is disabled by default
	//all settings must be done while the SPI peripheral is enabled
	SPI_PeripheralControl(SPI2, ENABLE);

	//while using ssm=1 we should enable ssi bit
	SPI_SSIConfig(SPI2,ENABLE);

	//Disable the SPI2 peripheral
	SPI_PeripheralControl(SPI2,DISABLE);

	return 0;
}
