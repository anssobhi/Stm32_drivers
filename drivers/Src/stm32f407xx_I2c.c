/*
 * stm32f407xx_I2c.c
 *
 *  Created on: ١٧‏/٠٢‏/٢٠٢٢
 *      Author: ans
 */

#include "stm32f407xx_I2c.h"



static void I2C_GenerateStartCondition(I2C_RegDef_t *pI2Cx);
static void I2C_ExecuteAddressPhaseSend(I2C_RegDef_t *pI2Cx,uint8_t SlaveAddr);
static void I2C_ExecuteAddressPhaseReceive(I2C_RegDef_t *pI2Cx,uint8_t SlaveAddr);
static void I2C_ClearADDRFlag(I2C_Handle_t *pI2CHandle);
static void I2C_MasterHandleTXEInterrupt(I2C_Handle_t *pI2CHandle);
static void I2C_MasterHandleRXNEInterrupt(I2C_Handle_t *pI2CHandle);

static void I2C_GenerateStartCondition(I2C_RegDef_t *pI2Cx)
{
	pI2Cx->CR1 |= (1 << I2C_CR1_START);
}

static void I2C_ExecuteAddressPhaseSend(I2C_RegDef_t *pI2Cx,uint8_t SlaveAddr)
{
	SlaveAddr = SlaveAddr << 1;
	SlaveAddr &= ~(1);
	pI2Cx->DR = SlaveAddr;
}
static void I2C_ClearADDRFlag(I2C_Handle_t *pI2CHandle)
{
	uint8_t dummy_read;
	if(pI2CHandle->pI2Cx->SR2 & (1 << I2C_SR2_MSL))
	{
		if(pI2CHandle->TxRxState == I2C_BUSY_IN_RX)
		{
			if(pI2CHandle->RxSize == 1)
			{
				I2C_MangeAck(pI2CHandle->pI2Cx,DISABLE);
				dummy_read = pI2CHandle->pI2Cx->SR1;
				dummy_read = pI2CHandle->pI2Cx->SR2;
				(void)dummy_read;
			}
		}else
		{
			dummy_read = pI2CHandle->pI2Cx->SR1;
			dummy_read = pI2CHandle->pI2Cx->SR2;
			(void)dummy_read;
		}
	}else
	{
		dummy_read = pI2CHandle->pI2Cx->SR1;
		dummy_read = pI2CHandle->pI2Cx->SR2;
		(void)dummy_read;
	}
}

void I2C_GenerateStopCondition(I2C_RegDef_t *pI2Cx)
{
	pI2Cx->CR1 |= (1<<I2C_CR1_STOP);
}

static void I2C_ExecuteAddressPhaseReceive(I2C_RegDef_t *pI2Cx,uint8_t SlaveAddr)
{
	SlaveAddr = SlaveAddr << 1;
	SlaveAddr |=(1);
	pI2Cx->DR = SlaveAddr;
}

void I2C_PerClockControl(I2C_RegDef_t *pI2Cx, uint8_t EnorDi)
{
	if(EnorDi == ENABLE)
	{
	if(pI2Cx == I2C1)
	{
		I2C1_PCLK_EN();
	}
	else if(pI2Cx == I2C2)
	{
		I2C2_PCLK_EN();
	}
	else if(pI2Cx == I2C3)
	{
		I2C3_PCLK_EN();
	}
	}else
	{
    if(pI2Cx == I2C1)
	{
	      I2C1_PCLK_DI();
	}
	else if(pI2Cx == I2C2)
	{
	      I2C2_PCLK_DI();
	}
	else if(pI2Cx == I2C3)
	{
	      I2C3_PCLK_DI();
	}
	}
}

/*
 * this for intialization
 */


void I2C_Inti(I2C_Handle_t *pI2CHandle)
{
	uint32_t  tempreg =0;

	I2C_PerClockControl(pI2CHandle->pI2Cx , ENABLE);

	tempreg |= pI2CHandle->I2C_Config.I2C_ACKControl << 10;
	pI2CHandle->pI2Cx->CR1 = tempreg;

	//configure the FRQ field
	tempreg = 0;
	tempreg |= RCC_GetPCLK1Value() / 1000000U;
	pI2CHandle->pI2Cx->CR2 = (tempreg & 0x3F);

	// configure the device own address
	tempreg =0;
	tempreg |= pI2CHandle->I2C_Config.I2C_DeviceAddress << 1;
	tempreg |= (1<<14);
	pI2CHandle->pI2Cx->OAR1 = tempreg;

	//CCR calculation
	uint16_t ccr_value = 0;
	tempreg = 0;
	if(pI2CHandle->I2C_Config.I2C_SCLSpeed <= I2C_SCL_SPEED_SM)
	{
		ccr_value = (RCC_GetPCLK1Value() / (2*pI2CHandle->I2C_Config.I2C_SCLSpeed));
		tempreg |= (ccr_value & 0xFFF);
	}else
	{
		tempreg |= (1<<15);
		tempreg |= (pI2CHandle->I2C_Config.I2C_FMDutyCycle <<14);

		if(pI2CHandle->I2C_Config.I2C_FMDutyCycle == I2C_FM_DUTY_2)
		{
			ccr_value = (RCC_GetPCLK1Value() / (3*pI2CHandle->I2C_Config.I2C_SCLSpeed));
		}else
		{
			ccr_value = (RCC_GetPCLK1Value() / (25*pI2CHandle->I2C_Config.I2C_SCLSpeed));
		}
		tempreg |= (ccr_value & 0xFFF);
	}
	pI2CHandle->pI2Cx->CCR = tempreg;

	if(pI2CHandle->I2C_Config.I2C_SCLSpeed <= I2C_SCL_SPEED_SM)
	{
		tempreg = (RCC_GetPCLK1Value()/1000000U) + 1;
	}else
	{
		tempreg = ((RCC_GetPCLK1Value()* 300) / 1000000U)+1;
	}
	pI2CHandle->pI2Cx->TRISE = (tempreg & 0x3F);
}
void I2C_DeInti(I2C_RegDef_t *pI2Cx)
{

}

void I2C_MasterSendData(I2C_Handle_t *pI2CHandle,uint8_t *pTxbuffer,uint32_t len,uint8_t SlaveAddr,uint8_t Sr)
{
	// 1. generate start condition
	I2C_GenerateStartCondition(pI2CHandle->pI2Cx);

	// 2. confirm that start condition is completed by checking the SB flag in the SR1
	while( ! I2C_GetFlagStatus(pI2CHandle->pI2Cx , I2C_FLAG_SB) );

	// 3. send the slave address and the operation
	I2C_ExecuteAddressPhaseSend(pI2CHandle->pI2Cx,SlaveAddr);

	// 4. confirm that address phase is completed by checking the ADDR Flag
	while( ! I2C_GetFlagStatus(pI2CHandle->pI2Cx , I2C_FLAG_ADDR) );

	// 5. clear the ADDR flag according to its software sequence
	I2C_ClearADDRFlag(pI2CHandle);

	//.6 send data
	while(len > 0)
	{
		while(! I2C_GetFlagStatus(pI2CHandle->pI2Cx, I2C_FLAG_TXE));
		pI2CHandle->pI2Cx->DR = *pTxbuffer;
		pTxbuffer++;
		len--;
	}
	// 7. when len is zero wait for TXE = 1 and BTF = 1 before generate stop cond
	while(! I2C_GetFlagStatus(pI2CHandle->pI2Cx, I2C_FLAG_TXE));
	while(! I2C_GetFlagStatus(pI2CHandle->pI2Cx, I2C_FLAG_BTF));

	if(Sr == I2C_DIABLE_SR)
	{
	I2C_GenerateStopCondition(pI2CHandle->pI2Cx);
	}

}

void I2C_MasterReceiveData(I2C_Handle_t *pI2CHandle,uint8_t *pRxbuffer,uint32_t len,uint8_t SlaveAddr, uint8_t Sr)
{
	// 1. generate start condition
	I2C_GenerateStartCondition(pI2CHandle->pI2Cx);

	// 2. confirm that start condition is completed by checking the SB flag in the SR1
	while( ! I2C_GetFlagStatus(pI2CHandle->pI2Cx , I2C_FLAG_SB) );

	// 3. send the slave address and the operation
	I2C_ExecuteAddressPhaseReceive(pI2CHandle->pI2Cx,SlaveAddr);

	// 4. confirm that address phase is completed by checking the ADDR Flag
	while( ! I2C_GetFlagStatus(pI2CHandle->pI2Cx , I2C_FLAG_ADDR) );

	if(len == 1)
	{
		pI2CHandle->pI2Cx->CR1 &= ~(1<<10);
		I2C_ClearADDRFlag(pI2CHandle);
		while(! I2C_GetFlagStatus(pI2CHandle->pI2Cx, I2C_FLAG_RXNE));
		if(Sr == I2C_DIABLE_SR)
		{
			I2C_GenerateStopCondition(pI2CHandle->pI2Cx);
		}
		*pRxbuffer = pI2CHandle->pI2Cx->DR;
	}

	//.6 Receive data
	if(len > 1)
	{
		I2C_ClearADDRFlag(pI2CHandle);
	for(uint32_t i = len;i>0;i--)
	{
		while(! I2C_GetFlagStatus(pI2CHandle->pI2Cx, I2C_FLAG_RXNE));
		if(i == 2)
		{
			pI2CHandle->pI2Cx->CR1 &= ~(1<<10);
			if(Sr == I2C_DIABLE_SR)
			{
				I2C_GenerateStopCondition(pI2CHandle->pI2Cx);
			}
		}
		*pRxbuffer = pI2CHandle->pI2Cx->DR;
		pRxbuffer++;
	}
	}
	if(pI2CHandle->I2C_Config.I2C_ACKControl == I2C_ACK_ENABLE)
	{
	pI2CHandle->pI2Cx->CR1 |= (1<<10);
	}
}


uint8_t I2C_MasterSendDataIT(I2C_Handle_t *pI2CHandle,uint8_t *pTxbuffer,uint32_t len,uint8_t SlaveAddr, uint8_t Sr)
{
	uint8_t busystate = pI2CHandle->TxRxState;

		if( (busystate != I2C_BUSY_IN_TX) && (busystate != I2C_BUSY_IN_RX))
		{
			pI2CHandle->pTxBuffer = pTxbuffer;
			pI2CHandle->TxLen = len;
			pI2CHandle->TxRxState = I2C_BUSY_IN_TX;
			pI2CHandle->DevAddr = SlaveAddr;
			pI2CHandle->Sr = Sr;

			//Implement code to Generate START Condition

			I2C_GenerateStartCondition(pI2CHandle->pI2Cx);
			//Implement the code to enable ITBUFEN Control Bit
			pI2CHandle->pI2Cx->CR2 |= ( 1 << I2C_CR2_ITBUFEN);

			//Implement the code to enable ITEVFEN Control Bit
			pI2CHandle->pI2Cx->CR2 |= ( 1 << I2C_CR2_ITEVTEN);

			//Implement the code to enable ITERREN Control Bit
			pI2CHandle->pI2Cx->CR2 |= ( 1 << I2C_CR2_ITERREN );

		}

		return busystate;
}
uint8_t I2C_MasterReceiveDataIT(I2C_Handle_t *pI2CHandle,uint8_t *pRxbuffer,uint32_t len,uint8_t SlaveAddr, uint8_t Sr)
{
	uint8_t busystate = pI2CHandle->TxRxState;

		if( (busystate != I2C_BUSY_IN_TX) && (busystate != I2C_BUSY_IN_RX))
		{
			pI2CHandle->pRxBuffer = pRxbuffer;
			pI2CHandle->RxLen = len;
			pI2CHandle->TxRxState = I2C_BUSY_IN_RX;
			pI2CHandle->RxSize = len; //Rxsize is used in the ISR code to manage the data reception
			pI2CHandle->DevAddr = SlaveAddr;
			pI2CHandle->Sr = Sr;

			//Implement code to Generate START Condition
			I2C_GenerateStartCondition(pI2CHandle->pI2Cx);

			//Implement the code to enable ITBUFEN Control Bit
			pI2CHandle->pI2Cx->CR2 |= ( 1 << I2C_CR2_ITBUFEN);

			//Implement the code to enable ITEVFEN Control Bit
			pI2CHandle->pI2Cx->CR2 |= ( 1 << I2C_CR2_ITEVTEN);

			//Implement the code to enable ITERREN Control Bit
			pI2CHandle->pI2Cx->CR2 |= ( 1 << I2C_CR2_ITERREN );

		}

		return busystate;
}

static void I2C_MasterHandleTXEInterrupt(I2C_Handle_t *pI2CHandle)
{
	if(pI2CHandle->TxLen >0)
	{
		pI2CHandle->pI2Cx->DR = *(pI2CHandle->pTxBuffer);

		pI2CHandle->TxLen--;

		pI2CHandle->pTxBuffer++;
	}
}

static void I2C_MasterHandleRXNEInterrupt(I2C_Handle_t *pI2CHandle)
{
	if(pI2CHandle->RxSize == 1)
				{
					*pI2CHandle->pRxBuffer = pI2CHandle->pI2Cx->DR;
					pI2CHandle->RxLen--;
				}
				if(pI2CHandle->RxSize > 1)
				{
					if(pI2CHandle->RxLen == 2)
					{
						I2C_MangeAck(pI2CHandle->pI2Cx, DISABLE);
					}
					*pI2CHandle->pRxBuffer = pI2CHandle->pI2Cx->DR;
					pI2CHandle->pRxBuffer++;
					pI2CHandle->RxLen--;
				}
				if(pI2CHandle->RxLen == 0)
				{
					if(pI2CHandle->Sr == I2C_DIABLE_SR)
						I2C_GenerateStopCondition(pI2CHandle->pI2Cx);

					I2C_CloseReceiveData(pI2CHandle);

					I2C_ApllicationEventCallback(pI2CHandle, I2C_EV_RX_CMPLT);
				}
}
void I2C_CloseReceiveData(I2C_Handle_t *pI2CHandle)
{
	pI2CHandle->pI2Cx->CR2 &= ~(1<<I2C_CR2_ITBUFEN);
	pI2CHandle->pI2Cx->CR2 &= ~(1<<I2C_CR2_ITEVTEN);

	pI2CHandle->TxRxState = I2C_READY;
	pI2CHandle->pRxBuffer = NULL;
	pI2CHandle->RxLen = 0;
	pI2CHandle->RxSize = 0;
	if(pI2CHandle->I2C_Config.I2C_ACKControl == I2C_ACK_ENABLE)
	{
		I2C_MangeAck(pI2CHandle->pI2Cx, ENABLE);
	}
}
void I2C_CloseSendData(I2C_Handle_t *pI2CHandle)
{
	pI2CHandle->pI2Cx->CR2 &= ~(1<<I2C_CR2_ITBUFEN);
	pI2CHandle->pI2Cx->CR2 &= ~(1<<I2C_CR2_ITEVTEN);

	pI2CHandle->TxRxState = I2C_READY;
	pI2CHandle->pTxBuffer = NULL;
	pI2CHandle->TxLen =0;
}

void I2C_SlaveSendData(I2C_RegDef_t *pI2C , uint8_t data)
{
	pI2C->DR  = data;
}
uint8_t I2C_SlaveReceiveData(I2C_RegDef_t *pI2C)
{
	return (uint8_t)pI2C->DR;
}

void I2C_EV_IRQHandling(I2C_Handle_t *pI2CHandle)
{
	uint32_t temp1,temp2,temp3;

	temp1 = pI2CHandle->pI2Cx->CR2 & (1<<I2C_CR2_ITEVTEN);
	temp2 = pI2CHandle->pI2Cx->CR2 & (1<<I2C_CR2_ITBUFEN);
	temp3 = pI2CHandle->pI2Cx->SR1 & (1<<I2C_SR1_SB);

	if(temp1 && temp3)
	{
		if(pI2CHandle->TxRxState == I2C_BUSY_IN_TX)
		{
			I2C_ExecuteAddressPhaseSend(pI2CHandle->pI2Cx, pI2CHandle->DevAddr);

		}else if(pI2CHandle->TxRxState == I2C_BUSY_IN_RX)
		{
			I2C_ExecuteAddressPhaseReceive(pI2CHandle->pI2Cx, pI2CHandle->DevAddr);
		}
	}

	temp3 = pI2CHandle->pI2Cx->SR1 & (1<<I2C_SR1_ADDR);
	if(temp1 && temp3)
	{
		I2C_ClearADDRFlag(pI2CHandle);
	}

	temp3 = pI2CHandle->pI2Cx->SR1 & (1<<I2C_SR1_BTF);
	if(temp1 && temp3)
	{
		if(pI2CHandle->TxRxState == I2C_BUSY_IN_TX)
		{
			if(pI2CHandle->pI2Cx->SR1 & (1<< I2C_SR1_TxE))
			{
				if(pI2CHandle->TxLen == 0)
				{
				if(pI2CHandle->Sr == I2C_DIABLE_SR)
				I2C_GenerateStopCondition(pI2CHandle->pI2Cx);

				I2C_CloseSendData(pI2CHandle);

				I2C_ApllicationEventCallback(pI2CHandle, I2C_EV_TX_CMPLT);
				}
			}
		}else if(pI2CHandle->TxRxState == I2C_BUSY_IN_RX)
		{
			;
		}
	}
	temp3 = pI2CHandle->pI2Cx->SR1 & (1<<I2C_SR1_STOPF);
	if(temp1 && temp3)
	{
		pI2CHandle->pI2Cx->CR1 |= 0x0000;
		I2C_ApllicationEventCallback(pI2CHandle, I2C_EV_STOP);
	}
	temp3 = pI2CHandle->pI2Cx->SR1 & (1<<I2C_SR1_TxE);
	if(temp1 && temp2 && temp3)
	{
		if(pI2CHandle->pI2Cx->SR2 & (1<< I2C_SR2_MSL))
		{
		if(pI2CHandle->TxRxState == I2C_BUSY_IN_TX)
		{
			I2C_MasterHandleTXEInterrupt(pI2CHandle);
		}
		}else
		{
			if(pI2CHandle->pI2Cx->SR2 & (1<<I2C_SR2_TRA))
			{
				I2C_ApllicationEventCallback(pI2CHandle, I2C_EV_DATA_REQ);
			}
		}
	}

	temp3 = pI2CHandle->pI2Cx->SR1 & (1<<I2C_SR1_RxNE);
	if(temp1 && temp2 && temp3)
	{
		if(pI2CHandle->pI2Cx->SR2 & (1<< I2C_SR2_MSL))
		{
		    if(pI2CHandle->TxRxState == I2C_BUSY_IN_RX)
		    {
		    	I2C_MasterHandleRXNEInterrupt(pI2CHandle);
		    }
		}else
		{
			if(!(pI2CHandle->pI2Cx->SR2 & (1<< I2C_SR2_TRA)))
			{
				I2C_ApllicationEventCallback(pI2CHandle, I2C_EV_DATA_RCV);
			}
		}
	}
}
void I2C_ER_IRQHandling(I2C_Handle_t *pI2CHandle)
{
	{

		uint32_t temp1,temp2;

	    //Know the status of  ITERREN control bit in the CR2
		temp2 = (pI2CHandle->pI2Cx->CR2) & ( 1 << I2C_CR2_ITERREN);


	/***********************Check for Bus error************************************/
		temp1 = (pI2CHandle->pI2Cx->SR1) & ( 1<< I2C_SR1_BERR);
		if(temp1  && temp2 )
		{
			//This is Bus error

			//Implement the code to clear the buss error flag
			pI2CHandle->pI2Cx->SR1 &= ~( 1 << I2C_SR1_BERR);

			//Implement the code to notify the application about the error
		   I2C_ApllicationEventCallback(pI2CHandle,I2C_ERROR_BERR);
		}

	/***********************Check for arbitration lost error************************************/
		temp1 = (pI2CHandle->pI2Cx->SR1) & ( 1 << I2C_SR1_ARLO );
		if(temp1  && temp2)
		{
			//This is arbitration lost error

			//Implement the code to clear the arbitration lost error flag
			pI2CHandle->pI2Cx->SR1 &= ~(1 << I2C_SR1_ARLO);
			//Implement the code to notify the application about the error
		   I2C_ApllicationEventCallback(pI2CHandle, I2C_ERROR_ARLO);
		}

	/***********************Check for ACK failure  error************************************/

		temp1 = (pI2CHandle->pI2Cx->SR1) & ( 1 << I2C_SR1_AF);
		if(temp1  && temp2)
		{
			//This is ACK failure error

		    //Implement the code to clear the ACK failure error flag
			pI2CHandle->pI2Cx->SR1 &= ~(1 << I2C_SR1_AF);
			//Implement the code to notify the application about the error
			I2C_ApllicationEventCallback(pI2CHandle, I2C_ERROR_AF);
		}

	/***********************Check for Overrun/underrun error************************************/
		temp1 = (pI2CHandle->pI2Cx->SR1) & ( 1 << I2C_SR1_OVR);
		if(temp1  && temp2)
		{
			//This is Overrun/underrun

		    //Implement the code to clear the Overrun/underrun error flag
			pI2CHandle->pI2Cx->SR1 &= ~(1 << I2C_SR1_OVR);
			//Implement the code to notify the application about the error
			I2C_ApllicationEventCallback(pI2CHandle, I2C_ERROR_OVR);
		}

	/***********************Check for Time out error************************************/
		temp1 = (pI2CHandle->pI2Cx->SR1) & ( 1 << I2C_SR1_TIMEOUT);
		if(temp1  && temp2)
		{
			//This is Time out error

		    //Implement the code to clear the Time out error flag
			pI2CHandle->pI2Cx->SR1 &= ~(1 << I2C_SR1_TIMEOUT);
			//Implement the code to notify the application about the error
			I2C_ApllicationEventCallback(pI2CHandle, I2C_ERROR_TIMEOUT);
		}

	}
}


uint8_t I2C_GetFlagStatus(I2C_RegDef_t *pI2Cx, uint32_t FlagName)
{

	if(pI2Cx->SR1 & FlagName)
		{
			return FLAG_SET;
		}
		return FLAG_RESET;

}


void I2C_IRQInterruptConfig(uint8_t IRQNumber,uint8_t EnorDi)
{

	if(EnorDi == ENABLE)
	{
		if(IRQNumber<=31)
		{
			*NVIC_ISER0 |= (1<<IRQNumber);

		}else if(IRQNumber>31 && IRQNumber<64)
		{
			*NVIC_ISER1 |= (1<<(IRQNumber%32));

		}else if (IRQNumber >=64 && IRQNumber <96)
		{
			*NVIC_ISER3 |= (1<<(IRQNumber%64));
		}
	}else
	{
		if(IRQNumber<=31)
		{
			*NVIC_ICER0 |= (1<<IRQNumber);

		}else if(IRQNumber>31 && IRQNumber<64)
		{
			*NVIC_ICER1 |= (1<<(IRQNumber%32));

		}else if(IRQNumber>=64 && IRQNumber<96)
		{
			*NVIC_ISER3 |= (1<<(IRQNumber%64));
		}
	}
}
void I2C_IRQPriorityConfig(uint8_t IRQNumber,uint8_t IRQPriority)
{
	uint8_t iprx = IRQNumber /4;
	uint8_t iprx_section = IRQNumber %4;
	uint8_t shift_amount = (8 * iprx_section) + (8 - NO_PR_BITS_IMPLEMENTED);
	*(NVIC_PR_BASEADDR + iprx) |= (IRQPriority << shift_amount );
}


void I2C_PeripheralControl(I2C_RegDef_t *pI2Cx,uint8_t EnorDi)
{
	if(EnorDi == ENABLE)
		{
			pI2Cx->CR1 |= (1<<I2C_CR1_PE);
		}else
		{
			pI2Cx->CR1 &= ~(1<<I2C_CR1_PE);
		}
}

__attribute__((weak)) void I2C_ApllicationEventCallback(I2C_Handle_t *pI2CHandle,uint8_t AppEv)
{

}

void I2C_MangeAck(I2C_RegDef_t *pI2Cx,uint8_t EnorDi)
{
	if(EnorDi == ENABLE)
		{
			pI2Cx->CR1 |= (1<<I2C_CR1_ACK);
		}else
		{
			pI2Cx->CR1 &= ~(1<<I2C_CR1_ACK);
		}
}
void I2C_SlaveEnableDisableCallbackEvents( I2C_RegDef_t *pI2Cx , uint8_t EnorDi)
{
	if(EnorDi == ENABLE )
	{
		pI2Cx->CR2 |= ( 1 << I2C_CR2_ITEVTEN);
		pI2Cx->CR2 |= ( 1 << I2C_CR2_ITBUFEN);
		pI2Cx->CR2 |= ( 1 << I2C_CR2_ITERREN);
	}else
	{
		pI2Cx->CR2 &= ~( 1 << I2C_CR2_ITEVTEN);
		pI2Cx->CR2 &= ~( 1 << I2C_CR2_ITBUFEN);
		pI2Cx->CR2 &= ~( 1 << I2C_CR2_ITERREN);
	}
}
