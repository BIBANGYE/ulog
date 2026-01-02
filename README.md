# ULog  
**Unified Logging Framework for Embedded Systems**

[![License: MIT](https://img.shields.io/badge/License-MIT-blue.svg)](LICENSE)  
[![Platform](https://img.shields.io/badge/Platform-STM32-green.svg)]  
[![Build Status](https://img.shields.io/badge/Status-Beta-yellow.svg)]

ULog 是一款 **面向嵌入式系统的统一日志框架**，专为 **STM32 / Cortex-M** 等 MCU 设计，旨在解决多日志系统并存、接口混乱和维护困难的问题。ULog 提供 **统一日志接口 + 多后端适配能力**，支持 **SEGGER RTT、EasyLogger、UART printf** 等输出方式，适用于 **开发调试、量产和长期维护** 的嵌入式项目。

## ✨ Features
- 🔀 **多后端统一输出**  
  - SEGGER RTT  
  - EasyLogger  
  - UART / printf  
  - 支持多个后端同时启用
- 🧱 **统一日志接口**  
  - 类 Android logcat 风格  
  - 统一日志级别、模块标签  
- 🎨 **彩色日志输出（可选）**  
  - RTT / UART 支持 ANSI Color  
  - EasyLogger 原生输出保留兼容性
- 🕒 **可选时间戳**  
  - 毫秒 / d-h:m:s^ms 格式  
  - 可自定义时间源（HAL / FreeRTOS / RTC）
- 🧩 **模块化设计**  
  - Header-only 接口  
  - 后端解耦，易扩展  
  - 适配 BSP / Middleware / App 分层架构
- 🧠 **工程级可维护性**  
  - 编译期裁剪，零运行开销  
  - 可完全关闭日志  
  - 支持 Hexdump 输出  

## 📋 Log Levels
| Level   | Macro      | Description       |
| ------- | ---------- | ----------------- |
| ASSERT  | `ULOG_A()` | Critical / Assert |
| ERROR   | `ULOG_E()` | Error             |
| WARN    | `ULOG_W()` | Warning           |
| INFO    | `ULOG_I()` | Information       |
| DEBUG   | `ULOG_D()` | Debug             |
| VERBOSE | `ULOG_V()` | Verbose           |

## 🚀 Quick Start
### Basic Usage
```c
#define ULOG_TAG "MAIN"
ULOG_I("System init done");
ULOG_W("Low voltage detected");
ULOG_E("Device error: %d", err);
```
