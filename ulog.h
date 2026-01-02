/**
  ******************************************************************************
  * @file           : unified_log.h
  * @brief          : 统一日志系统头文件
  *                   支持 SEGGER RTT、EasyLogger 和 printf 三种日志输出方式
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2025 STMicroelectronics.
  * All rights reserved.
  *
  ******************************************************************************
  */

#ifndef __ULOG_H_
#define __ULOG_H_

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include <stdio.h>
#include <stdarg.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include "ulog_cfg.h"

void ulog_init(void);
void ulog_deinit(void);
void ulog_hexdump(const char *name, uint8_t width, const void *buf, uint16_t size);
void ulog_output_ex(unsigned char terminal_id,char level, const char *tag,const char *fmt, ...);

/* -------------------------------------------------------------------------- */
/* 日志输出方式宏定义                                                         */
/* -------------------------------------------------------------------------- */
#define ULOG_OUTPUT_RTT         0x01    /* 使用 SEGGER RTT */
#define ULOG_OUTPUT_EASYLOGGER  0x02    /* 使用 EasyLogger */
#define ULOG_OUTPUT_PRINTF      0x04    /* 使用 printf */
#define ULOG_OUTPUT_ALL     	(ULOG_OUTPUT_RTT | ULOG_OUTPUT_EASYLOGGER | ULOG_OUTPUT_PRINTF)

/* -------------------------------------------------------------------------- */
/* 日志级别定义（与 Android logcat 类似）                                     */
/* -------------------------------------------------------------------------- */
#define ULOG_LEVEL_ASSERT       0   /* 断言，最高优先级 */
#define ULOG_LEVEL_ERROR        1   /* 错误 */
#define ULOG_LEVEL_WARN         2   /* 警告 */
#define ULOG_LEVEL_INFO         3   /* 一般信息 */
#define ULOG_LEVEL_DEBUG        4   /* 调试 */
#define ULOG_LEVEL_VERBOSE      5   /* 最详细 */

#ifndef ULOG_COLOR_ENABLE
#define ULOG_COLOR_ENABLE       1   /* 默认启用彩色输出 */
#endif

/* -------------------------------------------------------------------------- */
/* RTT 配置                                                                   */
/* -------------------------------------------------------------------------- */
#if (ULOG_OUTPUT_METHOD & ULOG_OUTPUT_RTT)
    #include "SEGGER_RTT.h"
#endif

/* -------------------------------------------------------------------------- */
/* EasyLogger 配置                                                            */
/* -------------------------------------------------------------------------- */
#if (ULOG_OUTPUT_METHOD & ULOG_OUTPUT_EASYLOGGER)
    #include "elog.h"
#endif

/* -------------------------------------------------------------------------- */
/* 日志颜色（仅在 RTT/printf 输出时有效）                                     */
/* -------------------------------------------------------------------------- */
#if ULOG_COLOR_ENABLE

	/* Control sequences, based on ANSI.
	*  Can be used to control color, and clear the screen
	*/
	#define ULOG_CTRL_RESET                "\x1B[0m"         // Reset to default colors
	#define ULOG_CTRL_CLEAR                "\x1B[2J"         // Clear screen, reposition cursor to top left

	#define ULOG_CTRL_TEXT_BLACK           "\x1B[2;30m"
	#define ULOG_CTRL_TEXT_RED             "\x1B[2;31m"
	#define ULOG_CTRL_TEXT_GREEN           "\x1B[2;32m"
	#define ULOG_CTRL_TEXT_YELLOW          "\x1B[2;33m"
	#define ULOG_CTRL_TEXT_BLUE            "\x1B[2;34m"
	#define ULOG_CTRL_TEXT_MAGENTA         "\x1B[2;35m"
	#define ULOG_CTRL_TEXT_CYAN            "\x1B[2;36m"
	#define ULOG_CTRL_TEXT_WHITE           "\x1B[2;37m"

	#define ULOG_CTRL_TEXT_BRIGHT_BLACK    "\x1B[1;30m"
	#define ULOG_CTRL_TEXT_BRIGHT_RED      "\x1B[1;31m"
	#define ULOG_CTRL_TEXT_BRIGHT_GREEN    "\x1B[1;32m"
	#define ULOG_CTRL_TEXT_BRIGHT_YELLOW   "\x1B[1;33m"
	#define ULOG_CTRL_TEXT_BRIGHT_BLUE     "\x1B[1;34m"
	#define ULOG_CTRL_TEXT_BRIGHT_MAGENTA  "\x1B[1;35m"
	#define ULOG_CTRL_TEXT_BRIGHT_CYAN     "\x1B[1;36m"
	#define ULOG_CTRL_TEXT_BRIGHT_WHITE    "\x1B[1;37m"

	#define ULOG_CTRL_BG_BLACK             "\x1B[24;40m"
	#define ULOG_CTRL_BG_RED               "\x1B[24;41m"
	#define ULOG_CTRL_BG_GREEN             "\x1B[24;42m"
	#define ULOG_CTRL_BG_YELLOW            "\x1B[24;43m"
	#define ULOG_CTRL_BG_BLUE              "\x1B[24;44m"
	#define ULOG_CTRL_BG_MAGENTA           "\x1B[24;45m"
	#define ULOG_CTRL_BG_CYAN              "\x1B[24;46m"
	#define ULOG_CTRL_BG_WHITE             "\x1B[24;47m"

	#define ULOG_CTRL_BG_BRIGHT_BLACK      "\x1B[4;40m"
	#define ULOG_CTRL_BG_BRIGHT_RED        "\x1B[4;41m"
	#define ULOG_CTRL_BG_BRIGHT_GREEN      "\x1B[4;42m"
	#define ULOG_CTRL_BG_BRIGHT_YELLOW     "\x1B[4;43m"
	#define ULOG_CTRL_BG_BRIGHT_BLUE       "\x1B[4;44m"
	#define ULOG_CTRL_BG_BRIGHT_MAGENTA    "\x1B[4;45m"
	#define ULOG_CTRL_BG_BRIGHT_CYAN       "\x1B[4;46m"
	#define ULOG_CTRL_BG_BRIGHT_WHITE      "\x1B[4;47m"



	#define ULOG_COLOR_RESET     ULOG_CTRL_RESET
	#define ULOG_COLOR_ASSERT    ULOG_CTRL_TEXT_MAGENTA
	#define ULOG_COLOR_ERROR     ULOG_CTRL_TEXT_RED
	#define ULOG_COLOR_WARN      ULOG_CTRL_TEXT_YELLOW
	#define ULOG_COLOR_INFO      ULOG_CTRL_TEXT_CYAN
	#define ULOG_COLOR_DEBUG     ULOG_CTRL_TEXT_GREEN
	#define ULOG_COLOR_VERBOSE   ULOG_CTRL_TEXT_WHITE

  	/* 根据日志级别选择颜色 */
	#define ULOG_GET_COLOR(level) \
		((level == 'A') 	? 	ULOG_COLOR_ASSERT : \
		(level  == 'E') 	?  	ULOG_COLOR_ERROR  : \
		(level  == 'W') 	? 	ULOG_COLOR_WARN   : \
		(level  == 'I') 	? 	ULOG_COLOR_INFO   : \
		(level  == 'D') 	? 	ULOG_COLOR_DEBUG  : \
		(level  == 'V') 	? 	ULOG_COLOR_VERBOSE: "")
#else
	#define ULOG_COLOR_RESET     	""
	#define ULOG_GET_COLOR(level) 	""
#endif


/* -------------------------------------------------------------------------- */
/* 宏封装：快速输出日志                                                       */
/* -------------------------------------------------------------------------- */
/* 默认使用 ULOG_TAG 和 ULOG_RTT_TERMINAL_ID */
#if ULOG_OUTPUT_DISABLE
    #define ULOG_A(fmt, ...) ((void)0)
    #define ULOG_E(fmt, ...) ((void)0)
    #define ULOG_W(fmt, ...) ((void)0)
    #define ULOG_I(fmt, ...) ((void)0)
    #define ULOG_D(fmt, ...) ((void)0)
    #define ULOG_V(fmt, ...) ((void)0)
#else
	#define ULOG_A(fmt, ...) 	ulog_output_ex(ULOG_RTT_TERMINAL_ID, 'A', ULOG_TAG, fmt, ##__VA_ARGS__)
	#define ULOG_E(fmt, ...) 	ulog_output_ex(ULOG_RTT_TERMINAL_ID, 'E', ULOG_TAG, fmt, ##__VA_ARGS__)
	#define ULOG_W(fmt, ...) 	ulog_output_ex(ULOG_RTT_TERMINAL_ID, 'W', ULOG_TAG, fmt, ##__VA_ARGS__)
	#define ULOG_I(fmt, ...) 	ulog_output_ex(ULOG_RTT_TERMINAL_ID, 'I', ULOG_TAG, fmt, ##__VA_ARGS__)
	#define ULOG_D(fmt, ...) 	ulog_output_ex(ULOG_RTT_TERMINAL_ID, 'D', ULOG_TAG, fmt, ##__VA_ARGS__)
	#define ULOG_V(fmt, ...) 	ulog_output_ex(ULOG_RTT_TERMINAL_ID, 'V', ULOG_TAG, fmt, ##__VA_ARGS__)
#endif

/* 指定标签 */
#if ULOG_OUTPUT_DISABLE
    #define ULOG_A_TAG(tag, format, ...) ((void)0)
    #define ULOG_E_TAG(tag, format, ...) ((void)0)
    #define ULOG_W_TAG(tag, format, ...) ((void)0)
    #define ULOG_I_TAG(tag, format, ...) ((void)0)
    #define ULOG_D_TAG(tag, format, ...) ((void)0)
    #define ULOG_V_TAG(tag, format, ...) ((void)0)
#else
    #define ULOG_A_TAG(tag, format, ...) 	ulog_output_ex(ULOG_RTT_TERMINAL_ID, 'A', tag, format, ##__VA_ARGS__)
    #define ULOG_E_TAG(tag, format, ...) 	ulog_output_ex(ULOG_RTT_TERMINAL_ID, 'E', tag, format, ##__VA_ARGS__)
    #define ULOG_W_TAG(tag, format, ...) 	ulog_output_ex(ULOG_RTT_TERMINAL_ID, 'W', tag, format, ##__VA_ARGS__)
    #define ULOG_I_TAG(tag, format, ...) 	ulog_output_ex(ULOG_RTT_TERMINAL_ID, 'I', tag, format, ##__VA_ARGS__)
    #define ULOG_D_TAG(tag, format, ...) 	ulog_output_ex(ULOG_RTT_TERMINAL_ID, 'D', tag, format, ##__VA_ARGS__)
    #define ULOG_V_TAG(tag, format, ...) 	ulog_output_ex(ULOG_RTT_TERMINAL_ID, 'V', tag, format, ##__VA_ARGS__)
#endif

/* 指定终端 */
#if ULOG_OUTPUT_DISABLE
    #define ULOG_A_T(tid, fmt, ...) ((void)0)
    #define ULOG_E_T(tid, fmt, ...) ((void)0)
    #define ULOG_W_T(tid, fmt, ...) ((void)0)
    #define ULOG_I_T(tid, fmt, ...) ((void)0)
    #define ULOG_D_T(tid, fmt, ...) ((void)0)
    #define ULOG_V_T(tid, fmt, ...) ((void)0)
#else
	#define ULOG_A_T(tid, fmt, ...) ulog_output_ex(tid, 'A', ULOG_TAG, fmt, ##__VA_ARGS__)
	#define ULOG_E_T(tid, fmt, ...) ulog_output_ex(tid, 'E', ULOG_TAG, fmt, ##__VA_ARGS__)
	#define ULOG_W_T(tid, fmt, ...) ulog_output_ex(tid, 'W', ULOG_TAG, fmt, ##__VA_ARGS__)
	#define ULOG_I_T(tid, fmt, ...) ulog_output_ex(tid, 'I', ULOG_TAG, fmt, ##__VA_ARGS__)
	#define ULOG_D_T(tid, fmt, ...) ulog_output_ex(tid, 'D', ULOG_TAG, fmt, ##__VA_ARGS__)
	#define ULOG_V_T(tid, fmt, ...) ulog_output_ex(tid, 'V', ULOG_TAG, fmt, ##__VA_ARGS__)
#endif


/* 指定终端+标签 */
#if ULOG_OUTPUT_DISABLE
    #define ULOG_A_TT(tid, tag, fmt, ...) ((void)0)
    #define ULOG_E_TT(tid, tag, fmt, ...) ((void)0)
    #define ULOG_W_TT(tid, tag, fmt, ...) ((void)0)
    #define ULOG_I_TT(tid, tag, fmt, ...) ((void)0)
    #define ULOG_D_TT(tid, tag, fmt, ...) ((void)0)
    #define ULOG_V_TT(tid, tag, fmt, ...) ((void)0)
#else
	#define ULOG_A_TT(tid, tag, fmt, ...) ulog_output_ex(tid, 'A', tag, fmt, ##__VA_ARGS__)
	#define ULOG_E_TT(tid, tag, fmt, ...) ulog_output_ex(tid, 'E', tag, fmt, ##__VA_ARGS__)
	#define ULOG_W_TT(tid, tag, fmt, ...) ulog_output_ex(tid, 'W', tag, fmt, ##__VA_ARGS__)
	#define ULOG_I_TT(tid, tag, fmt, ...) ulog_output_ex(tid, 'I', tag, fmt, ##__VA_ARGS__)
	#define ULOG_D_TT(tid, tag, fmt, ...) ulog_output_ex(tid, 'D', tag, fmt, ##__VA_ARGS__)
	#define ULOG_V_TT(tid, tag, fmt, ...) ulog_output_ex(tid, 'V', tag, fmt, ##__VA_ARGS__)
#endif


/* 原始日志（无标签、无级别，仅输出内容） */
#if ULOG_OUTPUT_DISABLE
    #define ULOG_RAW(fmt, ...) ((void)0)
#else
	#define ULOG_RAW(fmt, ...) ulog_output_ex(ULOG_RTT_TERMINAL_ID, 0, "", fmt, ##__VA_ARGS__)
#endif

/* Hexdump 快捷宏封装   */  
#if ULOG_OUTPUT_DISABLE
    #define ULOG_HEX(name, buf, size) 	((void)0)
#else
	#define ULOG_HEX(name, buf, size) 	ulog_hexdump(name, ULOG_HEX_WIDTH, buf, size)
#endif                                                    

#ifdef __cplusplus
}
#endif

#endif /* __UNIFIED_LOG_H */


