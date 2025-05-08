/* =================== PIcoOS Pretty Picture Maker =================== */
/* This file helps us make the screen look nice with buttons and pictures! */

#ifndef GUI_MANAGER_H    /* This is a special guard that makes sure we only include this file once */
#define GUI_MANAGER_H

#include <stdint.h>          /* This gives us special number types */
#include "drivers/display.h"  /* This lets us talk to the screen */

/* ===== Picture Maker Problem Messages ===== */
// GUI status codes - messages about how our picture maker is doing
typedef enum {
    GUI_OK = 0,              /* Everything is working fine! */
    GUI_ERROR_INIT,          /* Problem starting the picture maker */
    GUI_ERROR_NO_DISPLAY,    /* We can't find a screen to draw on */
    GUI_ERROR_MEMORY,        /* Not enough memory for pretty pictures */
    GUI_ERROR_PARAM,         /* Someone gave wrong instructions */
    GUI_ERROR_THEME          /* Problem with the coloring style */
} gui_status_t;

/* ===== Color Styles for the Screen ===== */
// GUI theme types - different color styles for our screen
typedef enum {
    GUI_THEME_LIGHT = 0,    /* Bright colors with white background - like daytime */
    GUI_THEME_DARK,         /* Dark colors with black background - like nighttime */
    GUI_THEME_CUSTOM        /* Special colors we make ourselves - like rainbow time! */
} gui_theme_t;

/* ===== Types of Things We Can Put On Screen ===== */
// UI element types - different things we can draw on the screen
typedef enum {
    GUI_ELEMENT_WINDOW = 0,   /* A box that holds other things - like a picture frame */
    GUI_ELEMENT_BUTTON,       /* Something you can press - like a doorbell */
    GUI_ELEMENT_LABEL,        /* Words on the screen - like a name tag */
    GUI_ELEMENT_CHECKBOX,     /* A box you can check - like on a to-do list */
    GUI_ELEMENT_SLIDER,       /* A control you can slide - like a volume knob */
    GUI_ELEMENT_SWITCH,       /* Something you can turn on/off - like a light switch */
    GUI_ELEMENT_DROPDOWN,     /* A list that drops down - like a toy chest */
    GUI_ELEMENT_TEXTAREA,     /* A big space for lots of words - like a notebook */
    GUI_ELEMENT_SPINNER,      /* Numbers that go up and down - like counting */
    GUI_ELEMENT_PROGRESS,     /* A bar that fills up - like filling a cup */
    GUI_ELEMENT_IMAGE,        /* A picture - like a photograph */
    GUI_ELEMENT_CHART,        /* A special picture with numbers - like a growth chart */
    GUI_ELEMENT_GAUGE,        /* A dial that shows a value - like a speedometer */
    GUI_ELEMENT_KEYBOARD      /* Buttons for typing - like a typewriter */
} gui_element_type_t;

/* ===== Special Tags for Screen Things ===== */
// Opaque handle for UI elements - a special name tag for things on screen
typedef void* gui_element_t;  /* This is like a special sticker we put on each thing */

/* ===== Types of Things That Can Happen ===== */
// GUI event types - different ways people can interact with the screen
typedef enum {
    GUI_EVENT_CLICK = 0,      /* When someone taps something - like poking a toy */
    GUI_EVENT_LONG_PRESS,     /* When someone holds something - like squeezing a toy */
    GUI_EVENT_VALUE_CHANGED,  /* When a number changes - like turning a dial */
    GUI_EVENT_SELECTED,       /* When something is chosen - like picking a toy */
    GUI_EVENT_FOCUSED,        /* When something gets attention - like shining a flashlight */
    GUI_EVENT_DEFOCUSED,      /* When something loses attention - like turning away */
    GUI_EVENT_KEY,            /* When a key is pressed - like pushing a piano key */
    GUI_EVENT_SCROLL          /* When the screen moves up/down - like a rolling window */
} gui_event_type_t;

/* ===== Information About What Happened ===== */
// GUI event data - details about something that happened on screen
typedef struct {
    gui_event_type_t type;     /* What kind of thing happened */
    gui_element_t target;      /* Which thing it happened to - like which toy was poked */
    gui_element_t source;      /* What caused it to happen - like which person poked it */
    union {                    /* Different kinds of extra information */
        int32_t value;         /* A number - like how many times it was poked */
        const char* text;      /* Some words - like what someone said */
        struct {               /* A spot on the screen */
            int16_t x;         /* How far across the screen (from left) */
            int16_t y;         /* How far down the screen (from top) */
        } pos;
        uint32_t key;          /* Which key was pressed - like which piano key */
    } data;
} gui_event_t;

/* ===== Screen Thing Doorbell ===== */
// GUI event callback type - a function that rings when something happens on screen
typedef void (*gui_event_callback_t)(gui_event_t* event);  /* This is called when something happens */

/* ===== Starting the Picture Maker ===== */

/**
 * Turn on the picture maker so we can draw pretty things
 * @return Message telling us if it worked or not
 */
gui_status_t gui_init(void);  /* This is like getting our crayons and paper ready */

/**
 * Turn off the picture maker when we're done
 */
void gui_deinit(void);  /* This is like putting our crayons away */

/**
 * Keep the picture maker working - call this often
 */
void gui_update(void);  /* This redraws the screen to keep it looking nice */

/* ===== Choosing Colors ===== */

/**
 * Change the color style of everything on screen
 * @param theme Which color style to use (light, dark, or custom)
 * @return Message telling us if it worked or not
 */
gui_status_t gui_set_theme(gui_theme_t theme);  /* This is like changing the color of our paper */

/* ===== Creating and Removing Screen Things ===== */

/**
 * Put a new thing on the screen
 * @param type What kind of thing to create (button, label, etc.)
 * @param parent Where to put it (inside what other thing, or NULL for directly on screen)
 * @return A special tag for our new thing, or NULL if it didn't work
 */
gui_element_t gui_create_element(gui_element_type_t type, gui_element_t parent);  /* This is like drawing something new */

/**
 * Remove something from the screen
 * @param element The special tag for the thing we want to remove
 */
void gui_delete_element(gui_element_t element);  /* This is like erasing something we drew */

/* ===== Moving and Sizing Screen Things ===== */

/**
 * Move something to a different spot on the screen
 * @param element The special tag for the thing we want to move
 * @param x How far across the screen (from left)
 * @param y How far down the screen (from top)
 * @return Message telling us if it worked or not
 */
gui_status_t gui_set_position(gui_element_t element, int16_t x, int16_t y);  /* This is like moving a sticker on paper */

/**
 * Change how big something is on the screen
 * @param element The special tag for the thing we want to resize
 * @param width How wide to make it
 * @param height How tall to make it
 * @return Message telling us if it worked or not
 */
gui_status_t gui_set_size(gui_element_t element, uint16_t width, uint16_t height);  /* This is like stretching or shrinking a picture */

/* ===== Changing Words and Numbers ===== */

/**
 * Change the words shown on something
 * @param element The special tag for the thing we want to change
 * @param text What words to show
 * @return Message telling us if it worked or not
 */
gui_status_t gui_set_text(gui_element_t element, const char* text);  /* This is like writing words on a sign */

/**
 * Change the number value of something
 * @param element The special tag for the thing we want to change
 * @param value What number to set
 * @return Message telling us if it worked or not
 */
gui_status_t gui_set_value(gui_element_t element, int32_t value);  /* This is like changing a number on a scoreboard */

/**
 * Find out what number is showing on something
 * @param element The special tag for the thing we want to check
 * @return The current number value
 */
int32_t gui_get_value(gui_element_t element);  /* This is like reading a number from a scoreboard */

/* ===== Coloring Screen Things ===== */

/**
 * Change the color of something on the screen
 * @param element The special tag for the thing we want to color
 * @param color What color to make it
 * @param part Which part of the thing to color (0 for the main part)
 * @return Message telling us if it worked or not
 */
gui_status_t gui_set_color(gui_element_t element, display_color_t color, uint8_t part);  /* This is like coloring with a crayon */

/* ===== Setting Up Screen Thing Doorbells ===== */

/**
 * Make something ring a doorbell when people interact with it
 * @param element The special tag for the thing we're setting up
 * @param event_type What kind of interaction to watch for (click, press, etc.)
 * @param callback What doorbell function to ring when the interaction happens
 * @return Message telling us if it worked or not
 */
gui_status_t gui_register_event(gui_element_t element, gui_event_type_t event_type, gui_event_callback_t callback);  /* This is like saying "tell me when someone touches this" */

/* ===== Making Pop-up Messages ===== */

/**
 * Show a special message box on top of everything else
 * @param title The title at the top of the box
 * @param message The words to show in the box
 * @param buttons Names for buttons at the bottom (like "OK" or "Cancel")
 * @param callback What function to call when someone presses a button
 * @return A special tag for our message box, or NULL if it didn't work
 */
gui_element_t gui_create_msgbox(const char* title, const char* message, const char** buttons, gui_event_callback_t callback);  /* This is like making a note popup */

/* ===== Working with Pictures ===== */

/**
 * Load a picture from a file
 * @param filename Where to find the picture file
 * @return A special tag for our picture, or NULL if it didn't work
 */
void* gui_load_image(const char* filename);  /* This is like getting a photo from a photo album */

/**
 * Put a picture on something on the screen
 * @param element The special tag for the thing we want to show the picture on
 * @param image The special tag for the picture we want to show
 * @return Message telling us if it worked or not
 */
gui_status_t gui_set_image(gui_element_t element, void* image);  /* This is like sticking a photo on something */

/* ===== Handling Real Buttons ===== */

/**
 * Tell the screen that someone pressed a real button
 * @param button_id Which button was pressed
 */
void gui_handle_button_press(uint8_t button_id);  /* This tells the screen "someone pushed a button!" */

/**
 * Tell the screen that someone let go of a real button
 * @param button_id Which button was released
 */
void gui_handle_button_release(uint8_t button_id);  /* This tells the screen "someone let go of a button!" */

/**
 * Tell the screen that someone held a real button for a long time
 * @param button_id Which button was held
 */
void gui_handle_button_long_press(uint8_t button_id);  /* This tells the screen "someone squeezed a button!" */

/* ===== Working with Different Screens ===== */

/**
 * Make a brand new screen to put things on
 * @return A special tag for our new screen, or NULL if it didn't work
 */
gui_element_t gui_create_screen(void);  /* This is like getting a fresh sheet of paper */

/**
 * Choose which screen to show right now
 * @param screen The special tag for the screen we want to show
 * @return Message telling us if it worked or not
 */
gui_status_t gui_set_screen(gui_element_t screen);  /* This is like flipping to a different page */

/**
 * Find out which screen is showing right now
 * @return The special tag for the screen currently being shown
 */
gui_element_t gui_get_active_screen(void);  /* This tells us which page we're looking at now */

/* ===== Giving Special Attention to Things ===== */

/**
 * Give special attention to something on the screen
 * @param element The special tag for the thing we want to focus on
 * @return Message telling us if it worked or not
 */
gui_status_t gui_focus_element(gui_element_t element);  /* This is like shining a spotlight on something */

/**
 * Find out what is getting special attention right now
 * @return The special tag for the thing that has focus, or NULL if nothing does
 */
gui_element_t gui_get_focused_element(void);  /* This tells us what has the spotlight right now */

#endif /* End of GUI_MANAGER_H - we're done describing our picture maker! */
