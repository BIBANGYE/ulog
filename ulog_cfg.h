/**
  ******************************************************************************
  * @file           : unified_log_config.h
  * @brief          : 统一日志系统配置文件
  *                   用于配置日志输出方式、级别等参数
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2025 STMicroelectronics.
  * All rights reserved.
  *
  ******************************************************************************
  */

#ifndef __ULOG_CONFIG_H_
#define __ULOG_CONFIG_H_

#ifdef __cplusplus
extern "C" {
#endif

/************************************************************
 * ? 核心配置
 ************************************************************/

/* 日志输出方式
 * ULOG_OUTPUT_RTT         - 使能 SEGGER RTT 输出
 * ULOG_OUTPUT_EASYLOGGER  - 使能 EasyLogger 输出
 * ULOG_OUTPUT_PRINTF      - 使能 printf 输出
 * ULOG_OUTPUT_ALL         - 同时使能所有输出方式
 */
#define ULOG_OUTPUT_DISABLE     0    /* 禁用所有日志 */
#define ULOG_OUTPUT_METHOD      (ULOG_OUTPUT_PRINTF)


/* 日志级别配置
 * ULOG_LEVEL_ASSERT  (0) - 断言级别
 * ULOG_LEVEL_ERROR   (1) - 错误级别
 * ULOG_LEVEL_WARN    (2) - 警告级别
 * ULOG_LEVEL_INFO    (3) - 信息级别
 * ULOG_LEVEL_DEBUG   (4) - 调试级别
 * ULOG_LEVEL_VERBOSE (5) - 详细级别
 */
#define ULOG_LEVEL              	ULOG_LEVEL_VERBOSE

/* 默认日志标签 */
#define ULOG_TAG               		"MAIN"

#define ULOG_SHOW_LOG				1

/* 时间戳和颜色 */
#define ULOG_COLOR_ENABLE       	1			/* 是否启用颜色输出 */
#define ULOG_WITH_TIMESTAMP     	0			/* 是否启用时间戳 */

#if ULOG_WITH_TIMESTAMP
#define ULOG_TIMESTAMP_FORMAT   1   			/* 0 = 简单毫秒数, 1 = d-h:m:s^ms */
/**
 * @brief 获取日志时间戳（弱符号函数）
 * 
 * 默认实现使用 HAL_GetTick()，返回毫秒级 tick。
 * 用户可在自己工程文件中重新实现该函数，例如：
 *   - FreeRTOS:   return xTaskGetTickCount();
 *   - RTC:        return rtc_get_time_ms();
 *   - Linux:      return (uint32_t)time(NULL);
 */
static inline uint32_t ulog_get_timestamp(void) {
    return HAL_GetTick();
}

static inline void ulog_get_timestamp_str(char *buf, size_t size) {
    uint32_t tick = ulog_get_timestamp();

#if (ULOG_WITH_TIMESTAMP == 0)
    snprintf(buf, size, "[%lu ms]", (unsigned long)tick);

#elif (ULOG_WITH_TIMESTAMP == 1)
    uint32_t days         = tick / (1000 * 60 * 60 * 24);
    uint32_t hours        = (tick / (1000 * 60 * 60)) % 24;
    uint32_t minutes      = (tick / (1000 * 60)) % 60;
    uint32_t seconds      = (tick / 1000) % 60;
    uint32_t milliseconds = tick % 1000;

    snprintf(buf, size, "[%02ud-%02uh:%02um:%02us^%03ums]",
             days, hours, minutes, seconds, milliseconds);
#endif
}
#endif

#if (!ULOG_COLOR_ENABLE)
	#ifdef ELOG_COLOR_ENABLE
	#undef ELOG_COLOR_ENABLE
	#endif
#endif


/* 缓冲区 */
#define ULOG_BUFFER_SIZE        512			/* 日志缓冲区大小 */
#define ULOG_HEX_WIDTH          16			/* 十六进制输出每行字节数 */

/* RTT 终端号配置 */
#define ULOG_RTT_TERMINAL_ID    0			/* 默认使用终端 (0-10) */

/************************************************************
 * 扩展功能（默认关闭）
 ************************************************************/

/* 模块化日志级别 */
#define LOG_MODULE_LEVELS      0
#if LOG_MODULE_LEVELS
    #define LOG_LEVEL_MAIN     ULOG_LEVEL_INFO		/* 主模块 */
    #define LOG_LEVEL_UART     ULOG_LEVEL_DEBUG		/* UART模块 */
    #define LOG_LEVEL_SPI      ULOG_LEVEL_DEBUG		/* SPI模块 */
    #define LOG_LEVEL_I2C      ULOG_LEVEL_DEBUG		/* I2C模块 */
    #define LOG_LEVEL_SENSOR   ULOG_LEVEL_INFO		/* 传感器模块 */
    #define LOG_LEVEL_NET      ULOG_LEVEL_DEBUG		/* 网络模块 */
    #define LOG_LEVEL_FS       ULOG_LEVEL_INFO		/* 文件系统模块 */
#endif

/* 运行时日志级别控制 */
#ifdef LOG_RUNTIME_CONTROL
    /* 支持运行时动态调整日志级别 */
    #define LOG_ENABLE_RUNTIME_LEVEL_CONTROL  1
#else
    #define LOG_ENABLE_RUNTIME_LEVEL_CONTROL  0
#endif


/* 日志过滤 */
#define LOG_ENABLE_FILTER      0
#if LOG_ENABLE_FILTER
    #define LOG_MAX_FILTER_TAGS     	10			/* 最大过滤标签数量 */
    #define LOG_MAX_FILTER_KEYWORDS 	5			/* 最大关键词数量 */
#endif

/* 异步日志配置 */
#define LOG_ENABLE_ASYNC       0
#if LOG_ENABLE_ASYNC
    #define LOG_ASYNC_BUFFER_SIZE   1024				/* 异步缓冲区大小 */
    #define LOG_ASYNC_TASK_PRIORITY osPriorityNormal	/* 异步任务优先级 */
#endif

/* 日志文件输出配置 */
#define LOG_ENABLE_FILE        0
#if LOG_ENABLE_FILE
    #define LOG_FILE_PATH      "/log/system.log"		/* 日志文件路径 */
    #define LOG_FILE_MAX_SIZE  (1024 * 1024)   			/* 日志文件最大大小 (字节) */
    #define LOG_FILE_MAX_COUNT 5						/* 最大日志文件数量 */
#endif

/* 网络日志输出配置 */
#define LOG_ENABLE_NETWORK     0
#if LOG_ENABLE_NETWORK
    #define LOG_NETWORK_HOST   "192.168.1.100"			/* 网络日志服务器地址 */
    #define LOG_NETWORK_PORT   514						/* 网络日志服务器端口 */
#endif

/* 编译时优化配置 */
#define LOG_OPTIMIZE_FOR_SIZE  		0					/* 优化代码大小，移除不必要的功能 */		
#define LOG_OPTIMIZE_FOR_SPEED 		0					/* 优化执行速度，可能增加代码大小 */

/* 调试配置 */
#define LOG_ENABLE_DEBUG       		0					/* 启用日志系统内部调试 */


#ifdef __cplusplus
}
#endif

#endif /* __UNIFIED_LOG_CONFIG_H */
