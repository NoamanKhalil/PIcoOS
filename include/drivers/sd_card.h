#ifndef SD_CARD_H
#define SD_CARD_H

#include <stdint.h>

// SD card status codes
typedef enum {
    SD_CARD_OK = 0,
    SD_CARD_ERROR_INIT,
    SD_CARD_ERROR_READ,
    SD_CARD_ERROR_WRITE,
    SD_CARD_ERROR_TIMEOUT,
    SD_CARD_ERROR_INVALID_PARAM,
    SD_CARD_ERROR_NO_CARD
} sd_card_status_t;

// SD card info structure
typedef struct {
    uint8_t cardType;        // Card type (SD, SDHC, etc.)
    uint32_t capacity;       // Capacity in blocks
    uint32_t blockSize;      // Block size in bytes
    uint8_t manufacturer;    // Manufacturer ID
    uint16_t oem;            // OEM ID
    char productName[8];     // Product name
    uint8_t productRevision; // Product revision
    uint32_t serialNumber;   // Serial number
} sd_card_info_t;

/**
 * Initialize the SD card interface
 * @return Status code
 */
sd_card_status_t sd_card_init(void);

/**
 * Deinitialize the SD card interface
 */
void sd_card_deinit(void);

/**
 * Get SD card information
 * @param info Pointer to info structure to populate
 * @return Status code
 */
sd_card_status_t sd_card_get_info(sd_card_info_t *info);

/**
 * Read blocks from SD card
 * @param buffer Buffer to read data into
 * @param block Starting block number
 * @param count Number of blocks to read
 * @return Status code
 */
sd_card_status_t sd_card_read_blocks(uint8_t *buffer, uint32_t block, uint32_t count);

/**
 * Write blocks to SD card
 * @param buffer Buffer containing data to write
 * @param block Starting block number
 * @param count Number of blocks to write
 * @return Status code
 */
sd_card_status_t sd_card_write_blocks(const uint8_t *buffer, uint32_t block, uint32_t count);

/**
 * Check if SD card is present
 * @return 1 if present, 0 if not
 */
uint8_t sd_card_is_present(void);

/**
 * Get SD card capacity in bytes
 * @return Capacity in bytes
 */
uint64_t sd_card_get_capacity(void);

#endif // SD_CARD_H
