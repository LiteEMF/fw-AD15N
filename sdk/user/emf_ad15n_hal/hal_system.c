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
#include  "api/api_system.h"
#include  "clock.h"
/******************************************************************************************************
** Defined
*******************************************************************************************************/
#define MAP_NULL	0X00

uint8_t Map_Port = MAP_NULL;

/*****************************************************************************************************
**  Function
******************************************************************************************************/

/*******************************************************************
** Parameters:		
** Returns:	
** Description:		
*******************************************************************/
bool hal_set_sysclk(emf_clk_t clk, uint32_t freq)
{
	return true;
}
/*******************************************************************
** Parameters:		
** Returns:	
** Description:		
*******************************************************************/
uint32_t hal_get_sysclk(emf_clk_t clk)
{
	return HAL_SYS_FREQ;
}

bool hal_get_uuid(uint8_t *uuid, uint8_t len)
{
	return false;
}






