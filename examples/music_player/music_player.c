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
#include <stdio.h>
#include <string.h>

// Button mappings
#define BUTTON_PLAY_PAUSE 0
#define BUTTON_NEXT       1
#define BUTTON_PREV       2
#define BUTTON_VOL_UP     3
#define BUTTON_VOL_DOWN   4

// GUI elements
static gui_element_t mainScreen = NULL;
static gui_element_t songLabel = NULL;
static gui_element_t statusLabel = NULL;
static gui_element_t progressBar = NULL;
static gui_element_t volumeLabel = NULL;

// Audio state
static audio_state_t currentState = AUDIO_STATE_STOPPED;
static char currentSong[MAX_FILENAME_LENGTH] = "";
static uint32_t currentPosition = 0;
static uint32_t currentDuration = 0;
static uint8_t currentVolume = 70; // Default volume

// Music directory path
static const char *musicDir = "/music";

// Forward declarations
static void update_gui(void);
static void play_song(const char *filename);
static void next_song(void);
static void prev_song(void);
static void handle_button_event(uint8_t button_id, button_event_t event);
static void initialize_gui(void);
static void populate_playlist(void);

// List of songs
static char songList[20][MAX_FILENAME_LENGTH];
static int songCount = 0;
static int currentSongIndex = 0;

// Mutex for protecting audio operations
static SemaphoreHandle_t audioMutex = NULL;

/**
 * Task to update GUI and handle audio state
 */
static void music_player_task(void *pvParameters) {
    uint32_t lastUpdate = 0;
    
    // Initialize playlist
    populate_playlist();
    
    if (songCount == 0) {
        printf("No songs found in %s\n", musicDir);
        if (statusLabel != NULL) {
            gui_set_text(statusLabel, "No songs found!");
        }
    } else {
        // Play first song
        play_song(songList[0]);
    }
    
    while (1) {
        uint32_t currentTime = xTaskGetTickCount() / (SYSTEM_TICK_RATE_HZ / 1000);
        
        // Update position every 500ms
        if (currentTime - lastUpdate >= 500) {
            lastUpdate = currentTime;
            
            if (currentState == AUDIO_STATE_PLAYING) {
                // Get current playback position
                xSemaphoreTake(audioMutex, portMAX_DELAY);
                audio_get_position(&currentPosition);
                xSemaphoreGive(audioMutex);
                
                // Update GUI
                update_gui();
                
                // Check if song finished
                if (currentPosition >= currentDuration && currentDuration > 0) {
                    // Go to next song
                    next_song();
                }
            }
        }
        
        vTaskDelay(pdMS_TO_TICKS(10));
    }
}

/**
 * Initialize the music player application
 */
void music_player_init(void) {
    printf("Initializing Music Player Application\n");
    
    // Create mutex
    audioMutex = xSemaphoreCreateMutex();
    
    // Initialize GUI if display is available
    if (display_is_connected()) {
        initialize_gui();
    }
    
    // Set up buttons
    button_set_long_press_threshold(BUTTON_PLAY_PAUSE, 1000); // 1 second for long press
    
    // Create task
    xTaskCreate(music_player_task, "MUSIC", 2048, NULL, 3, NULL);
    
    printf("Music Player initialized\n");
}

/**
 * Handle button events
 */
static void handle_button_event(uint8_t button_id, button_event_t event) {
    if (event == BUTTON_PRESSED) {
        switch (button_id) {
            case BUTTON_PLAY_PAUSE:
                if (currentState == AUDIO_STATE_PLAYING) {
                    // Pause playback
                    xSemaphoreTake(audioMutex, portMAX_DELAY);
                    audio_pause();
                    currentState = AUDIO_STATE_PAUSED;
                    xSemaphoreGive(audioMutex);
                    
                    if (statusLabel != NULL) {
                        gui_set_text(statusLabel, "Paused");
                    }
                    printf("Playback paused\n");
                } else if (currentState == AUDIO_STATE_PAUSED) {
                    // Resume playback
                    xSemaphoreTake(audioMutex, portMAX_DELAY);
                    audio_resume();
                    currentState = AUDIO_STATE_PLAYING;
                    xSemaphoreGive(audioMutex);
                    
                    if (statusLabel != NULL) {
                        gui_set_text(statusLabel, "Playing");
                    }
                    printf("Playback resumed\n");
                } else if (currentState == AUDIO_STATE_STOPPED && songCount > 0) {
                    // Start playback
                    play_song(songList[currentSongIndex]);
                }
                break;
                
            case BUTTON_NEXT:
                next_song();
                break;
                
            case BUTTON_PREV:
                prev_song();
                break;
                
            case BUTTON_VOL_UP:
                // Increase volume
                if (currentVolume < 100) {
                    currentVolume += 5;
                    xSemaphoreTake(audioMutex, portMAX_DELAY);
                    audio_set_volume(currentVolume);
                    xSemaphoreGive(audioMutex);
                    
                    // Update volume display
                    char volText[20];
                    snprintf(volText, sizeof(volText), "Volume: %d%%", currentVolume);
                    if (volumeLabel != NULL) {
                        gui_set_text(volumeLabel, volText);
                    }
                    printf("%s\n", volText);
                }
                break;
                
            case BUTTON_VOL_DOWN:
                // Decrease volume
                if (currentVolume >= 5) {
                    currentVolume -= 5;
                    xSemaphoreTake(audioMutex, portMAX_DELAY);
                    audio_set_volume(currentVolume);
                    xSemaphoreGive(audioMutex);
                    
                    // Update volume display
                    char volText[20];
                    snprintf(volText, sizeof(volText), "Volume: %d%%", currentVolume);
                    if (volumeLabel != NULL) {
                        gui_set_text(volumeLabel, volText);
                    }
                    printf("%s\n", volText);
                }
                break;
        }
    } else if (event == BUTTON_LONG_PRESS) {
        if (button_id == BUTTON_PLAY_PAUSE) {
            // Stop playback
            xSemaphoreTake(audioMutex, portMAX_DELAY);
            audio_stop();
            currentState = AUDIO_STATE_STOPPED;
            currentPosition = 0;
            xSemaphoreGive(audioMutex);
            
            if (statusLabel != NULL) {
                gui_set_text(statusLabel, "Stopped");
            }
            if (progressBar != NULL) {
                gui_set_value(progressBar, 0);
            }
            printf("Playback stopped\n");
        }
    }
}

/**
 * Initialize GUI elements
 */
static void initialize_gui(void) {
    gui_init();
    
    // Create main screen
    mainScreen = gui_create_screen();
    
    // Create song label
    songLabel = gui_create_element(GUI_ELEMENT_LABEL, mainScreen);
    gui_set_position(songLabel, 10, 10);
    gui_set_text(songLabel, "No song selected");
    
    // Create status label
    statusLabel = gui_create_element(GUI_ELEMENT_LABEL, mainScreen);
    gui_set_position(statusLabel, 10, 40);
    gui_set_text(statusLabel, "Stopped");
    
    // Create progress bar
    progressBar = gui_create_element(GUI_ELEMENT_PROGRESS, mainScreen);
    gui_set_position(progressBar, 10, 70);
    gui_set_size(progressBar, 220, 20);
    gui_set_value(progressBar, 0);
    
    // Create volume label
    volumeLabel = gui_create_element(GUI_ELEMENT_LABEL, mainScreen);
    gui_set_position(volumeLabel, 10, 100);
    char volText[20];
    snprintf(volText, sizeof(volText), "Volume: %d%%", currentVolume);
    gui_set_text(volumeLabel, volText);
    
    // Add button hints at bottom of screen
    gui_element_t buttonHints = gui_create_element(GUI_ELEMENT_LABEL, mainScreen);
    gui_set_position(buttonHints, 10, 160);
    gui_set_text(buttonHints, "B1: Play/Pause | B2: Next | B3: Prev | B4: Vol+ | B5: Vol-");
    
    // Set main screen active
    gui_set_screen(mainScreen);
}

/**
 * Update GUI elements with current state
 */
static void update_gui(void) {
    if (mainScreen == NULL) {
        return; // No GUI available
    }
    
    // Update progress bar
    if (progressBar != NULL && currentDuration > 0) {
        int progress = (currentPosition * 100) / currentDuration;
        gui_set_value(progressBar, progress);
    }
}

/**
 * Play specified song
 */
static void play_song(const char *filename) {
    if (filename == NULL || strlen(filename) == 0) {
        return;
    }
    
    // Stop current playback if any
    xSemaphoreTake(audioMutex, portMAX_DELAY);
    audio_stop();
    xSemaphoreGive(audioMutex);
    
    // Construct full path
    char fullPath[MAX_PATH_LENGTH];
    snprintf(fullPath, sizeof(fullPath), "%s/%s", musicDir, filename);
    
    // Update song info
    strncpy(currentSong, filename, sizeof(currentSong) - 1);
    currentSong[sizeof(currentSong) - 1] = '\0';
    
    // Start playback
    xSemaphoreTake(audioMutex, portMAX_DELAY);
    audio_status_t status = audio_play_file(fullPath);
    
    if (status == AUDIO_OK) {
        currentState = AUDIO_STATE_PLAYING;
        audio_get_duration(&currentDuration);
        currentPosition = 0;
        audio_set_volume(currentVolume);
    } else {
        currentState = AUDIO_STATE_STOPPED;
        printf("Error playing file: %d\n", status);
    }
    xSemaphoreGive(audioMutex);
    
    // Update GUI
    if (songLabel != NULL) {
        gui_set_text(songLabel, currentSong);
    }
    
    if (statusLabel != NULL) {
        gui_set_text(statusLabel, (currentState == AUDIO_STATE_PLAYING) ? "Playing" : "Error");
    }
    
    if (progressBar != NULL) {
        gui_set_value(progressBar, 0);
    }
    
    printf("Now playing: %s\n", currentSong);
}

/**
 * Play next song in playlist
 */
static void next_song(void) {
    if (songCount == 0) {
        return;
    }
    
    currentSongIndex = (currentSongIndex + 1) % songCount;
    play_song(songList[currentSongIndex]);
}

/**
 * Play previous song in playlist
 */
static void prev_song(void) {
    if (songCount == 0) {
        return;
    }
    
    currentSongIndex = (currentSongIndex > 0) ? (currentSongIndex - 1) : (songCount - 1);
    play_song(songList[currentSongIndex]);
}

/**
 * Populate playlist from music directory
 */
static void populate_playlist(void) {
    fs_dir_t dir;
    fs_file_info_t info;
    
    songCount = 0;
    
    // Open music directory
    if (fs_opendir(musicDir, &dir) != FS_OK) {
        printf("Failed to open music directory: %s\n", musicDir);
        
        // Try to create the directory
        fs_mkdir(musicDir);
        return;
    }
    
    // Read directory entries
    while (fs_readdir(dir, &info) == FS_OK && songCount < 20) {
        // Check if it's a file (not directory)
        if (!info.is_dir) {
            // Check file extension for audio files
            const char *ext = strrchr(info.name, '.');
            if (ext != NULL) {
                if (strcasecmp(ext, ".mp3") == 0 || 
                    strcasecmp(ext, ".wav") == 0 || 
                    strcasecmp(ext, ".ogg") == 0) {
                    // Add to song list
                    strncpy(songList[songCount], info.name, MAX_FILENAME_LENGTH - 1);
                    songList[songCount][MAX_FILENAME_LENGTH - 1] = '\0';
                    songCount++;
                }
            }
        }
    }
    
    // Close directory
    fs_closedir(dir);
    
    printf("Found %d songs in %s\n", songCount, musicDir);
}

/**
 * Main function
 */
int main(void) {
    // Initialize hardware and OS
    gpio_init();
    buttons_init(handle_button_event);
    
    // Initialize system
    SystemConfig sysConfig;
    memset(&sysConfig, 0, sizeof(SystemConfig));
    sysConfig.cpuFrequency = 125000000; // 125MHz
    sysConfig.criticalHeapThreshold = 4096; // 4KB
    sysConfig.enablePerformanceLogging = 1;
    sysConfig.enableErrorLed = 1;
    system_init(&sysConfig);
    
    // Initialize audio (required)
    if (audio_init() != AUDIO_OK) {
        printf("Failed to initialize audio system\n");
        // Continue anyway (audio is critical for this app, but we won't crash)
    }
    
    // Initialize SD card and file system (required)
    if (sd_card_init() != SD_CARD_OK || fs_init() != FS_OK) {
        printf("Failed to initialize SD card or file system\n");
        system_set_error(ERROR_FS_INIT_FAILED);
        while (1) {
            // Fatal error - can't continue without file system
            vTaskDelay(pdMS_TO_TICKS(1000));
        }
    }
    
    // Initialize display (optional)
    display_init(); // Ignore errors - display is optional
    
    // Initialize and start the music player
    music_player_init();
    
    // Start FreeRTOS scheduler
    vTaskStartScheduler();
    
    // Should never reach here
    while (1) {}
    
    return 0;
}
