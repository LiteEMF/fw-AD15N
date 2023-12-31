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


#ifndef _hal_flash_h
#define _hal_flash_h
#include "emf_typedef.h" 

#ifdef __cplusplus
extern "C" {
#endif

/******************************************************************************************************
** Defined
*******************************************************************************************************/
#ifndef API_FLASH_PAGE_NUM
#define API_FLASH_PAGE_NUM		2
#endif
#ifndef API_FLASH_PAGE_SIZE
#define API_FLASH_PAGE_SIZE		0x100
#endif
#ifndef API_FLASH_ADDRESS
#define API_FLASH_ADDRESS		(0x08010000 - API_FLASH_PAGE_SIZE*API_FLASH_PAGE_NUM)
#endif

// #define WINBOND_WRITE_ENABLE		        0x06
// #define WINBOND_READ_SR1			  		0x05
// #define WINBOND_READ_SR2			  		0x35
// #define WINBOND_WRITE_SR1			  		0x01
// #define WINBOND_WRITE_SR2			  		0x31
// #define WINBOND_READ_DATA		        	0x03
// #define WINBOND_FAST_READ_DATA		    	0x0b
// #define WINBOND_FAST_READ_DUAL_OUTPUT   	0x3b
// #define WINBOND_PAGE_PROGRAM	            0x02
// #define WINBOND_PAGE_ERASE                  0x81
// #define WINBOND_SECTOR_ERASE		        0x20
// #define WINBOND_BLOCK_ERASE		          	0xD8
// #define WINBOND_CHIP_ERASE		          	0xC7
// #define WINBOND_JEDEC_ID                    0x9F
// #define WINBOND_POWER_DOWN                  0xB9
// #define WINBOND_RELEASE_POWER_DOWN          0xAB

// enum {
//     FLASH_PAGE_ERASER,
//     FLASH_SECTOR_ERASER,
//     FLASH_BLOCK_ERASER,
//     FLASH_CHIP_ERASER,
// };
/******************************************************************************************************
**	Parameters
*******************************************************************************************************/



/*****************************************************************************************************
**  Function
******************************************************************************************************/

#ifdef __cplusplus
}
#endif
#endif




