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

	#define APP_MG							1

	#if APP_MG
		//AD14 CONFIG
		#define ENCODER_EN					1
		#define DECODER_MIDI_EN				0
		#define DECODER_MIDI_KEYBOARD_EN	0
		#define HAS_USB_EN					1
		#define EXT_FLASH_EN				1
		#define AUX_EN						1

		//emf config
		#define APP_KEY_ENABLE				1
		#define API_TIMER_BIT_ENABLE 		BIT(0)

		#define API_USBD_BIT_ENABLE			0 //BIT(0)
		#define USBD_NUM					1
		#define USBD_TYPE_SUPPORT 			(BIT_ENUM(DEV_TYPE_MSD))
		// #define USBD_TYPE_SUPPORT 		(BIT_ENUM(DEV_TYPE_HID))
		// #define USBD_HID_SUPPORT			(BIT_ENUM(HID_TYPE_KB) | BIT_ENUM(HID_TYPE_MOUSE) | BIT_ENUM(HID_TYPE_CONSUMER))
		// #define USBD_HID_SUPPORT			(BIT_ENUM(HID_TYPE_KB))

		#define SW_VERSION                     	0x01
        #define DEFAULT_NAME			       	"app mg"
        #define DEFAULT_MODEL					"mg_dev"
	#endif
#endif



#include "emf_config.h"
#include "hw_board.h"
#ifdef __cplusplus
}
#endif
#endif





