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

#include  "api/api_system.h"
#include  "api/api_pwm.h"
#include  "api/api_gpio.h"
#include  "api/api_log.h"

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
    /*_pwm_frq_duty(channel,frq,duty); */                \
    /*sel pwm use mcpwmtmr0*/                       \
    SFR(JL_PWM->PWMCON1, (1+channel*4), 2, channel);\
    SFR(JL_PWM->PWMCON1, (channel*4), 1, 0);		\
    /* mcpwm tmr0 en */                             \
    JL_PWM->TMR##channel##_CON = BIT(5);            \
    JL_PWM->PWMCON0 |= BIT(8+channel);              \
    /*mcpwm ch0 en  */                              \
    JL_PWM->PWMCON0 |= BIT(channel);                \
//    gpio_set_direction(mcpwm_tab[channel], 0);      \
	JL_IOMC->IOMC0 |= BIT(16+channel);


static u32 Mi(u32 num, int n)//num^n
{
    u32 powint = 1;
    int i;
    for (i = 1; i <= n; i++) {
        powint *= num;
    }
    return powint;
}

u8	mcpwm_att(u16 id,u8 channel,u16 dut)
{
    u16 tempfreq,temppwm,div;
	u32 con;
	u8 i;
	u32 *pcon,*pr,*cnt,*ch;

    div=1;
	for(i=0;i<9;i++)
	{	
		div=Mi(2,i);
		con=clk_get("lsb")/PWM_FREQ_ATT(id)/div;
		if(con<0x10000 && con!=0)
			break;
	}
	if(i==9)
		return 0;
    div=i;
	tempfreq=con;
	temppwm=tempfreq*dut/0xff;
    pcon=(u32*)&JL_PWM->TMR0_CON ;
	pr=(u32*)&JL_PWM->TMR0_PR ;
	ch=(u32*)&JL_PWM->CH0_CMP ;
	cnt=(u32*)&JL_PWM->TMR0_CNT ;
	*(cnt+id)=0;
	*(pcon+id)=div;	
	*(pr+id)=tempfreq;
	*(ch+id)=temppwm;    
    
    if (m_pwm_map[id].pin == mcpwm_tab[channel]) 
    {
        JL_IOMC->IOMC0|=1<<(16+channel);
        gpio_set_direction(m_pwm_map[id].pin,0);		
        gpio_set_die(m_pwm_map[id].pin,1);	
        gpio_set_pull_up(m_pwm_map[id].pin,0);
        gpio_set_pull_down(m_pwm_map[id].pin,0);	
        logd("Hard PWM\n");		
    }
    else{
        if(channel <= 1)
        {
            if((Map_Port & 0x40) == 0x00)
            {
                gpio_output_channle(m_pwm_map[id].pin, CH0_CH0_PWM+channel);//io选择--->设置输出通道
                // JL_IOMC->IOMC3&=~(0x07);
                // JL_IOMC->IOMC3|=channel;	
                SFR(Map_Port, 0, 0, channel);
                Map_Port |= 0x40;
                logd("Map_Port:0x%x",Map_Port);
                logd("IOMC3:0x%x",JL_IOMC->IOMC3);
            }
            else{
                logd("PWM%d MAP ERROR!\n",channel);
            }
        }
        else{
            if((Map_Port & 0x80) == 0x00)
            {
                gpio_output_channle(m_pwm_map[id].pin, CH1_CH2_PWM+channel-2);//io选择--->设置输出通道
                // JL_IOMC->IOMC3&=~(0x07<<3);
                // JL_IOMC->IOMC3|=((channel-2)<<3);
                SFR(Map_Port, 3, 3, (channel-2));
                Map_Port |= 0x80;
                logd("Map_Port:0x%x",Map_Port);
                logd("IOMC3:0x%x",JL_IOMC->IOMC3);
            }	
            else{
                logd("PWM%d MAP ERROR!\n",channel);
            }
        }  
        gpio_set_direction(m_pwm_map[id].pin,0);		
        gpio_set_die(m_pwm_map[id].pin,1);	
        gpio_set_pull_up(m_pwm_map[id].pin,1);
        gpio_set_pull_down(m_pwm_map[id].pin,1);	 
    }
    logd("pwmfrq:%d",pwm_frq_cnt(PWM_FREQ_ATT(id)));
	logd("iomc0:%x,iomc3:%x\r\n",JL_IOMC->IOMC0,JL_IOMC->IOMC3);
    
	return true;
}

static void mcpwm_set(u8 channel, u32 fre, u32 duty)
{
    switch (channel) {
    case 0:
        _mcpwm_set(0, fre, duty);
        break;
    case 1:
        _mcpwm_set(1, fre, duty);
        break;
    case 2:
        _mcpwm_set(2, fre, duty);
        break;
    case 3:
        _mcpwm_set(3, fre, duty);
        break;
    default:
        break;
    }
}

static void pwm_frq_duty(u8 channel, u32 fre, u32 duty)
{
    // switch (channel) {
    // case 0:
    //     _pwm_frq_duty(0, fre, duty);
    //     break;
    // case 1:
    //     _pwm_frq_duty(1, fre, duty);
    //     break;
    // case 2:
    //     _pwm_frq_duty(2, fre, duty);
    //     break;
    // case 3:
    //     _pwm_frq_duty(3, fre, duty);
    //     break;
    // default:
    //     break;
    // }

    u32 *ch,*prd;
	u32 temp0,temp1;

	prd=(u32*)&JL_PWM->TMR0_PR ;
	ch=(u32*)&JL_PWM->CH0_CMP ;
	temp0=*(prd+channel);
	temp1=duty;
	
	*(ch+channel)=(u16)temp0*temp1/0xff;
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
    mcpwm_att(id, PWM_CH_ATT(id), duty);
	return true;
}
bool hal_pwm_deinit(uint16_t id)
{
	hal_pwm_set_duty(id, 0);
	return true;
}



#endif





