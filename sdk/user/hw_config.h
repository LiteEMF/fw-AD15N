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


#ifndef _hw_config_h
#define _hw_config_h
#include "emf_typedef.h"

#ifdef __cplusplus
extern "C" {
#endif


/******************************************************************************************************
** Defined
*******************************************************************************************************/
#define HEAP_ID						0					/* used heap_n file*/
#define LOG_ENABLE                  1

#define PROJECT_KM                  0           //keyboard and mouse project
#define PROJECT_GAMEPAD             0           //keyboard and mouse
#define PROJECT_SOUND	            1           //sound box

#if PROJECT_KM
	/**********************************************************************************/
	#define DEV_TRPS_DEFAULT			(BIT(TR_BLE)|BIT(TR_USBD))				/*产品传输层支持*/
	#define DEV_TYPES_DEFAULT			BIT(DEV_TYPE_HID)
	#define HID_TYPES_DEFAULT			BIT(HID_TYPE_KB)
	/**********************************************************************************/

	#define HELLOW_KEYBOARD						1
	#if HELLOW_KEYBOARD
		#define SW_VERSION                     	0x01
        #define DEFAULT_NAME			       	"hellow_keyboard"
        #define DEFAULT_MODEL					"HKB"
	#else

	#endif

#elif PROJECT_GAMEPAD
	/**********************************************************************************/
	#define DEV_TRPS_DEFAULT			BIT(TR_NULL)				/*产品传输层支持*/
	#define DEV_TYPES_DEFAULT			BIT(DEV_TYPE_NONE)
	#define HID_TYPES_DEFAULT			BIT(HID_TYPE_NONE)
	/**********************************************************************************/
	#define GAMEPAD1					1
	

	#if GAMEPAD1	
		#define APP_KEY_ENABLE			1
		#define API_WDT_ENABLE			0
		#define API_TIMER_BIT_ENABLE 	BIT(0)

		#define API_USBD_BIT_ENABLE			0 //BIT(0)
		#define USBD_NUM					1
		#define USBD_TYPE_SUPPORT 			(BIT_ENUM(DEV_TYPE_MSD))
		// #define USBD_TYPE_SUPPORT 			(BIT_ENUM(DEV_TYPE_HID))
		// #define USBD_HID_SUPPORT			(BIT_ENUM(HID_TYPE_KB) | BIT_ENUM(HID_TYPE_MOUSE) | BIT_ENUM(HID_TYPE_CONSUMER))
		// #define USBD_HID_SUPPORT			(BIT_ENUM(HID_TYPE_KB))

		#define API_USBH_BIT_ENABLE			0 //(BIT(0) | BIT(1))
		#define	USBH_NUM					2
		#define USBH_TYPE_SUPPORT			(BIT_ENUM(DEV_TYPE_HID) | BIT_ENUM(DEV_TYPE_HUB))
		#define USBH_HID_SUPPORT			(BIT_ENUM(HID_TYPE_KB) | BIT_ENUM(HID_TYPE_MOUSE) | BIT_ENUM(HID_TYPE_CONSUMER))

		#define SW_VERSION                     	0x01
        #define DEFAULT_NAME			       	"gamepad"
        #define DEFAULT_MODEL					"GP_dev"
	#endif
#elif PROJECT_SOUND 
	/**********************************************************************************/
	#define DEV_TRPS_DEFAULT			BIT(TR_NULL)				/*产品传输层支持*/
	#define DEV_TYPES_DEFAULT			BIT(DEV_TYPE_NONE)
	#define HID_TYPES_DEFAULT			BIT(HID_TYPE_NONE)
	/**********************************************************************************/

	#define APP_MG							0
	#define MBOX							1

	#if APP_MG
		//emf config
		#define APP_KEY_ENABLE				1
		#define API_TIMER_BIT_ENABLE 		BIT(0)

		#define API_OTG_BIT_ENABLE			BIT(0)
		#define API_USBH_BIT_ENABLE			0 //(BIT(0) | BIT(1))

		#define API_USBD_BIT_ENABLE			0//BIT(0)
		#define USBD_NUM					1
		#define USBD_RAM_DISK_ENABLE		1
		#define USBD_TYPE_SUPPORT 		(BIT_ENUM(DEV_TYPE_MSD))
		// #define USBD_TYPE_SUPPORT 			BIT_ENUM(DEV_TYPE_HID)
		// #define USBD_HID_SUPPORT			(BIT_ENUM(HID_TYPE_KB) | BIT_ENUM(HID_TYPE_MOUSE) | BIT_ENUM(HID_TYPE_CONSUMER))


		//AD14 CONFIG
		#define ENCODER_EN					1
		#define DECODER_MIDI_EN				0
		#define DECODER_MIDI_KEYBOARD_EN	0
		#define HAS_USB_EN					1
		#define EXT_FLASH_EN				1
		#define FM_EN						0
		#define AUX_EN						1
		#define HAS_SDMMC_EN 				0
		#define TCFG_USB_EXFLASH_UDISK_ENABLE       1   /*外掛FLASH UDISK*/

		#if API_OTG_BIT_ENABLE
			#define TCFG_OTG_USB_DEV_EN     API_OTG_BIT_ENABLE

			#if API_USBD_BIT_ENABLE			//Enable USB SLAVE MODE
			#define USB_DEVICE_EN       
			#endif

			#if API_USBD_BIT_ENABLE && API_USBH_BIT_ENABLE
				#define TCFG_OTG_MODE       OTG_HOST_MODE | OTG_SLAVE_MODE | OTG_CHARGE_MODE | OTG_DET_DP_ONLY
			#elif API_USBD_BIT_ENABLE
				#define TCFG_OTG_MODE       OTG_SLAVE_MODE | OTG_CHARGE_MODE | OTG_DET_DP_ONLY
			#elif API_USBH_BIT_ENABLE
				#define TCFG_OTG_MODE       OTG_HOST_MODE | OTG_CHARGE_MODE | OTG_DET_DP_ONLY
			#else
				#define TCFG_OTG_MODE       0
			#endif
		#endif

		#define SW_VERSION                     	0x01
        #define DEFAULT_NAME			       	"app mg"
        #define DEFAULT_MODEL					"mg_dev"
	#elif MBOX
		//emf config
		#define APP_KEY_ENABLE				1
		#define API_TIMER_BIT_ENABLE 		BIT(0)
		
		#define API_OTG_BIT_ENABLE			0 //BIT(0)
		#define API_USBH_BIT_ENABLE			0

		#define API_USBD_BIT_ENABLE			0 //BIT(0)
		#define USBD_NUM					1
		#define USBD_TYPE_SUPPORT 			(BIT_ENUM(DEV_TYPE_MSD))
		#define USBD_MSC_BLOCK_SIZE			512
		#define USBD_DISK_BLOCK_SIZE		512
		#define USBD_DISK_BLOCK_NUM	    	0X2000
		// #define USBD_TYPE_SUPPORT 		(BIT_ENUM(DEV_TYPE_HID))
		// #define USBD_HID_SUPPORT			(BIT_ENUM(HID_TYPE_KB) | BIT_ENUM(HID_TYPE_MOUSE) | BIT_ENUM(HID_TYPE_CONSUMER))
		// #define USBD_HID_SUPPORT			(BIT_ENUM(HID_TYPE_KB))

		
		//SDK CONFIG
		#define D_MBOX_SDK					1	
		#define D_APP_MBOX					1
		#define AUX_EN						1
		#define DECODER_MIDI_EN				0
		#define DECODER_MIDI_KEYBOARD_EN	0
		#define EXT_FLASH_EN				1		
		#define FM_EN						0
		#define HAS_USB_EN					1		/*和 API_OTG_BIT_ENABLE 选择使用杰里内部还是LiteEMFusb*/
		#define HAS_SDMMC_EN 				0
		#define TCFG_USB_EXFLASH_UDISK_ENABLE       1   /*外掛FLASH UDISK*/
		
		#if API_OTG_BIT_ENABLE
			#define TCFG_OTG_USB_DEV_EN     API_OTG_BIT_ENABLE

			#if API_USBD_BIT_ENABLE			//Enable USB SLAVE MODE
			#define USB_DEVICE_EN       
			#endif

			#if API_USBD_BIT_ENABLE && API_USBH_BIT_ENABLE
				#define TCFG_OTG_MODE       OTG_HOST_MODE | OTG_SLAVE_MODE | OTG_CHARGE_MODE | OTG_DET_DP_ONLY
			#elif API_USBD_BIT_ENABLE
				#define TCFG_OTG_MODE       OTG_SLAVE_MODE | OTG_CHARGE_MODE | OTG_DET_DP_ONLY
			#elif API_USBH_BIT_ENABLE
				#define TCFG_OTG_MODE       OTG_HOST_MODE | OTG_CHARGE_MODE | OTG_DET_DP_ONLY
			#else
				#define TCFG_OTG_MODE       0
			#endif
		#endif


		#define SW_VERSION                     	0x01
        #define DEFAULT_NAME			       	"app_mbox"
        #define DEFAULT_MODEL					"mbox_dev"
	#endif
#endif



#include "emf_config.h"
#include "hw_board.h"
#ifdef __cplusplus
}
#endif
#endif





