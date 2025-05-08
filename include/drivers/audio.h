#ifndef AUDIO_H
#define AUDIO_H

#include <stdint.h>
#include <stddef.h>

// Audio status codes
typedef enum {
    AUDIO_OK = 0,
    AUDIO_ERROR_INIT,
    AUDIO_ERROR_BUSY,
    AUDIO_ERROR_PARAM,
    AUDIO_ERROR_MEMORY,
    AUDIO_ERROR_FORMAT,
    AUDIO_ERROR_IO,
    AUDIO_ERROR_TIMEOUT
} audio_status_t;

// Audio formats
typedef enum {
    AUDIO_FORMAT_WAV = 0,
    AUDIO_FORMAT_MP3,
    AUDIO_FORMAT_OGG,
    AUDIO_FORMAT_FLAC,
    AUDIO_FORMAT_RAW_PCM
} audio_format_t;

// Audio sample rates
typedef enum {
    AUDIO_SAMPLE_RATE_8K = 8000,
    AUDIO_SAMPLE_RATE_11K = 11025,
    AUDIO_SAMPLE_RATE_16K = 16000,
    AUDIO_SAMPLE_RATE_22K = 22050,
    AUDIO_SAMPLE_RATE_32K = 32000,
    AUDIO_SAMPLE_RATE_44K = 44100,
    AUDIO_SAMPLE_RATE_48K = 48000
} audio_sample_rate_t;

// Audio configuration
typedef struct {
    audio_format_t format;            // Audio format
    audio_sample_rate_t sample_rate;  // Sample rate in Hz
    uint8_t channels;                 // Number of channels (1=mono, 2=stereo)
    uint8_t bits_per_sample;          // Bits per sample (8, 16, 24, 32)
    uint16_t buffer_size;             // Buffer size in samples
} audio_config_t;

// Audio playback states
typedef enum {
    AUDIO_STATE_STOPPED = 0,
    AUDIO_STATE_PLAYING,
    AUDIO_STATE_PAUSED
} audio_state_t;

// Audio callback function type
typedef void (*audio_callback_t)(void *buffer, size_t size);

/**
 * Initialize the audio system
 * @return Status code
 */
audio_status_t audio_init(void);

/**
 * Deinitialize the audio system
 */
void audio_deinit(void);

/**
 * Configure audio parameters
 * @param config Configuration parameters
 * @return Status code
 */
audio_status_t audio_configure(const audio_config_t *config);

/**
 * Get current audio configuration
 * @param config Pointer to store configuration
 * @return Status code
 */
audio_status_t audio_get_config(audio_config_t *config);

/**
 * Play audio from file
 * @param filename Path to audio file
 * @return Status code
 */
audio_status_t audio_play_file(const char *filename);

/**
 * Play audio from memory buffer
 * @param buffer Buffer containing audio data
 * @param size Buffer size in bytes
 * @param format Audio format
 * @return Status code
 */
audio_status_t audio_play_buffer(const void *buffer, size_t size, audio_format_t format);

/**
 * Set audio volume
 * @param volume Volume level (0-100)
 * @return Status code
 */
audio_status_t audio_set_volume(uint8_t volume);

/**
 * Get current audio volume
 * @return Volume level (0-100)
 */
uint8_t audio_get_volume(void);

/**
 * Pause audio playback
 * @return Status code
 */
audio_status_t audio_pause(void);

/**
 * Resume audio playback
 * @return Status code
 */
audio_status_t audio_resume(void);

/**
 * Stop audio playback
 * @return Status code
 */
audio_status_t audio_stop(void);

/**
 * Seek to position in audio
 * @param position_ms Position in milliseconds
 * @return Status code
 */
audio_status_t audio_seek(uint32_t position_ms);

/**
 * Get current playback position
 * @param position_ms Pointer to store position in milliseconds
 * @return Status code
 */
audio_status_t audio_get_position(uint32_t *position_ms);

/**
 * Get current playback state
 * @return Playback state
 */
audio_state_t audio_get_state(void);

/**
 * Get audio duration
 * @param duration_ms Pointer to store duration in milliseconds
 * @return Status code
 */
audio_status_t audio_get_duration(uint32_t *duration_ms);

/**
 * Register callback for audio data
 * @param callback Callback function
 * @return Status code
 */
audio_status_t audio_register_callback(audio_callback_t callback);

/**
 * Update audio system - call periodically
 */
void audio_update(void);

#endif // AUDIO_H
