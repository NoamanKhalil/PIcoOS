#ifndef OS_CONFIG_H
#define OS_CONFIG_H

#include <stdint.h>

// Feature configuration
#define OS_CONFIG_ENABLE_GUI        1   // Enable/disable GUI subsystem
#define OS_CONFIG_ENABLE_AUDIO      1   // Enable/disable audio playback
#define OS_CONFIG_ENABLE_SDCARD     1   // Enable/disable SD card support

// Task priorities (higher number = higher priority)
#define SYSTEM_TASK_PRIORITY        5
#define FS_TASK_PRIORITY            4
#define AUDIO_TASK_PRIORITY         3
#define GUI_TASK_PRIORITY           2

// Task stack sizes (in words)
#define SYSTEM_TASK_STACK_SIZE      512
#define FS_TASK_STACK_SIZE          1024
#define AUDIO_TASK_STACK_SIZE       1024
#define GUI_TASK_STACK_SIZE         2048

// System configuration
#define SYSTEM_TICK_RATE_HZ         1000
#define MAX_FILENAME_LENGTH         128
#define MAX_PATH_LENGTH             256

// Error codes
typedef enum {
    ERROR_NONE = 0,
    ERROR_FS_INIT_FAILED,
    ERROR_FS_MOUNT_FAILED,
    ERROR_DISPLAY_INIT_FAILED,
    ERROR_AUDIO_INIT_FAILED,
    ERROR_OUT_OF_MEMORY,
    ERROR_HARDWARE_FAILURE
} error_code_t;

// Performance optimization settings
#define ENABLE_CACHE_OPTIMIZATION   1
#define ENABLE_DMA_TRANSFERS        1
#define ENABLE_INSTRUCTION_PREFETCH 1

// Memory management
#define HEAP_SIZE                   (64 * 1024)  // 64KB heap for dynamic allocation

#endif // OS_CONFIG_H
