// #ifndef __APP_CONFIG_H__
// #define __APP_CONFIG_H_

#define ENABLE_THIS_MOUDLE					1
#define DISABLE_THIS_MOUDLE					0

#define ENABLE								1
#define DISABLE								0

/*---------UART Configuration---------------*/
#define UART_TX_OUTPUT_CH_EN        DISABLE
#define UART_OUTPUT_CH_PORT			IO_PORTA_04

/*---------KEY Configuration---------------*/
//AD KEY
#define AD_KEY_CH_SEL				ADC_CH_PA0
//IR KEY
#define IR_KEY_IO_SEL				IO_PORTA_09
#define TCFG_ADKEY_IR_IO_REUSE		DISABLE//ADKEY 和 红外IO复用
//MATRIX KEY
/*X轴 io 要求是AD口，详细AD口看adc_drv.h*/
#define X_ADC_CH_SEL 				{ADC_CH_PA2,ADC_CH_PA3,ADC_CH_PA5}
/*Y轴 io 要求是普通IO口*/
#define Y_PORT_SEL 					{IO_PORTA_10,IO_PORTA_11}
#define MATRIX_KEY_THRESHOLD_SET	300
//TOUCH KEY
#define TOUCH_KEY_SEL				{IO_PORTA_09,IO_PORTA_10,IO_PORTA_11}

/*---------FLASH Configuration---------------*/
// #define 	EXT_FLASH_EN		0

/*---------KEY VOICE Configuration---------------*/
#define KEY_VOICE_EN   				0

/*---------dev upgrade Configuration---------------*/
#define TFG_DEV_UPGRADE_SUPPORT 1
#define TFG_UPGRADE_FILE_NAM	"/update.ufw"

/*---------SD Configuration---------------*/
#define TFG_SD_EN				0//ENABLE
/*
 ******************注意事项******************
 *功能描述：实现spi_flash和sd卡的引脚复用
 *注意：尚未完全调试完成，不能使用
 */
#define SPI_SD_IO_REUSE			0//SPI_FLASH与SD卡模块IO复用使能
/*---------USB Configuration---------------*/
#if HAS_USB_EN
#define TCFG_PC_ENABLE						DISABLE  //PC模块使能
#define TCFG_USB_MSD_CDROM_ENABLE           DISABLE  //用于pc工具更新flash数据
#define TCFG_USB_EXFLASH_UDISK_ENABLE       DISABLE  //外掛FLASH UDISK
#define TCFG_UDISK_ENABLE					DISABLE  //U盘模块使能
#define TCFG_HID_HOST_ENABLE				DISABLE
#define TCFG_ADB_ENABLE      				DISABLE
#define TCFG_AOA_ENABLE      				DISABLE

#else

#define TCFG_PC_ENABLE						DISABLE  //PC模块使能
#define TCFG_USB_MSD_CDROM_ENABLE           DISABLE  //用于pc工具更新flash数据
#define TCFG_USB_EXFLASH_UDISK_ENABLE       DISABLE  //外掛FLASH UDISK
#define TCFG_UDISK_ENABLE					DISABLE  //U盘模块使能
#define TCFG_HID_HOST_ENABLE				DISABLE
#define TCFG_ADB_ENABLE      				DISABLE
#define TCFG_AOA_ENABLE      				DISABLE
#endif


#define TCFG_USB_PORT_CHARGE                DISABLE
#define TCFG_OTG_USB_DEV_EN                 BIT(0)//USB0 = BIT(0)  USB1 = BIT(1)
#define TCFG_USB_DM_MULTIPLEX_WITH_SD_DAT0       DISABLE

#if TCFG_PC_ENABLE
#define USB_DEVICE_EN       //Enable USB SLAVE MODE
#endif
#if TCFG_UDISK_ENABLE
#define	USB_DISK_EN        //是否可以读U盘
#endif

#if TCFG_PC_ENABLE || TCFG_UDISK_ENABLE
#include "usb_std_class_def.h"
#include "usb_common_def.h"

#undef USB_DEVICE_CLASS_CONFIG
#define  USB_DEVICE_CLASS_CONFIG             MASSSTORAGE_CLASS//(MASSSTORAGE_CLASS|SPEAKER_CLASS|MIC_CLASS|HID_CLASS)  //配置usb从机模式支持的class

#undef TCFG_OTG_MODE
#define TCFG_OTG_MODE                       (TCFG_OTG_MODE_HOST|TCFG_OTG_MODE_SLAVE|TCFG_OTG_MODE_CHARGE|OTG_DET_DP_ONLY)
#else
#define  USB_DEVICE_CLASS_CONFIG            0
#define TCFG_OTG_MODE                       0
#endif

/*---------FM Configuration---------------*/
#define TCFG_FM_RDA5807_ENABLE DISABLE
#define TCFG_FM_QN8035_ENABLE  DISABLE
#define TCFG_FM_BK1080_ENABLE  DISABLE
// #endif
