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
#ifdef HW_PWM_MAP
#include  "api/api_pwm.h"
#include  "api/api_gpio.h"

#include "sfr.h"
#include "cpu.h"
#include "config.h"
#include "gpio.h"
#include "clock.h"

/******************************************************************************************************
** Defined
*******************************************************************************************************/
// #define HW_PWM_MAP {	\
// 			{PA_04,0,VAL2FLD(PWM_FREQ,200000)|VAL2FLD(PWM_CH,0)|VAL2FLD(PWM_ACTIVE,1)},		\
// 			{PA_05,0,VAL2FLD(PWM_FREQ,200000)|VAL2FLD(PWM_CH,1)|VAL2FLD(PWM_ACTIVE,0)},		\
// 			{PA_08,0,VAL2FLD(PWM_FREQ,200000)|VAL2FLD(PWM_CH,2)|VAL2FLD(PWM_ACTIVE,0)},		\
// 			{PA_09,0,VAL2FLD(PWM_FREQ,200000)|VAL2FLD(PWM_CH,3)|VAL2FLD(PWM_ACTIVE,0)},		\
// 			}
/******************************************************************************************************
**	static Parameters
*******************************************************************************************************/
typedef struct __HAL_PWM__
{
	u32	freq;
	u16	pwm;	//
	u8	output;	//0xFF不使用output,用硬件IO;0-1:使用映射IO
	u8  timer;	//哪个timer
	u8  inv;	//反向
	u8	io;		//IO
}HAL_MCPWM;

HAL_MCPWM	gLEDPWM[4];
/******************************************************************************************************
**	public Parameters
*******************************************************************************************************/

/*****************************************************************************************************
**	static Function
******************************************************************************************************/
#define pwm_frq_cnt(f)  (clk_get("lsb")/f)

#ifdef CPU_SH54 
const u32 mcpwm_tab[4] = {
    IO_PORTA_04,
    IO_PORTA_05,
    IO_PORTA_08,
    IO_PORTA_09,
};
#elif DCPU_SH55
const u32 mcpwm_tab[4] = {
    IO_PORTA_06,
    IO_PORTA_07,
    IO_PORTA_11,
    IO_PORTA_12,
};
#endif

#define _pwm_frq_duty(channel,frq,duty) \
    JL_PWM->TMR##channel##_PR = pwm_frq_cnt(frq); \
    JL_PWM->CH##channel##_CMP = pwm_frq_cnt(frq) * duty / 0xff;


#define _mcpwm_set(channel,frq,duty)                 \
    _pwm_frq_duty(channel,frq,duty);                 \
    /*sel pwm use mcpwmtmr0*/                       \
    SFR(JL_PWM->PWMCON1, (1+channel*4), 2, channel);\
    SFR(JL_PWM->PWMCON1, (channel*4), 1, 0);		\
    /* mcpwm tmr0 en */                             \
    JL_PWM->TMR##channel##_CON = BIT(5);            \
    JL_PWM->PWMCON0 |= BIT(8+channel);              \
    /*mcpwm ch0 en  */                              \
    JL_PWM->PWMCON0 |= BIT(channel);                \
    gpio_set_direction(mcpwm_tab[channel], 0);      \
	JL_IOMC->IOMC0 |= BIT(16+channel);

u8	mcpwm_att(u8 channel)
{
    if (m_pwm_map[channel].pin == mcpwm_tab[channel]) 
    {
        JL_IOMC->IOMC0|=1<<(16+channel);
        gpio_set_direction(m_pwm_map[channel].pin,0);		
        gpio_set_die(m_pwm_map[channel].pin,1);	
        gpio_set_pull_up(m_pwm_map[channel].pin,0);
        gpio_set_pull_down(m_pwm_map[channel].pin,0);	
        printf("Hard PWM\n");		
    }
    else{
        // if((pwm->timer&0x01)==0)
		// 	out=0;
		// else
		// 	out=1;
		// die=pwm->output;
		// out<<=die*3;
		// JL_IOMC->IOMC3&=~(0x07<<(die*3));
		// JL_IOMC->IOMC3|=out;	
		// gpio_set_direction(m_pwm_map[channel].pin,0);
		// gpio_set_pull_up(m_pwm_map[channel].pin,1);
		// gpio_set_pull_down(m_pwm_map[channel].pin,1);	
		// gpio_write(m_pwm_map[channel].pin,out);			
		// gpio_set_die(m_pwm_map[channel].pin,1);	
        if(channel <= 1)
        {
            gpio_output_channle(m_pwm_map[channel].pin, CH0_CH0_PWM+channel);//io选择--->设置输出通道
        }
        else{
            gpio_output_channle(m_pwm_map[channel].pin, CH1_CH2_PWM+channel-2);//io选择--->设置输出通道
        }   
    }
	printf("iomc0:%x,iomc3:%x\n",JL_IOMC->IOMC0,JL_IOMC->IOMC3);
	return true;
}

static void mcpwm_set(u8 channel, u32 fre, u32 duty)
{
    switch (channel) {
    case 0:
        _mcpwm_set(0, fre, duty);
        mcpwm_att(0);
        break;
    case 1:
        _mcpwm_set(1, fre, duty);
        mcpwm_att(1);
        break;
    case 2:
        _mcpwm_set(2, fre, duty);
        mcpwm_att(2);
        break;
    case 3:
        _mcpwm_set(3, fre, duty);
        mcpwm_att(3);
        break;
    default:
        break;
    }
}

static void pwm_frq_duty(u8 channel, u32 fre, u32 duty)
{
    switch (channel) {
    case 0:
        _pwm_frq_duty(0, fre, duty);
        break;
    case 1:
        _pwm_frq_duty(1, fre, duty);
        break;
    case 2:
        _pwm_frq_duty(2, fre, duty);
        break;
    case 3:
        _pwm_frq_duty(3, fre, duty);
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
** Description:		
*******************************************************************/
bool hal_pwm_set_duty(uint16_t id, uint8_t duty)
{
	pwm_frq_duty(PWM_CH_ATT(id),PWM_FREQ_ATT(id), duty);
	return true;
}
bool hal_pwm_init(uint16_t id, uint8_t duty)
{
	mcpwm_set(PWM_CH_ATT(id),PWM_FREQ_ATT(id), duty);
	return true;
}
bool hal_pwm_deinit(uint16_t id)
{
	hal_pwm_set_duty(id, 0);
	return true;
}



#endif





