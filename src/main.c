#include "FreeRTOS.h"
#include "task.h"
#include "semphr.h"
#include "os_config.h"
#include "drivers/gpio.h"
#include "drivers/sd_card.h"
#include "drivers/display.h"
#include "drivers/audio.h"
#include "fs/fs_manager.h"
#include "gui/gui_manager.h"
#include "core/system.h"

// Task handles
static TaskHandle_t guiTaskHandle = NULL;
static TaskHandle_t fsTaskHandle = NULL;
static TaskHandle_t audioTaskHandle = NULL;
static TaskHandle_t systemTaskHandle = NULL;

// Semaphores for resource access
static SemaphoreHandle_t sdCardMutex;
static SemaphoreHandle_t displayMutex;
static SemaphoreHandle_t audioMutex;

// System task - manages overall system status and performance
static void systemTask(void *pvParameters) {
    SystemConfig config;
    system_init(&config);
    
    while (1) {
        system_update();
        vTaskDelay(pdMS_TO_TICKS(10)); // 10ms update interval
    }
}

// GUI task - handles display and user interface
static void guiTask(void *pvParameters) {
    if (display_init() != DISPLAY_OK) {
        // Continue without display
        vTaskDelete(NULL);
        return;
    }
    
    gui_init();
    
    while (1) {
        xSemaphoreTake(displayMutex, portMAX_DELAY);
        gui_update();
        xSemaphoreGive(displayMutex);
        vTaskDelay(pdMS_TO_TICKS(16)); // ~60fps
    }
}

// Filesystem task - handles SD card operations
static void fsTask(void *pvParameters) {
    if (sd_card_init() != SD_CARD_OK) {
        // Critical error - cannot continue without filesystem
        system_set_error(ERROR_FS_INIT_FAILED);
        vTaskDelete(NULL);
        return;
    }
    
    if (fs_init() != FS_OK) {
        system_set_error(ERROR_FS_MOUNT_FAILED);
        vTaskDelete(NULL);
        return;
    }
    
    while (1) {
        xSemaphoreTake(sdCardMutex, portMAX_DELAY);
        fs_update();
        xSemaphoreGive(sdCardMutex);
        vTaskDelay(pdMS_TO_TICKS(50)); // 50ms update interval
    }
}

// Audio task - handles audio playback
static void audioTask(void *pvParameters) {
    if (audio_init() != AUDIO_OK) {
        // Continue without audio
        vTaskDelete(NULL);
        return;
    }
    
    while (1) {
        xSemaphoreTake(audioMutex, portMAX_DELAY);
        audio_update();
        xSemaphoreGive(audioMutex);
        vTaskDelay(pdMS_TO_TICKS(5)); // 5ms update interval for smooth audio
    }
}

// Button input callback
static void buttonCallback(uint8_t button_id, button_event_t event) {
    if (event == BUTTON_PRESSED) {
        gui_handle_button_press(button_id);
    } else if (event == BUTTON_RELEASED) {
        gui_handle_button_release(button_id);
    } else if (event == BUTTON_LONG_PRESS) {
        gui_handle_button_long_press(button_id);
    }
}

int main(void) {
    // Initialize hardware
    gpio_init();
    buttons_init(buttonCallback);
    
    // Create semaphores
    sdCardMutex = xSemaphoreCreateMutex();
    displayMutex = xSemaphoreCreateMutex();
    audioMutex = xSemaphoreCreateMutex();
    
    // Create tasks
    xTaskCreate(systemTask, "SYS", SYSTEM_TASK_STACK_SIZE, NULL, SYSTEM_TASK_PRIORITY, &systemTaskHandle);
    xTaskCreate(fsTask, "FS", FS_TASK_STACK_SIZE, NULL, FS_TASK_PRIORITY, &fsTaskHandle);
    xTaskCreate(audioTask, "AUDIO", AUDIO_TASK_STACK_SIZE, NULL, AUDIO_TASK_PRIORITY, &audioTaskHandle);
    
    // GUI task is optional - depends on display presence
    if (OS_CONFIG_ENABLE_GUI) {
        xTaskCreate(guiTask, "GUI", GUI_TASK_STACK_SIZE, NULL, GUI_TASK_PRIORITY, &guiTaskHandle);
    }
    
    // Start FreeRTOS scheduler
    vTaskStartScheduler();
    
    // Should never reach here
    while (1) {}
    
    return 0;
}
