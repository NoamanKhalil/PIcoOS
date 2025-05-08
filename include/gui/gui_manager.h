#ifndef GUI_MANAGER_H
#define GUI_MANAGER_H

#include <stdint.h>
#include "drivers/display.h"

// GUI status codes
typedef enum {
    GUI_OK = 0,
    GUI_ERROR_INIT,
    GUI_ERROR_NO_DISPLAY,
    GUI_ERROR_MEMORY,
    GUI_ERROR_PARAM,
    GUI_ERROR_THEME
} gui_status_t;

// GUI theme types
typedef enum {
    GUI_THEME_LIGHT = 0,
    GUI_THEME_DARK,
    GUI_THEME_CUSTOM
} gui_theme_t;

// UI element types
typedef enum {
    GUI_ELEMENT_WINDOW = 0,
    GUI_ELEMENT_BUTTON,
    GUI_ELEMENT_LABEL,
    GUI_ELEMENT_CHECKBOX,
    GUI_ELEMENT_SLIDER,
    GUI_ELEMENT_SWITCH,
    GUI_ELEMENT_DROPDOWN,
    GUI_ELEMENT_TEXTAREA,
    GUI_ELEMENT_SPINNER,
    GUI_ELEMENT_PROGRESS,
    GUI_ELEMENT_IMAGE,
    GUI_ELEMENT_CHART,
    GUI_ELEMENT_GAUGE,
    GUI_ELEMENT_KEYBOARD
} gui_element_type_t;

// Opaque handle for UI elements
typedef void* gui_element_t;

// GUI event types
typedef enum {
    GUI_EVENT_CLICK = 0,
    GUI_EVENT_LONG_PRESS,
    GUI_EVENT_VALUE_CHANGED,
    GUI_EVENT_SELECTED,
    GUI_EVENT_FOCUSED,
    GUI_EVENT_DEFOCUSED,
    GUI_EVENT_KEY,
    GUI_EVENT_SCROLL
} gui_event_type_t;

// GUI event data
typedef struct {
    gui_event_type_t type;     // Event type
    gui_element_t target;      // Target element
    gui_element_t source;      // Source element (if applicable)
    union {
        int32_t value;         // Numeric value
        const char* text;      // Text value
        struct {               // Position data
            int16_t x;
            int16_t y;
        } pos;
        uint32_t key;          // Key code
    } data;
} gui_event_t;

// GUI event callback type
typedef void (*gui_event_callback_t)(gui_event_t* event);

/**
 * Initialize the GUI system
 * @return Status code
 */
gui_status_t gui_init(void);

/**
 * Deinitialize the GUI system
 */
void gui_deinit(void);

/**
 * Update GUI system - call periodically
 */
void gui_update(void);

/**
 * Set GUI theme
 * @param theme Theme type
 * @return Status code
 */
gui_status_t gui_set_theme(gui_theme_t theme);

/**
 * Create a new UI element
 * @param type Element type
 * @param parent Parent element (NULL for screen)
 * @return Handle to created element or NULL on failure
 */
gui_element_t gui_create_element(gui_element_type_t type, gui_element_t parent);

/**
 * Delete a UI element
 * @param element Element handle
 */
void gui_delete_element(gui_element_t element);

/**
 * Set element position
 * @param element Element handle
 * @param x X coordinate
 * @param y Y coordinate
 * @return Status code
 */
gui_status_t gui_set_position(gui_element_t element, int16_t x, int16_t y);

/**
 * Set element size
 * @param element Element handle
 * @param width Width
 * @param height Height
 * @return Status code
 */
gui_status_t gui_set_size(gui_element_t element, uint16_t width, uint16_t height);

/**
 * Set element text
 * @param element Element handle
 * @param text Text string
 * @return Status code
 */
gui_status_t gui_set_text(gui_element_t element, const char* text);

/**
 * Set element value
 * @param element Element handle
 * @param value Numeric value
 * @return Status code
 */
gui_status_t gui_set_value(gui_element_t element, int32_t value);

/**
 * Get element value
 * @param element Element handle
 * @return Element value
 */
int32_t gui_get_value(gui_element_t element);

/**
 * Set element color
 * @param element Element handle
 * @param color Color value
 * @param part Element part (0 for main)
 * @return Status code
 */
gui_status_t gui_set_color(gui_element_t element, display_color_t color, uint8_t part);

/**
 * Register event callback for element
 * @param element Element handle
 * @param event_type Event type to listen for
 * @param callback Callback function
 * @return Status code
 */
gui_status_t gui_register_event(gui_element_t element, gui_event_type_t event_type, gui_event_callback_t callback);

/**
 * Create a message box
 * @param title Title text
 * @param message Message text
 * @param buttons Button text array (NULL-terminated)
 * @param callback Callback for button press
 * @return Handle to created message box or NULL on failure
 */
gui_element_t gui_create_msgbox(const char* title, const char* message, const char** buttons, gui_event_callback_t callback);

/**
 * Load an image from file
 * @param filename Path to image file
 * @return Handle to image or NULL on failure
 */
void* gui_load_image(const char* filename);

/**
 * Set image for element
 * @param element Element handle
 * @param image Image handle
 * @return Status code
 */
gui_status_t gui_set_image(gui_element_t element, void* image);

/**
 * Handle button press event from hardware button
 * @param button_id Button ID
 */
void gui_handle_button_press(uint8_t button_id);

/**
 * Handle button release event from hardware button
 * @param button_id Button ID
 */
void gui_handle_button_release(uint8_t button_id);

/**
 * Handle button long press event from hardware button
 * @param button_id Button ID
 */
void gui_handle_button_long_press(uint8_t button_id);

/**
 * Create a new screen
 * @return Handle to created screen or NULL on failure
 */
gui_element_t gui_create_screen(void);

/**
 * Set active screen
 * @param screen Screen handle
 * @return Status code
 */
gui_status_t gui_set_screen(gui_element_t screen);

/**
 * Get active screen
 * @return Handle to active screen
 */
gui_element_t gui_get_active_screen(void);

/**
 * Set focus to element
 * @param element Element handle
 * @return Status code
 */
gui_status_t gui_focus_element(gui_element_t element);

/**
 * Get focused element
 * @return Handle to focused element or NULL if none
 */
gui_element_t gui_get_focused_element(void);

#endif // GUI_MANAGER_H
