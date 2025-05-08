/* =================== PIcoOS System Control Center =================== */
/* This file is like the brain of our tiny computer system! */

#ifndef SYSTEM_H    /* This is a special guard that makes sure we only include this file once */
#define SYSTEM_H

#include <stdint.h>   /* This gives us special number types */
#include "os_config.h"  /* This gets our special settings */

/* ===== System Rules Box ===== */
typedef struct {
    uint32_t cpuFrequency;           /* How fast our computer brain thinks (in Hz) */
    uint32_t criticalHeapThreshold;  /* When to warn that we're running out of memory space */
    uint8_t enablePerformanceLogging; /* Should we keep track of how fast we're going? (1=yes, 0=no) */
    uint8_t enableErrorLed;          /* Should we blink a light when there's a problem? (1=yes, 0=no) */
} SystemConfig; /* This box holds all our important system settings */

/* ===== Important System Jobs ===== */

/**
 * Wake up the system and tell it how to behave
 * @param config A box of special rules for the system
 * @return 0 if everything worked, a negative number if there was a problem
 */
int system_init(SystemConfig *config); /* This turns on the brain of our computer */

/**
 * Check if everything is still working okay
 */
void system_update(void); /* This makes sure our computer is still healthy */

/**
 * Tell everyone there's a problem
 * @param error What kind of problem we have
 */
void system_set_error(error_code_t error); /* This is like raising a warning flag */

/**
 * Ask what problem we're having (if any)
 * @return The current problem code (or ERROR_NONE if everything is fine)
 */
error_code_t system_get_error(void); /* This checks if we have any problems */

/* ===== Checking How the System is Doing ===== */

/**
 * Ask how long the computer has been turned on
 * @return How many milliseconds (1/1000ths of a second) the system has been running
 */
uint32_t system_get_uptime(void); /* This is like checking how long a toy has been playing */

/**
 * Ask how hard the computer is working
 * @return A number from 0-100 (like a percentage) showing how hard the brain is working
 */
uint8_t system_get_cpu_usage(void); /* This checks if our computer is working very hard */

/**
 * Ask how much free memory we have left
 * @return How many bytes of memory are still free to use
 */
uint32_t system_get_free_heap(void); /* This checks if we have room for more toys */

/* ===== Secret Helper Functions ===== */
/* These are special functions that only the system itself can use */

static uint8_t calculate_cpu_usage(void);         /* Figure out how hard the computer is working */
static void memory_saving_mode(void);             /* Turn on special power-saving mode when memory is low */
static int initialize_clocks(void);               /* Start all the internal computer clocks */
static void enable_data_cache(void);              /* Turn on special memory tricks for data */
static void enable_instruction_cache(void);       /* Turn on special memory tricks for instructions */
static void enable_instruction_prefetch(void);    /* Make the computer look ahead at what to do next */
static void blink_error_led(error_code_t error);  /* Flash a special light when there's a problem */

#endif /* End of SYSTEM_H - we're done describing the computer's brain! */
