#include "ulog.h"



/* -------------------------------------------------------------------------- */
/* 初始化/反初始化函数                                                        */
/* -------------------------------------------------------------------------- */
void ulog_init(void)
{
#if !ULOG_OUTPUT_DISABLE

#if (ULOG_OUTPUT_METHOD & ULOG_OUTPUT_RTT)
	SEGGER_RTT_Init();
#endif
#if (ULOG_OUTPUT_METHOD & ULOG_OUTPUT_EASYLOGGER)
	elog_init();
	elog_start();
#endif

#if ULOG_SHOW_LOG
	ULOG_RAW("\r\n");
	ULOG_RAW("  _   _ _     ___   ____   ____   ___   ___ _____ ___ _   _  ____ \r\n");
    ULOG_RAW(" | | | | |   / _ \\ / ___| | __ ) / _ \\ / _ \\_   _|_ _| \\ | |/ ___|\r\n");
    ULOG_RAW(" | | | | |  | | | | |  _  |  _ \\| | | | | | || |  | ||  \\| | |  _ \r\n");
    ULOG_RAW(" | |_| | |__| |_| | |_| | | |_) | |_| | |_| || |  | || |\\  | |_| |\r\n");
    ULOG_RAW("  \\___/|_____\\___/ \\____| |____/ \\___/ \\___/ |_| |___|_| \\_|\\____|\r\n");
    ULOG_RAW("\r\n");
#endif

#endif
}

void ulog_deinit(void)
{
#if !ULOG_OUTPUT_DISABLE

#if (ULOG_OUTPUT_METHOD & ULOG_OUTPUT_EASYLOGGER)
	elog_stop();
	elog_deinit();
#endif

#endif
}

/* -------------------------------------------------------------------------- */
/* 内部函数：格式化日志消息                                                   */
/* -------------------------------------------------------------------------- */
static int format_log_message(char *buf, size_t size,
                                     char level, const char *tag,
                                     const char *fmt, va_list args)
{
    int len = 0;

    /* 格式：[E/TAG] 或 [TAG] */
    if (level && tag && *tag) {
        len = snprintf(buf, size, "[%c/%s] ", level, tag);
    } else if (tag && *tag) {
        len = snprintf(buf, size, "[%s] ", tag);
    }

	#if ULOG_WITH_TIMESTAMP
	char ts[64];
    ulog_get_timestamp_str(ts, sizeof(ts));
    len += snprintf(buf + len, size - len, "%s ", ts);
	#endif

    /* 追加实际日志内容 */
    len += vsnprintf(buf + len, size - len, fmt, args);

	#if 0
    /* 追加换行 */
    len += snprintf(buf + len, size - len, "\r\n");
	#endif

    return len;
}

/* -------------------------------------------------------------------------- */
/* 内部函数：输出到 RTT                                                       */
/* -------------------------------------------------------------------------- */
static void log_backend_rtt(unsigned char terminal, char level, const char *msg)
{
#if (ULOG_OUTPUT_METHOD & ULOG_OUTPUT_RTT) && (SUPPORT_SEGGER_RTT == 1)
    SEGGER_RTT_SetTerminal(terminal);
  #if ULOG_COLOR_ENABLE
    if (level) {
        SEGGER_RTT_WriteString(0, ULOG_COLOR_RESET);
        SEGGER_RTT_WriteString(0, ULOG_GET_COLOR(level));
        SEGGER_RTT_WriteString(0, msg);
        SEGGER_RTT_WriteString(0, ULOG_COLOR_RESET);
    } else {
        SEGGER_RTT_WriteString(0, msg);
    }

	/* 确保在函数结束时重置颜色 */
    SEGGER_RTT_WriteString(0,ULOG_CTRL_RESET"");
  #else
    SEGGER_RTT_WriteString(0, msg);
  #endif
#endif
}

/* -------------------------------------------------------------------------- */
/* 内部函数：输出到 EasyLogger                                                */
/* -------------------------------------------------------------------------- */
static void log_backend_easylogger(char level, const char *tag, const char *msg)
{
#if (ULOG_OUTPUT_METHOD & ULOG_OUTPUT_EASYLOGGER)
    if (level) {
        switch (level) {
            case 'A': elog_assert(tag, "%s", msg); break;
            case 'E': elog_error(tag, "%s", msg); break;
            case 'W': elog_warn(tag, "%s", msg);  break;
            case 'I': elog_info(tag, "%s", msg); break;
            case 'D': elog_debug(tag, "%s", msg); break;
            case 'V': elog_verbose(tag, "%s", msg); break;
            default : elog_raw("%s", msg); break;
        }
    } else {
        elog_raw("%s", msg);
    }
#endif
}

/* -------------------------------------------------------------------------- */
/* 内部函数：输出到 printf                                                    */
/* -------------------------------------------------------------------------- */
static void log_backend_printf(char level, const char *msg)
{
#if (ULOG_OUTPUT_METHOD & ULOG_OUTPUT_PRINTF)
  #if ULOG_COLOR_ENABLE
    if (level) {
        printf("%s%s%s", ULOG_GET_COLOR(level), msg, ULOG_COLOR_RESET);
    } else {
        printf("%s", msg);
    }
  #else
    printf("%s", msg);
  #endif
#endif
}

/* -------------------------------------------------------------------------- */
/* 核心函数：统一日志输出                                                     */
/* -------------------------------------------------------------------------- */
void ulog_output_ex(unsigned char terminal_id,
                                         char level, const char *tag,
                                         const char *fmt, ...)
{
#if !ULOG_OUTPUT_DISABLE
    char buffer[ULOG_BUFFER_SIZE] = {0};
    va_list args;
    va_start(args, fmt);
    int len = format_log_message(buffer, sizeof(buffer), level, tag, fmt, args);
    va_end(args);

    /* 统一分发到各个后端 */
    log_backend_rtt(terminal_id, level, buffer);
    log_backend_easylogger(level, tag, buffer);
    log_backend_printf(level, buffer);
#endif
}

/* -------------------------------------------------------------------------- */
/* Hexdump 辅助函数                                                           */
/* -------------------------------------------------------------------------- */
void ulog_hexdump(const char *name, uint8_t width, const void *buf, uint16_t size)
{
#if !ULOG_OUTPUT_DISABLE
    char line[128];

    // 打印标题
    int len = snprintf(line, sizeof(line), "=== %s ===", name);
    line[len] = '\0';
    log_backend_rtt(ULOG_RTT_TERMINAL_ID, 0, line);
    log_backend_easylogger(0, "", line);
    log_backend_printf(0, line);

    // 打印每行内容
    for (uint16_t i = 0; i < size; i += width) {
        len = snprintf(line, sizeof(line), "%04X: ", i);
        for (uint8_t j = 0; j < width && (i + j) < size; j++) {
            len += snprintf(line + len, sizeof(line) - len, "%02X ", ((uint8_t*)buf)[i + j]);
        }
        line[len] = '\0';
        log_backend_rtt(ULOG_RTT_TERMINAL_ID, 0, line);
        log_backend_easylogger(0, "", line);
        log_backend_printf(0, line);
    }
#endif
}