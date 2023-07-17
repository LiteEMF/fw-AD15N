/* 
*   BSD 2-Clause License
*   Copyright (c) 2022, LiteEMF
*   All rights reserved.
*   This software component is licensed by LiteEMF under BSD 2-Clause license,
*   the "License"; You may not use this file except in compliance with the
*   License. You may obtain a copy of the License at:
*       opensource.org/licenses/BSD-2-Clause
* 
*/

/************************************************************************************************************
**	Description:	
************************************************************************************************************/
#include "hw_config.h"
#include "hw_board.h"
#ifdef HW_SPI_HOST_MAP

#include  "api/api_spi_host.h"
#include  "api/api_gpio.h"

/******************************************************************************************************
** Defined
*******************************************************************************************************/
// #define HW_SPI_HOST_MAP {\
// 			{PB_07,PB_05,PB_06,PB_04,SPI1,VAL2FLD(SPI_BADU,1000)},	\
// 			}

/******************************************************************************************************
**	static Parameters
*******************************************************************************************************/

/******************************************************************************************************
**	public Parameters
*******************************************************************************************************/
// *INDENT-OFF*
//sh54: SPI1:  CLK       ,  DO        ,  DI
//SPI1: A组IO: IO_PORT_DP,  IO_PORT_DM,  IO_PORTA_03,
//SPI1: B组IO: IO_PORTA_11, IO_PORTA_12, IO_PORTA_10,
//SPI1: C组IO: IO_PORTA_04, IO_PORTA_05, IO_PORTA_06
//sh55: SPI1:  CLK       ,  DO        ,  DI
//SPI1: A组IO: IO_PORTB_00, IO_PORTB_01, IO_PORTB_02,
//SPI1: B组IO: IO_PORTA_14, IO_PORTA_15, IO_PORTA_13,
//SPI1: C组IO: IO_PORTA_06, IO_PORTA_07, IO_PORTA_08,
//SPI1: D组IO: IO_PORTB_08, IO_PORTB_09, IO_PORTB_07
/*****************************************************************************************************
**	static Function
******************************************************************************************************/

/*****************************************************************************************************
**  Function
******************************************************************************************************/

/*******************************************************************
** Parameters:		
** Returns:	
** Description:		
*******************************************************************/
bool hal_spi_host_write(uint8_t id,uint16_t addr, uint8_t * buf, uint16_t len)
{
	bool ret;
	uint16_t i;
	int8_t spi = m_spi_map[id].peripheral;

	if(addr >> 8){
		ret = !spi_send_byte(spi,(addr >> 8));
		ret &= !spi_send_byte(spi,addr);
	}else{
		ret = !spi_send_byte(spi,addr);
	}

	if(ret){
		ret = (len == spi_dma_send(spi, buf, len));
	}
	
	return true;
}
bool hal_spi_host_read(uint8_t id,uint16_t addr, uint8_t * buf, uint16_t len)
{
	bool ret;
	uint16_t i;
	int8_t spi = m_spi_map[id].peripheral;
	
	if(addr >> 8){
		ret = !spi_send_byte(spi,(addr >> 8));
		ret &= !spi_send_byte(spi,addr);
	}else{
		ret = !spi_send_byte(spi,addr);
	}
	if(ret){
		// ret = (0 < spi_dma_recv(spi, buf, len));	//全双工模式下dma 没有清除上一次发送的数据
		for(i=0; i<len; i++){
			buf[i] = spi_recv_byte(spi, NULL);
		}
		ret = true;
	}
	return ret;
}
bool hal_spi_host_isr_write(uint8_t id,uint16_t addr, uint8_t * buf, uint16_t len)
{
	return hal_spi_host_write(id,addr,buf,len);
}
bool hal_spi_host_isr_read(uint8_t id,uint16_t addr, uint8_t * buf, uint16_t len)
{
	return hal_spi_host_read(id,addr,buf,len);
}
bool hal_spi_host_init(uint8_t id)
{
	uint32_t badu = SPI_BADU_ATT(id) * 1000;

	spi1_p_data.port[0] = m_spi_map[id].clk;
	spi1_p_data.port[1] = m_spi_map[id].mosi;
	spi1_p_data.port[2] = m_spi_map[id].miso;
	spi1_p_data.mode = SPI_MODE_ATT(id);
	spi1_p_data.clk = badu;
	spi1_p_data.role = SPI_ROLE_MASTER;

	return !spi_open(m_spi_map[id].peripheral);
}
bool hal_spi_host_deinit(uint8_t id)
{
	spi_close(m_spi_map[id].peripheral);
	return true;
}
#endif




