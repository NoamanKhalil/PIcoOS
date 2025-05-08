/* =================== PIcoOS Main File =================== */
/* This is the main brain of our tiny computer system! */

/* These are like instruction books that tell our program how to use different parts */
#include "FreeRTOS.h"      /* This helps us do many things at once */
#include "task.h"         /* This helps us create different jobs */
#include "semphr.h"       /* This helps us share things nicely without fighting */
#include "os_config.h"    /* This has all our special settings */
#include "drivers/gpio.h" /* This lets us control buttons and lights */
#include "drivers/sd_card.h"  /* This lets us read and save files */
#include "drivers/display.h"  /* This helps us show pictures on screen */
#include "drivers/audio.h"    /* This lets us play sounds */
#include "fs/fs_manager.h"    /* This keeps our files organized */
#include "gui/gui_manager.h"  /* This makes the screen look nice */
#include "core/system.h"      /* This controls the whole system */

/* ===== Special helpers for our different jobs ===== */
/* Think of these as name tags for different workers */
// Task handles
static TaskHandle_t guiTaskHandle = NULL;      /* The worker who draws on screen */
static TaskHandle_t fsTaskHandle = NULL;       /* The worker who organizes files */
static TaskHandle_t audioTaskHandle = NULL;    /* The worker who plays sounds */
static TaskHandle_t systemTaskHandle = NULL;   /* The boss worker who checks on everything */

/* ===== Special tickets for sharing toys ===== */
/* These are like tickets that say "it's my turn to use this toy" */
// Semaphores for resource access
static SemaphoreHandle_t sdCardMutex;    /* Ticket for using the memory card */
static SemaphoreHandle_t displayMutex;   /* Ticket for drawing on the screen */
static SemaphoreHandle_t audioMutex;     /* Ticket for playing sounds */

/* ===== The Boss Job ===== */
// System task - manages overall system status and performance
static void systemTask(void *pvParameters) {
    SystemConfig config;             /* This is like a list of rules */
    system_init(&config);           /* Start up the brain of our system */
    
    /* This is like saying "keep doing this forever and ever" */
    while (1) {
        system_update();            /* Check if everything is okay */
        vTaskDelay(pdMS_TO_TICKS(10)); /* Take a tiny nap (10ms) before checking again */
    }
}

/* ===== The Drawing Job ===== */
// GUI task - handles display and user interface
static void guiTask(void *pvParameters) {
    /* Try to turn on the screen */
    if (display_init() != DISPLAY_OK) {
        // If we can't turn on the screen, stop this job
        vTaskDelete(NULL);        /* This is like saying "I quit!" */
        return;
    }
    
    gui_init();                    /* Get our crayons and paper ready */
    
    /* Keep drawing forever */
    while (1) {
        xSemaphoreTake(displayMutex, portMAX_DELAY);  /* Get the ticket to use the screen */
        gui_update();                                  /* Draw new pictures */
        xSemaphoreGive(displayMutex);                  /* Give the ticket back so others can use the screen */
        vTaskDelay(pdMS_TO_TICKS(16));                 /* Take a tiny nap (16ms) - this makes about 60 pictures every second */
    }
}

/* ===== The File Organizing Job ===== */
// Filesystem task - handles SD card operations
static void fsTask(void *pvParameters) {
    /* Try to turn on the memory card */
    if (sd_card_init() != SD_CARD_OK) {
        // This is a big problem - we need the memory card!
        system_set_error(ERROR_FS_INIT_FAILED);  /* Tell everyone there's a problem */
        vTaskDelete(NULL);                       /* Stop this job */
        return;
    }
    
    /* Try to set up our filing system */
    if (fs_init() != FS_OK) {
        system_set_error(ERROR_FS_MOUNT_FAILED);  /* Tell everyone there's a problem */
        vTaskDelete(NULL);                       /* Stop this job */
        return;
    }
    
    /* Keep organizing files forever */
    while (1) {
        xSemaphoreTake(sdCardMutex, portMAX_DELAY);  /* Get the ticket to use the memory card */
        fs_update();                                  /* Check if any files need attention */
        xSemaphoreGive(sdCardMutex);                  /* Give the ticket back so others can use the memory card */
        vTaskDelay(pdMS_TO_TICKS(50));                /* Take a nap (50ms) before checking again */
    }
}

/* ===== The Sound Playing Job ===== */
// Audio task - handles audio playback
static void audioTask(void *pvParameters) {
    /* Try to turn on the speaker */
    if (audio_init() != AUDIO_OK) {
        // If we can't make sounds, stop this job
        vTaskDelete(NULL);       /* This is like saying "I quit!" */
        return;
    }
    
    /* Keep playing sounds forever */
    while (1) {
        xSemaphoreTake(audioMutex, portMAX_DELAY);  /* Get the ticket to use the speaker */
        audio_update();                              /* Make the next sound */
        xSemaphoreGive(audioMutex);                  /* Give the ticket back so others can use the speaker */
        vTaskDelay(pdMS_TO_TICKS(5));                /* Take a very tiny nap (5ms) to make smooth sounds */
    }
}

/* ===== The Button Listener ===== */
// Button input callback - this is like a doorbell that rings when buttons are pressed
static void buttonCallback(uint8_t button_id, button_event_t event) {
    /* Check what happened to the button */
    if (event == BUTTON_PRESSED) {
        gui_handle_button_press(button_id);     /* Tell the screen someone pressed a button */
    } else if (event == BUTTON_RELEASED) {
        gui_handle_button_release(button_id);   /* Tell the screen someone let go of a button */
    } else if (event == BUTTON_LONG_PRESS) {
        gui_handle_button_long_press(button_id); /* Tell the screen someone held a button for a long time */
    }
}

/* ===== The Starting Point - This is where everything begins! ===== */
int main(void) {
    /* First, wake up all the hardware parts */
    gpio_init();                              /* Wake up the buttons and lights */
    buttons_init(buttonCallback);             /* Tell buttons to ring our doorbell when pressed */
    
    /* Make the tickets for sharing toys */
    sdCardMutex = xSemaphoreCreateMutex();    /* Make a ticket for using the memory card */
    displayMutex = xSemaphoreCreateMutex();   /* Make a ticket for using the screen */
    audioMutex = xSemaphoreCreateMutex();     /* Make a ticket for using the speaker */
    
    /* Hire workers for different jobs */
    xTaskCreate(systemTask, "SYS", SYSTEM_TASK_STACK_SIZE, NULL, SYSTEM_TASK_PRIORITY, &systemTaskHandle);
    /* ⬆️ Hire the boss worker to check on everything */
    
    xTaskCreate(fsTask, "FS", FS_TASK_STACK_SIZE, NULL, FS_TASK_PRIORITY, &fsTaskHandle);
    /* ⬆️ Hire a worker to organize files */
    
    xTaskCreate(audioTask, "AUDIO", AUDIO_TASK_STACK_SIZE, NULL, AUDIO_TASK_PRIORITY, &audioTaskHandle);
    /* ⬆️ Hire a worker to play sounds */
    
    /* Only hire a drawing worker if we have a screen */
    if (OS_CONFIG_ENABLE_GUI) {
        xTaskCreate(guiTask, "GUI", GUI_TASK_STACK_SIZE, NULL, GUI_TASK_PRIORITY, &guiTaskHandle);
        /* ⬆️ Hire a worker to draw pictures */
    }
    
    /* Start all the workers on their jobs! */
    vTaskStartScheduler();
    
    /* We should never reach this point - if we do, something went very wrong! */
    while (1) {}
    
    return 0;
    /* ⬆️ This is like saying "The End" - but we never actually get here */
}
