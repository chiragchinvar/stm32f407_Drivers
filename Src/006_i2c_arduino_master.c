/*
 * 006_i2c_arduino_master.c
 *
 *  Created on: May 24, 2021
 *      Author: chira
 */

#include<string.h>
#include "stm32f407.h"

char some_data[]="hello wolrd";

void delay(void)
{
	for(uint32_t i = 0 ; i < 500000 ; i ++);
}

/*
 * PB6 - I2C1 SCL
 * PB7 - I2C1 SDA
 * Alternate functionality mode 4
 */



void I2C1_GPIOInits(void)
{
	GPIO_Handle_t I2CPins;

	I2CPins.pGPIOx = GPIOB;
	I2CPins.GPIO_PinConfig.GPIO_PinMode = GPIO_MODE_ALTFN;
	I2CPins.GPIO_PinConfig.GPIO_PinAltFunMode = 4;
	I2CPins.GPIO_PinConfig.GPIO_PinOPType = GPIO_OP_TYPE_OD;
	I2CPins.GPIO_PinConfig.GPIO_PinPuPdControl = GPIO_PIN_PU;
	I2CPins.GPIO_PinConfig.GPIO_PinSpeed = GPIO_SPEED_FAST;

	//SCL
	I2CPins.GPIO_PinConfig.GPIO_PinNumber = GPIO_PIN_NO_6;
	GPIO_Init(&I2CPins);

	//SDA
	I2CPins.GPIO_PinConfig.GPIO_PinNumber = GPIO_PIN_NO_7;
	GPIO_Init(&I2CPins);


}

void I2C_Inits(void)
{

I2C_Handle_t  I2C1handle;

I2C1handle.pI2Cx=I2C1;
I2C1handle.I2C_Config.I2C_AckControl=I2C_ACK_ENABLE;
I2C1handle.I2C_Config.I2C_DeviceAddress=0x60;
I2C1handle.I2C_Config.I2C_FMDutyCycle=I2C_FM_DUTY_2;
I2C1handle.I2C_Config.I2C_SCLSpeed=I2C_SCL_SPEED_SM;



I2C_Init(&I2C1handle);
}

void GPIO_ButtonInit(void)
{
	GPIO_Handle_t GPIOBtn;

	//this is btn gpio configuration
	GPIOBtn.pGPIOx = GPIOA;
	GPIOBtn.GPIO_PinConfig.GPIO_PinNumber = GPIO_PIN_NO_0;
	GPIOBtn.GPIO_PinConfig.GPIO_PinMode = GPIO_MODE_IN;
	GPIOBtn.GPIO_PinConfig.GPIO_PinSpeed = GPIO_SPEED_FAST;
	GPIOBtn.GPIO_PinConfig.GPIO_PinPuPdControl = GPIO_NO_PUPD;

	GPIO_Init(&GPIOBtn);

}





void main(){
	//initialising the gpio as alt fun, button and i2c

	I2C1_GPIOInits();
	GPIO_ButtonInit();
	I2C_Inits();

	//enable the i2c peripheral
		I2C_PeripheralControl(I2C1,ENABLE);

		while(1){

			while( ! (GPIO_ReadFromInputPin(GPIOA, GPIO_PIN_NO_0)));

			//SEND DATA
			delay();

			I2C_MasterSendData(&I2C1handle,some_data,strlen((char*)some_data),0x68,0);
		}



}

