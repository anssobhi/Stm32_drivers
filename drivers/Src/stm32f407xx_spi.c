/*
 * stm32f407xx_spi.c
 *
 *  Created on: ٢٩‏/٠١‏/٢٠٢٢
 *      Author: ans
 */

#include "stm32f407xx_spi.h"

static void spi_txe_interrupt_handle(SPI_Handle_t *pSPIHandle);
static void spi_rxe_interrupt_handle(SPI_Handle_t *pSPIHandle);
static void spi_ovr_err_interrupt_handle(SPI_Handle_t *pSPIHandle);

void SPI_PerClockControl(SPI_RegDef_t *pSPIx, uint8_t EnorDi)
{
	if(EnorDi == ENABLE)
	{
		if(pSPIx == SPI1)
		{
			SPI1_PCLK_EN();
		}
		else if(pSPIx == SPI2)
		{
			SPI2_PCLK_EN();
		}
		else if (pSPIx == SPI3)
		{
			SPI3_PCLK_EN();
		}
	}else
	{
		if(pSPIx == SPI1)
		{
			SPI1_PCLK_DI();
		}
		else if(pSPIx == SPI2)
		{
		   SPI2_PCLK_DI();
		}
		else if (pSPIx == SPI3)
		{
			SPI3_PCLK_DI();
		}
	}
}

void SPI_Inti(SPI_Handle_t *pSPIHandle)
{
	SPI_PerClockControl(pSPIHandle->pSPIx,ENABLE);

	uint32_t  tempreg = 0;

	tempreg |= pSPIHandle->SPIConfig.SPI_DeviceMode << SPI_CR1_MSTR;

	if(pSPIHandle->SPIConfig.SPI_BusConfig == SPI_BUS_CONFIG_FD)
	{
		tempreg &= ~(1<<SPI_CR1_BIDIMODE);

	}else if(pSPIHandle->SPIConfig.SPI_BusConfig == SPI_BUS_CONFIG_HD)
	{
		tempreg |= (1<<SPI_CR1_BIDIMODE);
	}else if(pSPIHandle->SPIConfig.SPI_BusConfig == SPI_BUS_CONFIG_RXONLY)
	{
		tempreg &= ~(1<<SPI_CR1_BIDIMODE);
		tempreg |=  (1<<SPI_CR1_RXONLY);
	}
	if(pSPIHandle->SPIConfig.SPI_SSM == SPI_SSM_EN)
	{
		tempreg |= (1<<SPI_CR1_SSM);
	}else
	{
		tempreg &= ~(1<<SPI_CR1_SSM);
	}
	tempreg |= pSPIHandle->SPIConfig.SPI_SclkSpeed <<SPI_CR1_BAUDRATE;
	tempreg |= pSPIHandle->SPIConfig.SPI_DFF <<SPI_CR1_DFF;
	tempreg |= pSPIHandle->SPIConfig.SPI_CPOL <<SPI_CR1_CPOL;
	tempreg |= pSPIHandle->SPIConfig.SPI_CPHA <<SPI_CR1_CPHA;
	pSPIHandle->pSPIx->CR1 = tempreg;
}
void SPI_DeInti(SPI_RegDef_t *pSPIx)
{

}

uint8_t SPI_GetFlagStatus(SPI_RegDef_t *pSPIx, uint32_t FlagName)
{
	if(pSPIx->SR & FlagName)
	{
		return FLAG_SET;
	}
	return FLAG_RESET;
}

void SPI_SendData(SPI_RegDef_t *pSPIx,uint8_t *pTxBuffer,uint32_t Len)
{
	while(Len > 0)
	{
		// 1. wait until TXE is set
		while(SPI_GetFlagStatus(pSPIx,SPI_TXE_FLAG)== FLAG_RESET);
		// 2. check the DFF
		if((pSPIx->CR1 &(1<<SPI_CR1_DFF)))
		{
			pSPIx->DR = *((uint16_t*)pTxBuffer);
			Len--;
			Len--;
			(uint16_t*)pTxBuffer++;
		}else
		{
			pSPIx->DR = *pTxBuffer;
			Len--;
			pTxBuffer++;
		}
	}
}
void SPI_ReceiveData(SPI_RegDef_t *pSPIx,uint8_t *pRxBuffer,uint32_t Len)
{
	while(Len > 0)
	{
		// 1. wait until TXE is set
		while(SPI_GetFlagStatus(pSPIx,SPI_RXNE_FLAG)== FLAG_RESET);
		// 2. check the DFF
		if((pSPIx->CR1 &(1<<SPI_CR1_DFF)))
		{
			*((uint16_t*)pRxBuffer) = pSPIx->DR ;
			Len--;
			Len--;
			(uint16_t*)pRxBuffer++;
		}else
		{
			*pRxBuffer = pSPIx->DR ;
			Len--;
			pRxBuffer++;
		}
	}
}

void SPI_PeripheralControl(SPI_RegDef_t *pSPIx,uint8_t EnorDi)
{
	if(EnorDi == ENABLE)
	{
		pSPIx->CR1 |= (1<<SPI_CR1_SPE);
	}else
	{
		pSPIx->CR1 &= ~(1<<SPI_CR1_SPE);
	}
}
void SPI_SSOEConfig(SPI_RegDef_t *pSPIx,uint8_t EnorDi)
{
	if(EnorDi == ENABLE)
			{
				pSPIx->CR2 |= (1<<SPI_CR2_SSOE);
			}else
			{
				pSPIx->CR2 &= ~(1<<SPI_CR2_SSOE);
			}
}
void SPI_SSIConfig(SPI_RegDef_t *pSPIx,uint8_t EnorDi)
{
	if(EnorDi == ENABLE)
		{
			pSPIx->CR1 |= (1<<SPI_CR1_SSI);
		}else
		{
			pSPIx->CR1 &= ~(1<<SPI_CR1_SSI);
		}
}

void SPI_IRQInterruptConfig(uint8_t IRQNumber,uint8_t EnorDi)
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
void SPI_IRQPriorityConfig(uint8_t IRQNumber,uint8_t IRQPriority)
{
	uint8_t iprx = IRQNumber /4;
	uint8_t iprx_section = IRQNumber %4;
	uint8_t shift_amount = (8 * iprx_section) + (8 - NO_PR_BITS_IMPLEMENTED);
	*(NVIC_PR_BASEADDR + iprx) |= (IRQPriority << shift_amount );
}

uint8_t SPI_SendDataIT(SPI_Handle_t *pSPIHandle,uint8_t *pTxBuffer,uint32_t Len)
{
	uint8_t state = pSPIHandle->TxState;
	if(state != SPI_BUSY_IN_TX)
	{
	//1. save the Tx Buffer address and Len information in global variable
	pSPIHandle->PTxBuffer = pTxBuffer;
	pSPIHandle->TxLen = Len;
	// 2. mark the spi state as busy in transimission so no other code
	pSPIHandle->TxState = SPI_BUSY_IN_TX;
	// 3. enable the TXEIE control bit
	pSPIHandle->pSPIx->CR2 |= (1<<SPI_CR2_TXEIE);
	// 4. data transmission will be handled by the ISR code
	}
	return state;
}
uint8_t SPI_ReceiveDataIT(SPI_Handle_t *pSPIHandle,uint8_t *pRxBuffer,uint32_t Len)
{
	uint8_t state = pSPIHandle->RxState;
		if(state != SPI_BUSY_IN_RX)
		{
		//1. save the Tx Buffer address and Len information in global variable
		pSPIHandle->PRxBuffer = pRxBuffer;
		pSPIHandle->RxLen = Len;
		// 2. mark the spi state as busy in transimission so no other code
		pSPIHandle->RxState = SPI_BUSY_IN_RX;
		// 3. enable the TXEIE control bit
		pSPIHandle->pSPIx->CR2 |= (1<<SPI_CR2_RXNEIE);
		// 4. data transmission will be handled by the ISR code
		}
		return state;
}
void SPI_IRQHandling(SPI_Handle_t *pHandle)
{
	uint8_t temp1,temp2;
	temp1 = pHandle->pSPIx->SR & (1<<SPI_SR_TXE);
	temp2 = pHandle->pSPIx->CR2 & (1<<SPI_CR2_TXEIE);

	if(temp1 && temp2)
	{
		spi_txe_interrupt_handle(pHandle);
	}

	temp1 = pHandle->pSPIx->SR & (1<<SPI_SR_RXNE);
	temp2 = pHandle->pSPIx->CR2 & (1<<SPI_CR2_RXNEIE);

	if(temp1 && temp2)
	{
		spi_rxe_interrupt_handle(pHandle);
	}
	// check the ovr flag
	temp1 = pHandle->pSPIx->SR & (1<<SPI_SR_OVR);
	temp2 = pHandle->pSPIx->CR2 & (1<<SPI_CR2_ERRIE);

	if(temp1 && temp2)
	{
		spi_ovr_err_interrupt_handle(pHandle);
	}
}

static void spi_txe_interrupt_handle(SPI_Handle_t *pSPIHandle)
{
	if((pSPIHandle->pSPIx->CR1 &(1<<SPI_CR1_DFF)))
	{
		pSPIHandle->pSPIx->DR = *((uint16_t*)pSPIHandle->PTxBuffer);
		pSPIHandle->TxLen--;
		pSPIHandle->TxLen--;
		(uint16_t*)pSPIHandle->PTxBuffer++;
	}else
	{
	    pSPIHandle->pSPIx->DR = *(pSPIHandle->PTxBuffer);
	    pSPIHandle->TxLen--;
	    pSPIHandle->PTxBuffer++;
	}
	if(! pSPIHandle->TxLen)
	{
		// this is prevent interrupt
		SPI_CloseTransmission(pSPIHandle);
		SPI_ApllicationEventCallback(pSPIHandle,SPI_EVENT_TX_CMPLT);
	}

}
static void spi_rxe_interrupt_handle(SPI_Handle_t *pSPIHandle)
{
	if((pSPIHandle->pSPIx->CR1 &(1<<SPI_CR1_DFF)))
	{
		*((uint16_t*)pSPIHandle->PRxBuffer) = (uint16_t)pSPIHandle->pSPIx->DR ;
		pSPIHandle->RxLen -=2;
		pSPIHandle->PRxBuffer++;
		pSPIHandle->PRxBuffer++;
	}else
	{
		*(pSPIHandle->PRxBuffer) = (uint8_t)pSPIHandle->pSPIx->DR ;
		pSPIHandle->RxLen--;
		pSPIHandle->PRxBuffer++;
	}
	if(!pSPIHandle->RxLen)
	{
		SPI_CloseReception(pSPIHandle);
		SPI_ApllicationEventCallback(pSPIHandle,SPI_EVENT_RX_CMPLT);
	}
}
static void spi_ovr_err_interrupt_handle(SPI_Handle_t *pSPIHandle)
{
	uint8_t temp;
	if(pSPIHandle->TxState != SPI_BUSY_IN_TX)
	{
		temp = pSPIHandle->pSPIx->DR;
		temp = pSPIHandle->pSPIx->SR;
	}
	(void)temp;
	SPI_ApllicationEventCallback(pSPIHandle,SPI_EVENT_OVR_ERR);
}
void SPI_ClearOVRFlag(SPI_RegDef_t *pSPIx)
{
	uint8_t temp;
	temp = pSPIx->DR;
	temp = pSPIx->SR;
	(void)temp;
}
void SPI_CloseTransmission(SPI_Handle_t *pSPIHandle)
{
	   pSPIHandle->pSPIx->CR2 &= ~(1<<SPI_CR2_TXEIE);
	   pSPIHandle->PTxBuffer = NULL;
	   pSPIHandle->TxLen = 0;
	   pSPIHandle->TxState = SPI_READY;
}
void SPI_CloseReception(SPI_Handle_t *pSPIHandle)
{
	    pSPIHandle->pSPIx->CR2 &= ~(1<< SPI_CR2_RXNEIE);
	    pSPIHandle->PRxBuffer = NULL;
		pSPIHandle->RxLen = 0;
		pSPIHandle->RxState = SPI_READY;
}
__attribute__((weak)) void SPI_ApllicationEventCallback(SPI_Handle_t *pSPIHandle,uint8_t AppEv)
{

}
