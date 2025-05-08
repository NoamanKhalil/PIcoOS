/* =================== PIcoOS Configuration File =================== */
/* This file is like the settings page for our tiny computer! */

#ifndef OS_CONFIG_H   /* This is a special guard that makes sure we only include this file once */
#define OS_CONFIG_H

#include <stdint.h>   /* This gives us special number types */

/* ===== Turn Special Features On or Off ===== */
// Feature configuration - like light switches for different parts of our system
#define OS_CONFIG_ENABLE_GUI        1   /* 1 means ON, 0 means OFF - this is for pretty pictures */
#define OS_CONFIG_ENABLE_AUDIO      1   /* 1 means ON, 0 means OFF - this is for making sounds */
#define OS_CONFIG_ENABLE_SDCARD     1   /* 1 means ON, 0 means OFF - this is for saving files */

/* ===== Who Gets to Go First? ===== */
// Task priorities (higher number = higher priority) - like deciding which job is more important
#define SYSTEM_TASK_PRIORITY        5   /* The system boss gets highest priority (5) - most important! */
#define FS_TASK_PRIORITY            4   /* The file organizer gets next priority (4) */
#define AUDIO_TASK_PRIORITY         3   /* The sound player gets medium priority (3) */
#define GUI_TASK_PRIORITY           2   /* The screen drawer gets lowest priority (2) */

/* ===== How Much Memory Each Job Gets ===== */
// Task stack sizes (in words) - like giving each worker their own desk space
#define SYSTEM_TASK_STACK_SIZE      512    /* The system boss gets a small desk (512 words) */
#define FS_TASK_STACK_SIZE          1024   /* The file organizer gets a medium desk (1024 words) */
#define AUDIO_TASK_STACK_SIZE       1024   /* The sound player gets a medium desk (1024 words) */
#define GUI_TASK_STACK_SIZE         2048   /* The screen drawer gets the biggest desk (2048 words) because drawing needs space! */

/* ===== Other Important Settings ===== */
// System configuration - other rules for our computer
#define SYSTEM_TICK_RATE_HZ         1000   /* How many times per second our clock ticks (1000 = 1000 times!) */
#define MAX_FILENAME_LENGTH         128    /* How long a file name can be (128 letters maximum) */
#define MAX_PATH_LENGTH             256    /* How long a file path can be (256 letters maximum) */

/* ===== Problem Codes ===== */
// Error codes - like special names for different problems that might happen
typedef enum {
    ERROR_NONE = 0,                  /* No problem at all! Everything is working fine */
    ERROR_FS_INIT_FAILED,            /* Problem starting the memory card */
    ERROR_FS_MOUNT_FAILED,           /* Problem setting up our filing system */
    ERROR_DISPLAY_INIT_FAILED,       /* Problem turning on the screen */
    ERROR_AUDIO_INIT_FAILED,         /* Problem turning on the speaker */
    ERROR_OUT_OF_MEMORY,             /* Problem: we ran out of memory space! */
    ERROR_HARDWARE_FAILURE           /* Problem with the computer parts */
} error_code_t;

/* ===== Speed Boost Settings ===== */
// Performance optimization settings - like special tricks to make our computer faster
#define ENABLE_CACHE_OPTIMIZATION   1   /* Turn ON memory tricks to go faster */
#define ENABLE_DMA_TRANSFERS        1   /* Turn ON special data moving tricks */
#define ENABLE_INSTRUCTION_PREFETCH 1   /* Turn ON looking ahead at instructions */

/* ===== Memory Space Settings ===== */
// Memory management - how much space we have for toys
#define HEAP_SIZE                   (64 * 1024)  /* 64KB (that's 65,536 bytes!) of memory for all our needs */

#endif /* End of OS_CONFIG_H - remember that special guard from the beginning? This is its matching end! */
