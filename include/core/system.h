#ifndef SYSTEM_H
#define SYSTEM_H

#include <stdint.h>
#include "os_config.h"

typedef struct {
    uint32_t cpuFrequency;           // CPU clock frequency in Hz
    uint32_t criticalHeapThreshold;  // Threshold for low memory warning in bytes
    uint8_t enablePerformanceLogging; // Flag to enable/disable performance logging
    uint8_t enableErrorLed;          // Flag to enable/disable error LED
} SystemConfig;

/**
 * Initialize the system with the given configuration
 * @param config System configuration parameters
 * @return 0 on success, negative value on failure
 */
int system_init(SystemConfig *config);

/**
 * Update system state - call periodically from system task
 */
void system_update(void);

/**
 * Set system error status
 * @param error Error code
 */
void system_set_error(error_code_t error);

/**
 * Get current system error status
 * @return Current error code
 */
error_code_t system_get_error(void);

/**
 * Get system uptime in milliseconds
 * @return Uptime in ms
 */
uint32_t system_get_uptime(void);

/**
 * Get CPU usage percentage
 * @return CPU usage (0-100%)
 */
uint8_t system_get_cpu_usage(void);

/**
 * Get available heap memory
 * @return Free heap in bytes
 */
uint32_t system_get_free_heap(void);

// Internal functions (static in implementation)
static uint8_t calculate_cpu_usage(void);
static void memory_saving_mode(void);
static int initialize_clocks(void);
static void enable_data_cache(void);
static void enable_instruction_cache(void);
static void enable_instruction_prefetch(void);
static void blink_error_led(error_code_t error);

#endif // SYSTEM_H
