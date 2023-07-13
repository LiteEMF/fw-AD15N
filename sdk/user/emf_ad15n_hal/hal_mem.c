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
#include "utils/mem/emf_mem.h"

#include "typedef.h"
#include "my_malloc.h"


/******************************************************************************************************
** Defined
*******************************************************************************************************/

/*****************************************************************************************************
**  Function
******************************************************************************************************/

/*******************************************************************
** Parameters:		
** Returns:	
** Description:		
*******************************************************************/
void* hal_malloc(uint32_t size)
{
	return my_malloc(size,MM_USER);	//TODO
}

/*******************************************************************
** Parameters:		
** Returns:	
** Description:		
*******************************************************************/
void hal_free(void* p)
{
	my_free(p);
}

/*******************************************************************
** Parameters:		
** Returns:	
** Description:		
*******************************************************************/
void hal_mem_stats(void)
{

}
