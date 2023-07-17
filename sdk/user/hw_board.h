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


#ifndef _hw_board_h
#define _hw_board_h
#include "emf_typedef.h"

#ifdef __cplusplus
extern "C" {
#endif


/******************************************************************************************************
** Defined
*******************************************************************************************************/
#if PROJECT_KM

#elif PROJECT_GAMEPAD
	#if GAMEPAD1
		// timer
		#define HW_TIMER_MAP	{0, VAL2FLD(TIMER_FREQ,1000)|VAL2FLD(TIMER_PRI,1)}

		// uart
		#define HW_UART_MAP {\
			{PA_04, PIN_NULL,	0, 0, (uint32_t)JL_UT0, VAL2FLD(UART_BAUD,1000000)},	\
			{PA_09, PIN_NULL, 	0, 64, (uint32_t)NULL, VAL2FLD(UART_BAUD,1000000)|VAL2FLD(UART_PRI,1)},		\
			}

		//adc
		#define HW_ADC_MAP {	\
			{PA_00,0,VAL2FLD(ADC_CH,0)},								\
			{PA_01,0,VAL2FLD(ADC_CH,1) | VAL2FLD(ADC_PULL,1)}			\
			}
		// iic 
		#define HW_IIC_MAP {	\
			{PA_08,PA_09,PB_02,'D',VAL2FLD(IIC_BADU,400000)},	\
			}
		// spi
		// #define HW_SPI_HOST_MAP {\
		// 	{PB_07,PB_05,PB_06,PB_04,SPI1,VAL2FLD(SPI_BADU,1000)},	\
		// 	}

		// pwm
		// #define HW_PWM_MAP {	\
		// 	{PA_04,0,VAL2FLD(PWM_FREQ,200000)|VAL2FLD(PWM_CH,0)|VAL2FLD(PWM_ACTIVE,1)},		\
		// 	{PA_05,0,VAL2FLD(PWM_FREQ,200000)|VAL2FLD(PWM_CH,1)|VAL2FLD(PWM_ACTIVE,0)},		\
		// 	}
	#endif
#elif PROJECT_SOUND 
	#if APP_MG
		// timer
		#define HW_TIMER_MAP	{0, VAL2FLD(TIMER_FREQ,1000)|VAL2FLD(TIMER_PRI,1)}

		// uart
		#define HW_UART_MAP {\
			{PA_04, PIN_NULL,	0, 0, (uint32_t)JL_UT0, VAL2FLD(UART_BAUD,1000000)},	\
			{PA_09, PIN_NULL, 	0, 64, (uint32_t)NULL, VAL2FLD(UART_BAUD,1000000)|VAL2FLD(UART_PRI,1)},		\
			}

		//adc
		#define HW_ADC_MAP {	\
			{PA_00,0,VAL2FLD(ADC_CH,0)},								\
			{PA_01,0,VAL2FLD(ADC_CH,1) | VAL2FLD(ADC_PULL,1)}			\
			}
		// iic 
		#define HW_IIC_MAP {	\
			{PA_08,PA_09,PB_02,'D',VAL2FLD(IIC_BADU,400000)},	\
			}
		// spi
		// #define HW_SPI_HOST_MAP {\
		// 	{PB_07,PB_05,PB_06,PB_04,SPI1,VAL2FLD(SPI_BADU,1000)},	\
		// 	}

		// pwm
		// #define HW_PWM_MAP {	\
		// 	{PA_04,0,VAL2FLD(PWM_FREQ,200000)|VAL2FLD(PWM_CH,0)|VAL2FLD(PWM_ACTIVE,1)},		\
		// 	{PA_05,0,VAL2FLD(PWM_FREQ,200000)|VAL2FLD(PWM_CH,1)|VAL2FLD(PWM_ACTIVE,0)},		\
		// 	}
	#elif MBOX

		#define KEY_POWER_GPIO 		PA_04

		// timer
		#define HW_TIMER_MAP {\
			{0, VAL2FLD(TIMER_FREQ,1000)|VAL2FLD(TIMER_CH,0)|VAL2FLD(TIMER_PRI,1)},	}
			

		// uart
		#define HW_UART_MAP {\
			{PA_00, PIN_NULL,	0, 0, (uint32_t)JL_UT0, VAL2FLD(UART_BAUD,1000000)},	}

		//adc
	//	#define HW_ADC_MAP {	\
			{PA_00,0,VAL2FLD(ADC_CH,0)},								\
			{PA_01,0,VAL2FLD(ADC_CH,1) | VAL2FLD(ADC_PULL,1)}			\
			}
		// iic 
		// #define HW_IIC_MAP {	\
		// 	{PA_08,PA_09,PB_02,'D',VAL2FLD(IIC_BADU,400000)},	\
		// 	}
		// spi
		// #define HW_SPI_HOST_MAP {\
		// 	{PB_07,PB_05,PB_06,PB_04,SPI1,VAL2FLD(SPI_BADU,1000)},	\
		// 	}

		// pwm
	//	#define HW_PWM_MAP {	\
			{PA_06,0,VAL2FLD(PWM_FREQ,150)|VAL2FLD(PWM_CH,0)|VAL2FLD(PWM_ACTIVE,1)},		\
			{PA_05,0,VAL2FLD(PWM_FREQ,1500)|VAL2FLD(PWM_CH,1)|VAL2FLD(PWM_ACTIVE,1)},		\
			{PA_08,0,VAL2FLD(PWM_FREQ,500000)|VAL2FLD(PWM_CH,2)|VAL2FLD(PWM_ACTIVE,1)},		\
			{PA_09,0,VAL2FLD(PWM_FREQ,38)|VAL2FLD(PWM_CH,3)|VAL2FLD(PWM_ACTIVE,1)},		\
			}

		//test board
		#define SPI_CS_PORT_SEL			IO_PORTA_03
		#define SPI_GROUP_SEL 			SPI1_GROUPC_IO
	#endif
#endif




#ifdef __cplusplus
}
#endif
#endif





