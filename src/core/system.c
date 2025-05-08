#include "core/system.h"
#include "os_config.h"
#include "FreeRTOS.h"
#include "task.h"
#include <stdio.h>
#include <string.h>

// System state
static SystemConfig sysConfig;
static error_code_t currentError = ERROR_NONE;
static uint32_t systemUptime = 0;
static uint8_t cpuUtilization = 0;
static uint32_t freeHeapSize = 0;

// Performance monitoring
static uint32_t lastPerformanceCheck = 0;
static const uint32_t PERFORMANCE_CHECK_INTERVAL = 1000; // ms

// Function declarations for internal functions
static uint8_t calculate_cpu_usage(void);
static void memory_saving_mode(void);
static int initialize_clocks(void);
static void enable_data_cache(void);
static void enable_instruction_cache(void);
static void enable_instruction_prefetch(void);
static void blink_error_led(error_code_t error);

int system_init(SystemConfig *config) {
    memcpy(&sysConfig, config, sizeof(SystemConfig));
    
    // Initialize system clock
    if (!initialize_clocks()) {
        return -1;
    }
    
    // Configure cache and performance optimizations
    if (ENABLE_CACHE_OPTIMIZATION) {
        enable_data_cache();
        enable_instruction_cache();
    }
    
    if (ENABLE_INSTRUCTION_PREFETCH) {
        enable_instruction_prefetch();
    }
    
    printf("PicoOS for RP2350 initialized\n");
    return 0;
}

void system_update(void) {
    systemUptime = xTaskGetTickCount() / (SYSTEM_TICK_RATE_HZ / 1000);
    freeHeapSize = xPortGetFreeHeapSize();
    
    // Periodically check system performance
    uint32_t currentTime = xTaskGetTickCount() / (SYSTEM_TICK_RATE_HZ / 1000);
    if (currentTime - lastPerformanceCheck >= PERFORMANCE_CHECK_INTERVAL) {
        cpuUtilization = calculate_cpu_usage();
        lastPerformanceCheck = currentTime;
        
        // Log system stats periodically if enabled
        if (sysConfig.enablePerformanceLogging) {
            printf("System Stats - Uptime: %lu ms, CPU: %u%%, Free Heap: %lu bytes\n", 
                   systemUptime, cpuUtilization, freeHeapSize);
        }
        
        // Check if memory is critically low
        if (freeHeapSize < sysConfig.criticalHeapThreshold) {
            printf("WARNING: Low memory condition detected! Available: %lu bytes\n", freeHeapSize);
            // Take memory saving actions
            memory_saving_mode();
        }
    }
}

void system_set_error(error_code_t error) {
    currentError = error;
    printf("System Error: %d\n", error);
    
    // Optional: blink LED or other visual indicator of system error
    if (sysConfig.enableErrorLed) {
        blink_error_led(error);
    }
}

error_code_t system_get_error(void) {
    return currentError;
}

uint32_t system_get_uptime(void) {
    return systemUptime;
}

uint8_t system_get_cpu_usage(void) {
    return cpuUtilization;
}

uint32_t system_get_free_heap(void) {
    return freeHeapSize;
}

static uint8_t calculate_cpu_usage(void) {
    // This is a simplified approach, actual implementation would depend on RP2350 specifics
    TaskStatus_t *pxTaskStatusArray;
    volatile UBaseType_t uxArraySize, x;
    uint32_t ulTotalRunTime;
    uint8_t utilization = 0;

    // Get number of tasks
    uxArraySize = uxTaskGetNumberOfTasks();

    // Allocate array to store task status
    pxTaskStatusArray = pvPortMalloc(uxArraySize * sizeof(TaskStatus_t));
    
    if (pxTaskStatusArray != NULL) {
        // Generate the task status
        uxArraySize = uxTaskGetSystemState(pxTaskStatusArray, uxArraySize, &ulTotalRunTime);
        
        // Calculate total time spent executing
        uint32_t totalExecutionTime = 0;
        for (x = 0; x < uxArraySize; x++) {
            totalExecutionTime += pxTaskStatusArray[x].ulRunTimeCounter;
        }
        
        // Calculate utilization as percentage of idle task vs total
        if (ulTotalRunTime > 0) {
            for (x = 0; x < uxArraySize; x++) {
                if (strcmp(pxTaskStatusArray[x].pcTaskName, "IDLE") == 0) {
                    uint32_t idleTime = pxTaskStatusArray[x].ulRunTimeCounter;
                    utilization = 100 - ((idleTime * 100) / ulTotalRunTime);
                    break;
                }
            }
        }
        
        // Free the array
        vPortFree(pxTaskStatusArray);
    }
    
    return utilization;
}

static void memory_saving_mode(void) {
    // Implement memory-saving actions based on your application
    // For example:
    // - Flush caches
    // - Release non-critical resources
    // - Disable optional features
}

static int initialize_clocks(void) {
    // Implement RP2350-specific clock initialization
    // Set CPU speed based on configuration
    uint32_t freq = sysConfig.cpuFrequency;
    if (freq == 0) {
        freq = 125000000; // Default to 125 MHz if not specified
    }
    
    // Example pseudo-code (replace with actual RP2350 SDK functions)
    // set_system_clock_khz(freq / 1000, true);
    
    return 1; // Return success
}

static void enable_data_cache(void) {
    // Implement RP2350-specific data cache initialization
}

static void enable_instruction_cache(void) {
    // Implement RP2350-specific instruction cache initialization
}

static void enable_instruction_prefetch(void) {
    // Implement RP2350-specific instruction prefetch initialization
}

static void blink_error_led(error_code_t error) {
    // Implement error LED blinking pattern based on error code
}
