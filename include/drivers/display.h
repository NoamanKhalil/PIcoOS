/* =================== PIcoOS Screen Controller =================== */
/* This file helps us draw pictures and words on the screen! */

#ifndef DISPLAY_H    /* This is a special guard that makes sure we only include this file once */
#define DISPLAY_H

#include <stdint.h>   /* This gives us special number types */

/* ===== Screen Problem Messages ===== */
// Display status codes - messages about how the screen is doing
typedef enum {
    DISPLAY_OK = 0,             /* Everything is working fine! */
    DISPLAY_ERROR_INIT,         /* Problem when turning on the screen */
    DISPLAY_ERROR_PARAMS,       /* Someone gave the screen wrong instructions */
    DISPLAY_ERROR_COMM,         /* Can't talk to the screen properly */
    DISPLAY_ERROR_TIMEOUT,      /* The screen is taking too long to answer */
    DISPLAY_ERROR_NO_DEVICE     /* We can't find a screen at all! */
} display_status_t;

/* ===== Types of Screens ===== */
// Display types - different kinds of screens we can use
typedef enum {
    DISPLAY_TYPE_SPI_TFT = 0,    /* A colorful screen that talks through SPI */
    DISPLAY_TYPE_I2C_OLED,       /* A tiny bright screen that talks through I2C */
    DISPLAY_TYPE_RGB_LCD,        /* A screen that uses separate red, green, blue signals */
    DISPLAY_TYPE_PARALLEL_TFT    /* A screen that gets all its data at once through many wires */
} display_type_t;

/* ===== Ways to Make Colors ===== */
// Color format - different ways to mix colors on the screen
typedef enum {
    DISPLAY_COLOR_RGB565 = 0,  /* 16-bit colors - like having a small box of crayons */
    DISPLAY_COLOR_RGB888,      /* 24-bit colors - like having a medium box of crayons */
    DISPLAY_COLOR_ARGB8888,    /* 32-bit colors with transparency - like clear crayons too! */
    DISPLAY_COLOR_MONO,        /* Just black and white - like using only a pencil */
    DISPLAY_COLOR_GRAY4,       /* 16 shades of gray - like different darkness pencil marks */
    DISPLAY_COLOR_GRAY8        /* 256 shades of gray - like very precise pencil shading */
} display_color_format_t;

/* ===== Screen Settings Box ===== */
// Display configuration - all the important settings for our screen
typedef struct {
    display_type_t type;               /* What kind of screen we have */
    display_color_format_t format;     /* What kind of colors our screen uses */
    uint16_t width;                    /* How many dots wide our screen is */
    uint16_t height;                   /* How many dots tall our screen is */
    uint8_t rotation;                  /* Which way our screen is turned (0, 90, 180, or 270 degrees) */
    uint8_t backlight_percentage;      /* How bright our screen's light is (0-100%) */
    void *driver_config;               /* Special secret settings for different screens */
} display_config_t;

/* ===== Color Mixing Box ===== */
// RGB color type (24-bit) - how we mix red, green, and blue to make any color
typedef struct {
    uint8_t r;  /* Red amount (0-255) - how much red to add */
    uint8_t g;  /* Green amount (0-255) - how much green to add */
    uint8_t b;  /* Blue amount (0-255) - how much blue to add */
} display_color_t; /* When we mix these three colors, we can make any color! */

/* ===== Turning the Screen On and Off ===== */

/**
 * Wake up the screen so we can use it
 * @return Message telling us if it worked or not
 */
display_status_t display_init(void); /* This turns on the screen */

/**
 * Turn off the screen when we're done
 */
void display_deinit(void); /* This turns off the screen to save power */

/**
 * Tell the screen how we want it to work
 * @param config A box of settings for the screen
 * @return Message telling us if it worked or not
 */
display_status_t display_configure(const display_config_t *config); /* This tells the screen how to behave */

/* ===== Basic Screen Controls ===== */

/**
 * Ask the screen what settings it's using right now
 * @param config A box where we'll put all the screen settings
 * @return Message telling us if it worked or not
 */
display_status_t display_get_config(display_config_t *config); /* This asks the screen about its settings */

/**
 * Change how bright the screen's light is
 * @param percentage How bright (0 means off, 100 means super bright)
 * @return Message telling us if it worked or not
 */
display_status_t display_set_backlight(uint8_t percentage); /* This makes the screen brighter or dimmer */

/**
 * Erase everything on the screen with one color
 * @param color What color to fill the whole screen with
 * @return Message telling us if it worked or not
 */
display_status_t display_clear(display_color_t color); /* This is like wiping the screen clean */

/* ===== Drawing Simple Shapes ===== */

/**
 * Draw a tiny dot on the screen
 * @param x How far across the screen (from left)
 * @param y How far down the screen (from top)
 * @param color What color to make the dot
 * @return Message telling us if it worked or not
 */
display_status_t display_draw_pixel(uint16_t x, uint16_t y, display_color_t color); /* This draws a single tiny dot */

/**
 * Draw a straight line from one point to another
 * @param x1 Where to start the line (how far across from left)
 * @param y1 Where to start the line (how far down from top)
 * @param x2 Where to end the line (how far across from left)
 * @param y2 Where to end the line (how far down from top)
 * @param color What color to make the line
 * @return Message telling us if it worked or not
 */
display_status_t display_draw_line(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, display_color_t color); /* This draws a straight line */

/* ===== Drawing More Shapes ===== */

/**
 * Draw a rectangle (like a box) on the screen
 * @param x Where to start drawing (how far across from left)
 * @param y Where to start drawing (how far down from top)
 * @param width How wide to make the rectangle
 * @param height How tall to make the rectangle
 * @param color What color to make the rectangle
 * @param filled 1 means color it in, 0 means just draw the outline
 * @return Message telling us if it worked or not
 */
display_status_t display_draw_rect(uint16_t x, uint16_t y, uint16_t width, uint16_t height, display_color_t color, uint8_t filled); /* This draws a box shape */

/**
 * Draw a circle on the screen
 * @param x_center Where the middle of the circle is (how far across from left)
 * @param y_center Where the middle of the circle is (how far down from top)
 * @param radius How big the circle is (from middle to edge)
 * @param color What color to make the circle
 * @param filled 1 means color it in, 0 means just draw the outline
 * @return Message telling us if it worked or not
 */
display_status_t display_draw_circle(uint16_t x_center, uint16_t y_center, uint16_t radius, display_color_t color, uint8_t filled); /* This draws a round circle shape */

/* ===== Drawing Letters and Words ===== */

/**
 * Draw a single letter on the screen
 * @param x Where to start drawing (how far across from left)
 * @param y Where to start drawing (how far down from top)
 * @param c Which letter to draw
 * @param font Which style of letters to use
 * @param color What color to make the letter
 * @param bg_color What color to put behind the letter
 * @return Message telling us if it worked or not
 */
display_status_t display_draw_char(uint16_t x, uint16_t y, char c, uint8_t font, display_color_t color, display_color_t bg_color); /* This draws one letter */

/**
 * Draw a whole sentence on the screen
 * @param x Where to start drawing (how far across from left)
 * @param y Where to start drawing (how far down from top)
 * @param text The words to write on the screen
 * @param font Which style of letters to use
 * @param color What color to make the text
 * @param bg_color What color to put behind the text
 * @return Message telling us if it worked or not
 */
display_status_t display_draw_text(uint16_t x, uint16_t y, const char *text, uint8_t font, display_color_t color, display_color_t bg_color); /* This draws many letters to make words */

/* ===== Drawing Pictures ===== */

/**
 * Draw a picture on the screen
 * @param x Where to start drawing (how far across from left)
 * @param y Where to start drawing (how far down from top)
 * @param width How wide the picture is
 * @param height How tall the picture is
 * @param data The actual picture data (all the colored dots)
 * @return Message telling us if it worked or not
 */
display_status_t display_draw_bitmap(uint16_t x, uint16_t y, uint16_t width, uint16_t height, const uint8_t *data); /* This draws a whole picture at once */

/**
 * Make all our drawing show up on the screen
 * Sometimes we draw in a special hidden place first, then show it all at once
 * @return Message telling us if it worked or not
 */
display_status_t display_update(void); /* This is like saying "now show everything I drew!" */

/* ===== Checking Screen Information ===== */

/**
 * Ask if a screen is actually plugged in
 * @return 1 if we found a screen, 0 if there's no screen
 */
uint8_t display_is_connected(void); /* This checks if we have a screen to use */

/**
 * Turn the screen to a different direction
 * @param rotation Which way to turn (0 is normal, 90 is sideways, 180 is upside-down, 270 is other sideways)
 * @return Message telling us if it worked or not
 */
display_status_t display_set_rotation(uint8_t rotation); /* This turns everything on the screen */

/**
 * Ask how wide the screen is
 * @return How many dots wide the screen is
 */
uint16_t display_get_width(void); /* This tells us how wide our screen is */

/**
 * Ask how tall the screen is
 * @return How many dots tall the screen is
 */
uint16_t display_get_height(void); /* This tells us how tall our screen is */

/**
 * Enter display sleep mode
 * @return Status code
 */
display_status_t display_sleep(void);

/**
 * Wake display from sleep mode
 * @return Status code
 */
display_status_t display_wake(void);

#endif // DISPLAY_H
