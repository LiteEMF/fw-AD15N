#ifndef __APP_MODULES_H__
#define __APP_MODULES_H__
//  此文件在在服务器编译库时会自动生成，源文件位于app/bsp/common/app_modules_h.c
//  作者: 刘杰
//  日期:2022年11月22日
//  设计目的: 用于在应用代码中控制各种算法模块的使用
//  注意事项：不要在库文件中包含

#ifdef LITEEMF_ENABLED
#include "hw_config.h"
#endif


//  A格式解码
#ifndef DECODER_A_EN
#define DECODER_A_EN 1
#endif

//  标准MP3格式解码
#ifndef DECODER_MP3_ST_EN
#define DECODER_MP3_ST_EN 1
#endif

//  WAV格式解码
#ifndef DECODER_WAV_EN
#define DECODER_WAV_EN 1
#endif

//  F1A格式解码
#ifndef DECODER_F1A_EN
#define DECODER_F1A_EN 1
#endif
#ifndef MAX_F1A_CHANNEL
#define MAX_F1A_CHANNEL 2
#endif

//  UMP3格式解码
#ifndef DECODER_UMP3_EN
#define DECODER_UMP3_EN 1
#endif

//  MIDI格式解码
#ifndef DECODER_MIDI_EN
#define DECODER_MIDI_EN 1
#endif
#ifndef DECODER_MIDI_KEYBOARD_EN
#define DECODER_MIDI_KEYBOARD_EN 0
#endif

//  MIO功能使能
#ifndef HAS_MIO_EN
#define HAS_MIO_EN 0
#endif

//  解码SPEED功能使能
#ifndef AUDIO_SPEED_EN
#define AUDIO_SPEED_EN 1
#endif

//  陷波/移频啸叫抑制使能
#ifndef NOTCH_HOWLING_EN
#define NOTCH_HOWLING_EN 1
#endif
#ifndef PITCHSHIFT_HOWLING_EN
#define PITCHSHIFT_HOWLING_EN 2
#endif

//  变声功能使能
#ifndef VO_PITCH_EN
#define VO_PITCH_EN 0
#endif

//  动物变声功能使能
#ifndef VO_CHANGER_EN
#define VO_CHANGER_EN 0
#endif

//  ECHO混响功能使能
#ifndef ECHO_EN
#define ECHO_EN 1
#endif

//  EQ功能使能
#ifndef PCM_EQ_EN
#define PCM_EQ_EN 0
#endif

//  APP应用使能
#ifndef RECORD_MODE_EN
#define RECORD_MODE_EN 1 //  录音应用模式
#endif
#ifndef LINEIN_MODE_EN
#define LINEIN_MODE_EN 1 //  Linein应用模式
#endif
#ifndef SIMPLE_DEC_EN
#define SIMPLE_DEC_EN 0 //  多路解码应用模式
#endif
#ifndef LOUDSPEAKER_EN
#define LOUDSPEAKER_EN 1 //  扩音应用模式
#endif
#ifndef FM_EN
#define FM_EN 1 //  FM应用模式
#endif
//  驱动使能
#ifndef UART_UPDATE_EN
#define UART_UPDATE_EN 0 //  串口带电升级驱动使能
#endif


#endif


// ****************************************************************
//  此文件用于管理不同CPU间有差异的宏，源文件为app_modules_cpu_h.c
// ****************************************************************
#ifndef __APP_MODULES_CPU_H__
#define __APP_MODULES_CPU_H__

//  驱动使能
#ifndef EXT_FLASH_EN
#define EXT_FLASH_EN 0 //  外挂资源flash使能
#endif
#ifndef TFG_SD_EN
#define TFG_SD_EN 1 //  SDMMC驱动使能
#endif
#endif
