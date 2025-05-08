/* =================== PIcoOS Memory Card Controller =================== */
/* This file helps us save and load files on a memory card! */

#ifndef SD_CARD_H    /* This is a special guard that makes sure we only include this file once */
#define SD_CARD_H

#include <stdint.h>   /* This gives us special number types */

/* ===== Memory Card Problem Messages ===== */
// SD card status codes - messages about how the memory card is doing
typedef enum {
    SD_CARD_OK = 0,              /* Everything is working fine! */
    SD_CARD_ERROR_INIT,          /* Problem when turning on the memory card */
    SD_CARD_ERROR_READ,          /* Problem reading files from the memory card */
    SD_CARD_ERROR_WRITE,         /* Problem saving files to the memory card */
    SD_CARD_ERROR_TIMEOUT,       /* The memory card is taking too long to answer */
    SD_CARD_ERROR_INVALID_PARAM, /* Someone gave the memory card wrong instructions */
    SD_CARD_ERROR_NO_CARD        /* There's no memory card plugged in! */
} sd_card_status_t;

/* ===== Memory Card Information Box ===== */
// SD card info structure - details about our memory card
typedef struct {
    uint8_t cardType;        /* What kind of memory card it is (SD, SDHC, etc.) */
    uint32_t capacity;       /* How many blocks of space it has */
    uint32_t blockSize;      /* How big each block is (in bytes) */
    uint8_t manufacturer;    /* Who made the memory card */
    uint16_t oem;            /* Which company's name is on the card */
    char productName[8];     /* What the memory card is called */
    uint8_t productRevision; /* Which version of the card it is */
    uint32_t serialNumber;   /* The card's special ID number */
} sd_card_info_t;

/* ===== Turning the Memory Card On and Off ===== */

/**
 * Wake up the memory card so we can save and load files
 * @return Message telling us if it worked or not
 */
sd_card_status_t sd_card_init(void);  /* This turns on the memory card */

/**
 * Turn off the memory card when we're done using it
 */
void sd_card_deinit(void);  /* This turns off the memory card to save power */

/* ===== Learning About Our Memory Card ===== */

/**
 * Ask the memory card to tell us about itself
 * @param info A box where we'll put all the memory card details
 * @return Message telling us if it worked or not
 */
sd_card_status_t sd_card_get_info(sd_card_info_t *info);  /* This asks the memory card "who are you?" */

/* ===== Reading and Writing to the Memory Card ===== */

/**
 * Get information from the memory card
 * @param buffer A place to put the information we read
 * @param block Which section of the memory card to start reading from
 * @param count How many sections to read
 * @return Message telling us if it worked or not
 */
sd_card_status_t sd_card_read_blocks(uint8_t *buffer, uint32_t block, uint32_t count);  /* This is like copying pages from a book */

/**
 * Save information to the memory card
 * @param buffer The information we want to save
 * @param block Which section of the memory card to start writing to
 * @param count How many sections to write
 * @return Message telling us if it worked or not
 */
sd_card_status_t sd_card_write_blocks(const uint8_t *buffer, uint32_t block, uint32_t count);  /* This is like writing in a notebook */

/* ===== Checking on the Memory Card ===== */

/**
 * Check if a memory card is plugged in
 * @return 1 if the card is there, 0 if it's missing
 */
uint8_t sd_card_is_present(void);  /* This checks if we have a memory card to use */

/**
 * Find out how much space the memory card has
 * @return The total number of bytes the card can hold
 */
uint64_t sd_card_get_capacity(void);  /* This tells us how big our memory card is */

#endif /* End of SD_CARD_H - we're done describing the memory card! */
