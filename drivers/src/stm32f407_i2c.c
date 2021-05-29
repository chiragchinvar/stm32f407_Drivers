/*
 * stm32f407_i2c.c
 *
 *  Created on: May 23, 2021
 *      Author: chira
 */

#include "stm32f407_i2c.h"
uint32_t abh1p[]={2,4,8,16,64,128,256};
uint32_t apb1p[]={2,4,8,16};
static void I2C_ExecuteAddressPhaseWrite(I2C_RegDef_t *pI2Cx, uint8_t SlaveAddr);
static void I2C_ClearADDRFlag(I2C_Handle_t *pI2CHandle );

static void I2C_ExecuteAddressPhaseWrite(I2C_RegDef_t *pI2Cx, uint8_t SlaveAddr){
	SlaveAddr = SlaveAddr <<1 ;
	SlaveAddr &= ~(1);
	pI2Cx->I2C_DR=SlaveAddr;

}
static void I2C_ClearADDRFlag(I2C_Handle_t *pI2CHandle ){
	/*uint32_t dummyRead = pI2CHandle->SR1;
	dummyRead = pI2CHandle->SR2;
	(void)dummyRead;
*/
}
/*
 * I2C CLOCK CALCULATION FUNCTION
 */

uint32_t rcc_i2cclk(void){
	uint32_t pclk1,sysclk,temp,ahb1,apb1;
	uint8_t clksrc;

	//get the clk source
	clksrc=(RCC->CFGR>>2) & 0x3;
	if(clksrc==0) sysclk=16000000;
	else if(clksrc==1) sysclk=8000000;
	else if(clksrc==2);//calculate freq

	//fetch the ahb prescaler becuase the clock is fed to ahb prescaler and then to apb prescaler
	temp=(RCC->CFGR >> 4) & 0XF;
	if (temp<8) ahb1=0;
	else ahb1=abh1p[temp-8];

	//fetch the apb prescaler
	temp=0;
	temp=(RCC->CFGR >> 10) & (0x7);
	if (temp<4)apb1=0;
	else apb1=apb1p[temp-4];


	//calculate final clock
	pclk1= (sysclk/ahb1)/apb1;

	return pclk1;
}

/******************************************************************************************
 *								APIs supported by this driver
 *		 For more information about the APIs check the function definitions
 ******************************************************************************************/
/*
 * Peripheral Clock setup
 */
void I2C_PeriClockControl(I2C_RegDef_t *pI2Cx, uint8_t EnorDi){
if(EnorDi==ENABLE){
	if(pI2Cx==I2C1)I2C1_PCLK_EN();
				else if(pI2Cx==I2C2)I2C2_PCLK_EN();
				else if(pI2Cx==I2C3)I2C3_PCLK_EN();
				else {
					//call peripheral clock disable macros
				}

}
}
/*
 *
 * Init and De-init
 */
void I2C_Init(I2C_Handle_t *pI2CHandle){
	uint32_t temp=0;

	//enabling ACK control
	temp|=(1<<10);

	//configure freq field
	temp=0;
	temp |= rcc_i2cclk()/1000000;//dividing by 1mhz becuase value should be direct not in mhz
    pI2CHandle->pI2Cx->I2C_CR2 = ( temp& 0x3f);

    //configure slave address
    temp=0;
    temp|=(1<<14);//according to ref manual 14th bit should always be 1
    temp= pI2CHandle->I2C_Config.I2C_DeviceAddress;
    temp|=(pI2CHandle->pI2Cx->I2C_OAR1 <<1);

    //configure the ccr register for standard and fast mode
    uint16_t ccr_value=0;
    temp=0;

    if(pI2CHandle->I2C_Config.I2C_SCLSpeed <= I2C_SCL_SPEED_SM){
    	//STANDARD MODE CONFIGURATIONS
    	ccr_value=(rcc_i2cclk() /(2* pI2CHandle->I2C_Config.I2C_SCLSpeed));
        temp|=(ccr_value & 0xFFF);

    }
    else {
    	temp|= (1<<15); //configuring the mode bit to fast mode;
    	temp|=(pI2CHandle->I2C_Config.I2C_FMDutyCycle << 14);
    	if(pI2CHandle->I2C_Config.I2C_FMDutyCycle == I2C_FM_DUTY_2){
    		ccr_value = (rcc_i2cclk() /(3* pI2CHandle->I2C_Config.I2C_SCLSpeed));
    	}
    	else{
    		ccr_value = (rcc_i2cclk() /(25* pI2CHandle->I2C_Config.I2C_SCLSpeed));
    	}
    	temp|=ccr_value;

    }
    pI2CHandle->pI2Cx->I2C_CCR = temp;


    //configure trise
    if(pI2CHandle->I2C_Config.I2C_SCLSpeed==I2C_SCL_SPEED_SM){
    	//for standard mode multiply by 1000ns
    	temp=0;
    	temp= ((rcc_i2cclk()/1000000)+1);

    }
    else{
    	//refer datasheet for calculations
    	temp=0;
    	temp= ((((rcc_i2cclk())*300)/1000000000)+1);

    }

    pI2CHandle->pI2Cx->I2C_TRISE =temp & 0x3f;

    }
void I2C_DeInit(I2C_RegDef_t *pI2Cx);

/*
 * flag check reg
 */
uint8_t I2C_GetFlagStatus(I2C_RegDef_t *pI2Cx , uint32_t FlagName)
{
	if(pI2Cx->I2C_SR1 & FlagName)
	{
		return FLAG_SET;
	}
	return FLAG_RESET;
}

/*
 * Data Send and Receive
 */
void I2C_MasterSendData(I2C_Handle_t *pI2CHandle,uint8_t *pTxbuffer, uint32_t Len, uint8_t SlaveAddr,uint8_t Sr){
	uint32_t temp;

	//1. have to write 1 to start bit in cr1 reg to start i2c comm
     pI2CHandle->pI2Cx->I2C_CR1 |= (1<<8);

     //2.wait until the SB flag is set in SR1 REG
    while(!( I2C_GetFlagStatus(pI2CHandle->pI2Cx , I2C_FLAG_SB)));

    //3. write the slave addr into DR
    I2C_ExecuteAddressPhaseWrite(pI2CHandle->pI2Cx, SlaveAddr);

    //wait until addr flag in set in SR1
    while(!( I2C_GetFlagStatus(pI2CHandle->pI2Cx , I2C_FLAG_ADDR)));

    //clear the addr flag else SCL will always be low
    I2C_ClearADDRFlag(pI2CHandle);

    //WRITE DATA TO DR UNTIL LENGTH IS 0

    while(Len>0){
    	//WAIT UNTIL TXE FLAG IS SET
    	while(!(I2C_GetFlagStatus(pI2CHandle->pI2Cx , I2C_FLAG_TXE)));
        pI2CHandle->pI2Cx->I2C_DR = *pTxbuffer;
        pTxbuffer++;
        Len--;
    }

    //wait until txe flag and btf flag is set to stop
    while(!( I2C_GetFlagStatus(pI2CHandle->pI2Cx , I2C_FLAG_TXE)));
    while(!( I2C_GetFlagStatus(pI2CHandle->pI2Cx , I2C_FLAG_BTF)));


    //GENERATE STOP CONDITION
    pI2CHandle->pI2Cx->I2C_CR1 |= (1<<9);





}
void I2C_MasterReceiveData(I2C_Handle_t *pI2CHandle,uint8_t *pRxBuffer, uint8_t Len, uint8_t SlaveAddr,uint8_t Sr);
uint8_t I2C_MasterSendDataIT(I2C_Handle_t *pI2CHandle,uint8_t *pTxbuffer, uint32_t Len, uint8_t SlaveAddr,uint8_t Sr);
uint8_t I2C_MasterReceiveDataIT(I2C_Handle_t *pI2CHandle,uint8_t *pRxBuffer, uint8_t Len, uint8_t SlaveAddr,uint8_t Sr);

void I2C_CloseReceiveData(I2C_Handle_t *pI2CHandle);
void I2C_CloseSendData(I2C_Handle_t *pI2CHandle);


void I2C_SlaveSendData(I2C_RegDef_t *pI2C,uint8_t data);
uint8_t I2C_SlaveReceiveData(I2C_RegDef_t *pI2C);

/*
 * IRQ Configuration and ISR handling
 */
void I2C_IRQInterruptConfig(uint8_t IRQNumber, uint8_t EnorDi);
void I2C_IRQPriorityConfig(uint8_t IRQNumber, uint32_t IRQPriority);
void I2C_EV_IRQHandling(I2C_Handle_t *pI2CHandle);
void I2C_ER_IRQHandling(I2C_Handle_t *pI2CHandle);


