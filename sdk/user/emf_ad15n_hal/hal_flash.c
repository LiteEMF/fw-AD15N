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
#include  "api/api_flash.h"

#ifdef API_FLASH_ENABLE
#include  "api/api_gpio.h"
#include  "api/api_spi_host.h"
/******************************************************************************************************
** Defined
*******************************************************************************************************/

#define spi_cs_init() \
    do { \
        gpio_set_die(_norflash.spi_cs_io, 1); \
        hal_gpio_dir(m_spi_map[id].cs, PIN_OUT); \
        hal_gpio_out(_norflash.spi_cs_io, 1); \
    } while (0)

#define spi_cs_uninit() \
    do { \
        gpio_set_die(_norflash.spi_cs_io, 0); \
        hal_gpio_dir(_norflash.spi_cs_io, PIN_PULLNONE); \
        gpio_set_pull_up(_norflash.spi_cs_io, 0); \
        gpio_set_pull_down(_norflash.spi_cs_io, 0); \
    } while (0)
#define spi_cs_h()                  hal_gpio_out(_norflash.spi_cs_io, 1)
#define spi_cs_l()                  hal_gpio_out(_norflash.spi_cs_io, 0)

/*******************************flash 硬件spi选择*****************************/
#define spi_read_byte()             spi_recv_byte(_norflash.spi_num, NULL /* &_norflash.spi_err */)
#define spi_write_byte(x)           spi_send_byte(_norflash.spi_num, x)
#define spi_dma_read(x, y)          spi_dma_recv(_norflash.spi_num, x, y)
#define spi_dma_write(x, y)         spi_dma_send(_norflash.spi_num, x, y)
#define spi_set_width(x)            spi_set_bit_mode(_norflash.spi_num, x)
#define spi_init()              spi_open(_norflash.spi_num)
#define spi_closed()            spi_close(_norflash.spi_num)
#define spi_suspend()           hw_spi_suspend(_norflash.spi_num)
#define spi_resume()            hw_spi_resume(_norflash.spi_num)


/*****************************************************************************************************
**  Function
******************************************************************************************************/

static u8 is4byte_address = 0;
static void _norflash_send_addr(u32 addr)
{
    if (is4byte_address == 1) {
        spi_write_byte(addr >> 24);
    }
    spi_write_byte(addr >> 16);
    spi_write_byte(addr >> 8);
    spi_write_byte(addr);
}

static u32 _norflash_read_id()
{
    u8 id[3];
    int reg = 0;
    spi_cs_l();
    spi_write_byte(WINBOND_JEDEC_ID);
    for (u8 i = 0; i < sizeof(id); i++) {
        id[i] = spi_read_byte();
    }
    spi_cs_h();
    reg = _norflash_wait_ok();
    if (reg) {
        log_error("wait id fail\n");
        return 0;
    }
    if (id[0] == 0x01) {
        if (_S25FLxxx_read_id()) {
            return 0;
        }
    }
    return id[0] << 16 | id[1] << 8 | id[2];
}

/*******************************************************************
** Parameters:		
** Returns:	
** Description:		
*******************************************************************/
//hal
bool hal_flash_write(uint16_t offset,uint8_t *buf,uint16_t len)
{
	return false;
}
bool hal_flash_read(uint16_t offset,uint8_t *buf,uint16_t len)
{
	return false;
}
bool hal_flash_erase(uint16_t offset)
{
	return false;
}
bool hal_flash_init(void)
{


	return false;
}

#endif









