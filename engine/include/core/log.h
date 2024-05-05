#pragma once
#include <stdio.h>

#ifdef __EMSCRIPTEN__

#define ANSI_COLOR_RED
#define ANSI_COLOR_GREEN
#define ANSI_COLOR_YELLOW
#define ANSI_COLOR_BLUE
#define ANSI_COLOR_MAGENTA
#define ANSI_COLOR_CYAN
#define ANSI_COLOR_RESET

#else

#define ANSI_COLOR_RED "\x1b[31m"
#define ANSI_COLOR_GREEN "\x1b[32m"
#define ANSI_COLOR_YELLOW "\x1b[33m"
#define ANSI_COLOR_BLUE "\x1b[34m"
#define ANSI_COLOR_MAGENTA "\x1b[35m"
#define ANSI_COLOR_CYAN "\x1b[36m"
#define ANSI_COLOR_RESET "\x1b[0m"

#endif

#define log_message_internal(color, severity, message, ...)                               \
  do                                                                                      \
  {                                                                                       \
    printf(color severity ": " message ANSI_COLOR_RESET "\n" __VA_OPT__(, ) __VA_ARGS__); \
  } while (0)

#define log_critical(message, ...) log_message_internal(ANSI_COLOR_MAGENTA, "CRITICAL", message __VA_OPT__(, ) __VA_ARGS__)
#define log_error(message, ...) log_message_internal(ANSI_COLOR_RED, "ERROR", message __VA_OPT__(, ) __VA_ARGS__)
#define log_warn(message, ...) log_message_internal(ANSI_COLOR_YELLOW, "WARNING", message __VA_OPT__(, ) __VA_ARGS__)
#define log_info(message, ...) log_message_internal(ANSI_COLOR_GREEN, "INFO", message __VA_OPT__(, ) __VA_ARGS__)
#define log_debug(message, ...) log_message_internal(ANSI_COLOR_CYAN, "DEBUG", message __VA_OPT__(, ) __VA_ARGS__)
#define log_trace(message, ...) log_message_internal(ANSI_COLOR_RESET, "TRACE", message __VA_OPT__(, ) __VA_ARGS__)

