/*********************************************************************************************
    *   Filename        : main.c

    *   Description     :

    *   Author          :

    *   Email           :

    *   Last modifiled  :

    *   Copyright:(c)JIELI  2011-2017  @ , All Rights Reserved.
*********************************************************************************************/
#include "config.h"
#include "common.h"
#include "maskrom.h"
#include "asm/power/p33.h"
#include "app_config.h"
#include "init.h"
#include "vm.h"
#include "asm/power_interface.h"
#include "power_api.h"
#ifdef LITEEMF_ENABLED
#include "app/emf.h"
#include "api/api_log.h"
#endif

#define LOG_TAG_CONST       MAIN
#define LOG_TAG             "[main]"
#include "log.h"

extern void app(void);
int c_main(int cfg_addr)
{
    VDC13_LOAD_EN(1);

#if 1
    ///暂时调低p33跑的波特率
    JL_P33->SPI_CON |= (BIT(2) | BIT(3));
#endif

    /* spi_cache_way_switch(1); */
    /* request_irq(1, 7, exception_irq_handler, 0); */
    mask_init(exception_analyze, putchar);
    all_init_isr();

    
#ifdef HW_UART_MAP
	api_uart_init(UART_DEBUG_ID);
#else
    log_init(1000000);
#endif

    log_info("---------sh5x apps------------- \n");

    p33_tx_1byte(P3_PINR_CON, 0);

    reset_source_dump();
    power_reset_source_dump();

    //LVD电压配置，默认关闭
    /* p33_vlvd(LVLD_SEL_25V,0);//0:reset , 1:interrupt(wkup)*/

    //注:soft_off会Latch io, 唤醒之后电源初始化才会释放io，所以在电源初始化之后才能翻io/打印
    sys_power_init();

    //--- OSC CL  12M
    SFR(JL_CLK->CON0, 19, 2, 3);

    system_init();

    //init
    #ifdef LITEEMF_ENABLED
	emf_api_init();
    emf_init();
    #endif

    app();
    while (1) {
        wdt_clear();
    }
}


