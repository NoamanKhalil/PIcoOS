/* =================== PIcoOS Button and Light Controller =================== */
/* This file helps us control buttons, lights, and other simple parts! */

#ifndef GPIO_H    /* This is a special guard that makes sure we only include this file once */
#define GPIO_H

#include <stdint.h>   /* This gives us special number types */

/* ===== Pin Jobs ===== */
// GPIO pin modes - what job each pin can do
typedef enum {
    GPIO_MODE_INPUT = 0,      /* Listening mode - like ears that hear button presses */
    GPIO_MODE_OUTPUT,         /* Talking mode - like a mouth that can turn on lights */
    GPIO_MODE_ALT_FUNC,       /* Special job mode - for fancy work with other parts */
    GPIO_MODE_ANALOG          /* Measuring mode - can tell how much, not just yes/no */
} gpio_mode_t;

/* ===== Pin Default States ===== */
// GPIO pull modes - like choosing if a button is normally pressed or not pressed
typedef enum {
    GPIO_PULL_NONE = 0,       /* No helper - pin floats freely (like a balloon) */
    GPIO_PULL_UP,             /* Helper that pulls pin up when nothing is happening */
    GPIO_PULL_DOWN            /* Helper that pulls pin down when nothing is happening */
} gpio_pull_t;

/* ===== When to Notice Changes ===== */
// GPIO edge trigger modes - when to notice that a button changed
typedef enum {
    GPIO_EDGE_NONE = 0,       /* Don't notice any changes */
    GPIO_EDGE_RISING,         /* Notice when a button is pressed */
    GPIO_EDGE_FALLING,        /* Notice when a button is released */
    GPIO_EDGE_BOTH            /* Notice both when pressed and released */
} gpio_edge_t;

/* ===== Pin Change Doorbell ===== */
// GPIO interrupt callback type - like a doorbell that rings when a pin changes
typedef void (*gpio_callback_t)(uint8_t pin);  /* This is a special function that runs when a pin changes */

/* ===== Basic Pin Control ===== */

/**
 * Wake up all the pins so we can use them
 */
void gpio_init(void);  /* This turns on the part that controls all pins */

/**
 * Tell a pin what job to do
 * @param pin Which pin to setup (like its name tag)
 * @param mode What job the pin should do (listen, talk, etc.)
 * @param pull Whether the pin should be pulled up or down when resting
 * @return 0 if it worked, a negative number if there was a problem
 */
int gpio_configure(uint8_t pin, gpio_mode_t mode, gpio_pull_t pull);  /* This gives a pin its job */

/* ===== Talking To Pins ===== */

/**
 * Turn a pin on or off
 * @param pin Which pin to control
 * @param value 0 to turn it off, any other number to turn it on
 */
void gpio_write(uint8_t pin, uint8_t value);  /* This is like a light switch - turn pins on or off */

/**
 * Check if a pin is on or off
 * @param pin Which pin to check
 * @return 0 if it's off, 1 if it's on
 */
uint8_t gpio_read(uint8_t pin);  /* This checks if a button is pressed or a light is on */

/**
 * Flip a pin from on to off or off to on
 * @param pin Which pin to flip
 */
void gpio_toggle(uint8_t pin);  /* This flips a switch - if it's on, turn it off; if it's off, turn it on */

/* ===== Setting Up Pin Doorbells ===== */

/**
 * Make a pin ring a doorbell when it changes
 * @param pin Which pin to watch
 * @param edge When to notice changes (pressed, released, or both)
 * @param callback What doorbell function to ring when the pin changes
 * @return 0 if it worked, a negative number if there was a problem
 */
int gpio_set_interrupt(uint8_t pin, gpio_edge_t edge, gpio_callback_t callback); /* This sets up a pin to alert us when it changes */

/**
 * Turn on the doorbell for a pin
 * @param pin Which pin's doorbell to turn on
 */
void gpio_enable_interrupt(uint8_t pin); /* This turns on the doorbell system for a pin */

/**
 * Turn off the doorbell for a pin
 * @param pin Which pin's doorbell to turn off
 */
void gpio_disable_interrupt(uint8_t pin); /* This turns off the doorbell system for a pin */

/* ===== Special Pin Jobs ===== */

/**
 * Give a pin a special job to do
 * @param pin Which pin to give a special job
 * @param function What special job number to assign
 * @return 0 if it worked, a negative number if there was a problem
 */
int gpio_set_function(uint8_t pin, uint8_t function); /* This gives a pin a fancy special job */

/* ===== Button Events ===== */
// Button events - different ways a button can be used
typedef enum {
    BUTTON_PRESSED = 0,     /* When someone pushes a button down */
    BUTTON_RELEASED,        /* When someone lets go of a button */
    BUTTON_LONG_PRESS       /* When someone holds a button down for a long time */
} button_event_t;

/* ===== Button Doorbell ===== */
// Button callback type - a special function that runs when buttons change
typedef void (*button_callback_t)(uint8_t button_id, button_event_t event); /* This function gets called when buttons are used */

/* ===== Button Controls ===== */

/**
 * Wake up all the buttons so we can use them
 * @param callback What function to call when buttons are used
 * @return 0 if it worked, a negative number if there was a problem
 */
int buttons_init(button_callback_t callback); /* This turns on all the buttons and sets up their doorbell */

/**
 * Check if a button is being pressed right now
 * @param button_id Which button to check
 * @return 1 if the button is pressed, 0 if not pressed
 */
uint8_t button_is_pressed(uint8_t button_id); /* This checks if someone is pressing a button */

/**
 * Set how long someone needs to hold a button for a "long press"
 * @param button_id Which button to set
 * @param threshold_ms How many milliseconds (1/1000ths of a second) is "long"
 */
void button_set_long_press_threshold(uint8_t button_id, uint32_t threshold_ms); /* This sets how long makes a "long press" */

#endif /* End of GPIO_H - we're done describing buttons and lights! */
