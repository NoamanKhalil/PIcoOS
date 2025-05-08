#ifndef DISPLAY_H
#define DISPLAY_H

#include <stdint.h>

// Display status codes
typedef enum {
    DISPLAY_OK = 0,
    DISPLAY_ERROR_INIT,
    DISPLAY_ERROR_PARAMS,
    DISPLAY_ERROR_COMM,
    DISPLAY_ERROR_TIMEOUT,
    DISPLAY_ERROR_NO_DEVICE
} display_status_t;

// Display types
typedef enum {
    DISPLAY_TYPE_SPI_TFT = 0,
    DISPLAY_TYPE_I2C_OLED,
    DISPLAY_TYPE_RGB_LCD,
    DISPLAY_TYPE_PARALLEL_TFT
} display_type_t;

// Color format
typedef enum {
    DISPLAY_COLOR_RGB565 = 0,  // 16-bit RGB (5-6-5)
    DISPLAY_COLOR_RGB888,      // 24-bit RGB (8-8-8)
    DISPLAY_COLOR_ARGB8888,    // 32-bit ARGB (8-8-8-8)
    DISPLAY_COLOR_MONO,        // Monochrome
    DISPLAY_COLOR_GRAY4,       // 4-bit grayscale
    DISPLAY_COLOR_GRAY8        // 8-bit grayscale
} display_color_format_t;

// Display configuration
typedef struct {
    display_type_t type;               // Display type
    display_color_format_t format;     // Color format
    uint16_t width;                    // Width in pixels
    uint16_t height;                   // Height in pixels
    uint8_t rotation;                  // Rotation (0, 90, 180, 270)
    uint8_t backlight_percentage;      // Backlight brightness (0-100%)
    void *driver_config;               // Driver-specific configuration
} display_config_t;

// RGB color type (24-bit)
typedef struct {
    uint8_t r;  // Red component (0-255)
    uint8_t g;  // Green component (0-255)
    uint8_t b;  // Blue component (0-255)
} display_color_t;

/**
 * Initialize display
 * @return Status code
 */
display_status_t display_init(void);

/**
 * Deinitialize display
 */
void display_deinit(void);

/**
 * Configure display parameters
 * @param config Configuration parameters
 * @return Status code
 */
display_status_t display_configure(const display_config_t *config);

/**
 * Get current display configuration
 * @param config Pointer to store configuration
 * @return Status code
 */
display_status_t display_get_config(display_config_t *config);

/**
 * Set display backlight brightness
 * @param percentage Brightness percentage (0-100)
 * @return Status code
 */
display_status_t display_set_backlight(uint8_t percentage);

/**
 * Clear display with specified color
 * @param color Color to clear with
 * @return Status code
 */
display_status_t display_clear(display_color_t color);

/**
 * Draw pixel at specified coordinates
 * @param x X coordinate
 * @param y Y coordinate
 * @param color Pixel color
 * @return Status code
 */
display_status_t display_draw_pixel(uint16_t x, uint16_t y, display_color_t color);

/**
 * Draw line between two points
 * @param x1 Start X coordinate
 * @param y1 Start Y coordinate
 * @param x2 End X coordinate
 * @param y2 End Y coordinate
 * @param color Line color
 * @return Status code
 */
display_status_t display_draw_line(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, display_color_t color);

/**
 * Draw rectangle
 * @param x X coordinate of top-left corner
 * @param y Y coordinate of top-left corner
 * @param width Width of rectangle
 * @param height Height of rectangle
 * @param color Rectangle color
 * @param filled 1 for filled, 0 for outline
 * @return Status code
 */
display_status_t display_draw_rect(uint16_t x, uint16_t y, uint16_t width, uint16_t height, display_color_t color, uint8_t filled);

/**
 * Draw circle
 * @param x_center X coordinate of center
 * @param y_center Y coordinate of center
 * @param radius Circle radius
 * @param color Circle color
 * @param filled 1 for filled, 0 for outline
 * @return Status code
 */
display_status_t display_draw_circle(uint16_t x_center, uint16_t y_center, uint16_t radius, display_color_t color, uint8_t filled);

/**
 * Draw character
 * @param x X coordinate
 * @param y Y coordinate
 * @param c Character to draw
 * @param font Font to use
 * @param color Text color
 * @param bg_color Background color
 * @return Status code
 */
display_status_t display_draw_char(uint16_t x, uint16_t y, char c, uint8_t font, display_color_t color, display_color_t bg_color);

/**
 * Draw text string
 * @param x X coordinate
 * @param y Y coordinate
 * @param text Text string to draw
 * @param font Font to use
 * @param color Text color
 * @param bg_color Background color
 * @return Status code
 */
display_status_t display_draw_text(uint16_t x, uint16_t y, const char *text, uint8_t font, display_color_t color, display_color_t bg_color);

/**
 * Draw bitmap image
 * @param x X coordinate of top-left corner
 * @param y Y coordinate of top-left corner
 * @param width Bitmap width
 * @param height Bitmap height
 * @param data Bitmap data
 * @return Status code
 */
display_status_t display_draw_bitmap(uint16_t x, uint16_t y, uint16_t width, uint16_t height, const uint8_t *data);

/**
 * Update display
 * Copies framebuffer to display (if applicable)
 * @return Status code
 */
display_status_t display_update(void);

/**
 * Check if display is connected
 * @return 1 if connected, 0 if not
 */
uint8_t display_is_connected(void);

/**
 * Set display rotation
 * @param rotation Rotation angle in degrees (0, 90, 180, 270)
 * @return Status code
 */
display_status_t display_set_rotation(uint8_t rotation);

/**
 * Get display width
 * @return Width in pixels
 */
uint16_t display_get_width(void);

/**
 * Get display height
 * @return Height in pixels
 */
uint16_t display_get_height(void);

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
