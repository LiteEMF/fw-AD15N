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
#ifdef HW_IIC_MAP

#include  "api/api_iic_host.h"
#include  "api/api_gpio.h"

/******************************************************************************************************
** Defined
*******************************************************************************************************/
// #define HW_IIC_MAP {	\
// 			{PB_00,PB_01,PB_02,'A',VAL2FLD(IIC_BADU,400000)},	\
// 			}


#define IIC_DEV_ID	 IIC_0

/******************************************************************************************************
**	public Parameters
*******************************************************************************************************/
//硬件IIC设备数据初始化
struct hw_iic_config hw_iic_cfg[1];

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
bool hal_iic_write(uint8_t id,uint8_t dev_addr,uint16_t addr, uint8_t const *buf, uint16_t len)
{
	uint8_t ret, i;

	hw_iic_start(IIC_DEV_ID);
	ret = hw_iic_tx_byte(IIC_DEV_ID, dev_addr);
	if(!ret)	goto err;

	if(addr >> 8){
		ret = hw_iic_tx_byte(IIC_DEV_ID, addr >> 8);
		if(!ret)	goto err;

	}
	ret = hw_iic_tx_byte(IIC_DEV_ID, addr);
	if(!ret)	goto err;


	i = 0;
	while (i < len) {
		ret = hw_iic_tx_byte(IIC_DEV_ID, buf[i]);
		if(!ret)	goto err;
		i++;

	}

err:
	hw_iic_stop(IIC_DEV_ID);  
	delay_us(30); 	//stop后需要delay一段时间后再start,JL6321必要的延时,两次通信之间至少20us

	return ret;
}
bool hal_iic_read(uint8_t id,uint8_t dev_addr,uint16_t addr, uint8_t* buf, uint16_t len)
{
	uint8_t ret, i;

	hw_iic_start(IIC_DEV_ID);
	ret = hw_iic_tx_byte(IIC_DEV_ID, dev_addr);
	if(!ret)	goto err;

	if(addr >> 8){
		ret = hw_iic_tx_byte(IIC_DEV_ID, addr >> 8);
		if(!ret)	goto err;
	}

	ret = hw_iic_tx_byte(IIC_DEV_ID, addr);
	if(!ret)	goto err;

	hw_iic_start(IIC_DEV_ID);
	ret = hw_iic_tx_byte(IIC_DEV_ID, dev_addr | 0x01UL);
	if(!ret)	goto err;

	i = 0;
	while (i < len - 1) {
		buf[i] = hw_iic_rx_byte(IIC_DEV_ID, 1);
		i++;
	}
	buf[i] = hw_iic_rx_byte(IIC_DEV_ID, 0);  //IIC主机接收最后1 byte NACK

err:
	hw_iic_stop(IIC_DEV_ID);
	delay_us(30);  	//stop后需要delay一段时间后再start,必要的延时,两次通信之间至少20us

	return ret;
}
bool hal_iic_isr_write(uint8_t id,uint8_t dev_addr,uint16_t addr, uint8_t const *buf, uint16_t len)
{
	return hal_iic_write(id, dev_addr, addr, buf, len);
}
bool hal_iic_isr_read(uint8_t id,uint8_t dev_addr,uint16_t addr, uint8_t* buf, uint16_t len)
{
	return hal_iic_read(id, dev_addr, addr, buf, len);
}
bool hal_iic_init(uint8_t id)
{
	hw_iic_cfg[0].port = m_iic_map[id].peripheral;
	hw_iic_cfg[0].baudrate = IIC_BADU_ATT(id);
	hw_iic_cfg[0].hdrive = 0;
	hw_iic_cfg[0].io_pu = 1;
	hw_iic_cfg[0].role = IIC_MASTER;
	hw_iic_cfg[0].isr_en = 0;

	return !hw_iic_init(IIC_DEV_ID);
}
bool hal_iic_deinit(uint8_t id)
{
	hw_iic_uninit(IIC_DEV_ID);
	return true;
}

#endif





