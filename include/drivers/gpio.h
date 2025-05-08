#ifndef GPIO_H
#define GPIO_H

#include <stdint.h>

// GPIO pin modes
typedef enum {
    GPIO_MODE_INPUT = 0,      // Input pin
    GPIO_MODE_OUTPUT,         // Output pin
    GPIO_MODE_ALT_FUNC,       // Alternate function
    GPIO_MODE_ANALOG          // Analog input
} gpio_mode_t;

// GPIO pull modes
typedef enum {
    GPIO_PULL_NONE = 0,       // No pull
    GPIO_PULL_UP,             // Pull-up
    GPIO_PULL_DOWN            // Pull-down
} gpio_pull_t;

// GPIO edge trigger modes
typedef enum {
    GPIO_EDGE_NONE = 0,       // No edge detection
    GPIO_EDGE_RISING,         // Rising edge
    GPIO_EDGE_FALLING,        // Falling edge
    GPIO_EDGE_BOTH            // Both edges
} gpio_edge_t;

// GPIO interrupt callback type
typedef void (*gpio_callback_t)(uint8_t pin);

/**
 * Initialize GPIO system
 */
void gpio_init(void);

/**
 * Configure a GPIO pin
 * @param pin Pin number
 * @param mode Pin mode
 * @param pull Pull mode
 * @return 0 on success, negative value on failure
 */
int gpio_configure(uint8_t pin, gpio_mode_t mode, gpio_pull_t pull);

/**
 * Set GPIO pin output value
 * @param pin Pin number
 * @param value 0 for low, non-zero for high
 */
void gpio_write(uint8_t pin, uint8_t value);

/**
 * Read GPIO pin input value
 * @param pin Pin number
 * @return 0 for low, 1 for high
 */
uint8_t gpio_read(uint8_t pin);

/**
 * Toggle GPIO pin output value
 * @param pin Pin number
 */
void gpio_toggle(uint8_t pin);

/**
 * Configure GPIO pin interrupt
 * @param pin Pin number
 * @param edge Edge trigger mode
 * @param callback Callback function
 * @return 0 on success, negative value on failure
 */
int gpio_set_interrupt(uint8_t pin, gpio_edge_t edge, gpio_callback_t callback);

/**
 * Enable GPIO pin interrupt
 * @param pin Pin number
 */
void gpio_enable_interrupt(uint8_t pin);

/**
 * Disable GPIO pin interrupt
 * @param pin Pin number
 */
void gpio_disable_interrupt(uint8_t pin);

/**
 * Configure GPIO pin alternate function
 * @param pin Pin number
 * @param function Alternate function number
 * @return 0 on success, negative value on failure
 */
int gpio_set_function(uint8_t pin, uint8_t function);

// Button events
typedef enum {
    BUTTON_PRESSED = 0,
    BUTTON_RELEASED,
    BUTTON_LONG_PRESS
} button_event_t;

// Button callback type
typedef void (*button_callback_t)(uint8_t button_id, button_event_t event);

/**
 * Initialize buttons with callback
 * @param callback Button event callback function
 * @return 0 on success, negative value on failure
 */
int buttons_init(button_callback_t callback);

/**
 * Get current button state
 * @param button_id Button ID
 * @return 1 if pressed, 0 if not pressed
 */
uint8_t button_is_pressed(uint8_t button_id);

/**
 * Set long press threshold
 * @param button_id Button ID
 * @param threshold_ms Threshold in milliseconds
 */
void button_set_long_press_threshold(uint8_t button_id, uint32_t threshold_ms);

#endif // GPIO_H
