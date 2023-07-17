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
#if	defined HW_TIMER_MAP && API_TIMER_BIT_ENABLE
#include  "api/api_timer.h"
#include  "api/api_tick.h"
#include "sfr.h"
#include "cpu.h"
#include "config.h"
#include "gpio.h"
#include "clock.h"

/******************************************************************************************************
** Defined
*******************************************************************************************************/
// #define HW_TIMER_MAP	{0, VAL2FLD(TIMER_FREQ,1000)|VAL2FLD(TIMER_PRI,1)}
/******************************************************************************************************
**	static Parameters
*******************************************************************************************************/

/******************************************************************************************************
**	public Parameters
*******************************************************************************************************/

/*****************************************************************************************************
**	static Function
******************************************************************************************************/

#define TIMER_SFR(ch) JL_TMR##ch

/*
 *timer
 * */
#define _timer_init(ch,us)  \
    HWI_Install(IRQ_TIME##ch##_IDX, (u32)timer##ch##_isr, 4); 	\
	TIMER_SFR(ch)->PRD = clk_get("lsb")/4000000 * (us);			\
    TIMER_SFR(ch)->CON = BIT(0)<<4;               /* 4分频*/    \
	TIMER_SFR(ch)->CON |= BIT(0)|BIT(6);



uint8_t get_timer_id (uint8_t timer)
{
    uint8_t i;

    for(i=0; i<m_timer_num; i++){
        if(m_timer_map[i].peripheral == (uint32_t)timer){
            return i;
        }
    }
    return ID_NULL;
}

#if API_TIMER_BIT_ENABLE & BIT(0)
SET(interrupt(""))
static void timer0_isr(void)
{
    uint8_t id = get_timer_id(0);   
    TIMER_SFR(0)->CON |= BIT(6);
    api_timer_hook(id);  
}
#endif

#if API_TIMER_BIT_ENABLE & BIT(1)
SET(interrupt(""))
static void timer1_isr(void)
{
    uint8_t id = get_timer_id(1);   
    TIMER_SFR(1)->CON |= BIT(6);
    api_timer_hook(id);  
}
#endif

#if API_TIMER_BIT_ENABLE & BIT(2)
SET(interrupt(""))
static void timer2_isr(void)
{
    uint8_t id = get_timer_id(2);   
    TIMER_SFR(2)->CON |= BIT(6);
    api_timer_hook(id);  
    // JL_PORTA->DIR &= ~BIT(12);
    // JL_PORTA->DIE |= BIT(12);
    // JL_PORTA->OUT ^= BIT(12);
}
#endif


static void timer_init(u8 timer_ch, u32 us)
{
    switch (timer_ch) {
    case 0:
		#if API_TIMER_BIT_ENABLE & BIT(0)
        _timer_init(0, us);
		#endif
        break;
    case 1:
		#if API_TIMER_BIT_ENABLE & BIT(1)
        _timer_init(1, us);
		#endif
        break;
    case 2:
		#if API_TIMER_BIT_ENABLE & BIT(2)
        _timer_init(2, us);
		#endif
        break;
    default:
        break;
    }
}


/*****************************************************************************************************
**  Function
******************************************************************************************************/

/*******************************************************************
** Parameters:		
** Returns:	
** Description:	isr default on
*******************************************************************/
bool hal_timer_init(uint8_t id)
{
	uint32_t chanel = TIMER_CH_ATT(id);
	timer_init(chanel, TIMER_FREQ_ATT(id));
	return true;
}
bool hal_timer_deinit(uint8_t id)
{
	switch (id) {
    case 0:
		TIMER_SFR(0)->CON = 0;
        break;
    case 1:
		TIMER_SFR(1)->CON = 0;
        break;
    case 2:
		TIMER_SFR(2)->CON = 0;
        break;
    default:
        break;
    }

	return true;
}

#endif






