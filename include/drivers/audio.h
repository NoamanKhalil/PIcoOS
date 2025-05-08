/* =================== PIcoOS Sound and Music Controller =================== */
/* This file helps us play sounds and music through the speaker! */

#ifndef AUDIO_H    /* This is a special guard that makes sure we only include this file once */
#define AUDIO_H

#include <stdint.h>   /* This gives us special number types */
#include <stddef.h>   /* This gives us special size types */

/* ===== Sound Problem Messages ===== */
// Audio status codes - messages about how the speaker is doing
typedef enum {
    AUDIO_OK = 0,            /* Everything is working fine! */
    AUDIO_ERROR_INIT,        /* Problem when turning on the speaker */
    AUDIO_ERROR_BUSY,        /* The speaker is too busy right now */
    AUDIO_ERROR_PARAM,       /* Someone gave the speaker wrong instructions */
    AUDIO_ERROR_MEMORY,      /* Not enough memory to play the sound */
    AUDIO_ERROR_FORMAT,      /* The sound file is in a format we don't understand */
    AUDIO_ERROR_IO,          /* Problem reading the sound data */
    AUDIO_ERROR_TIMEOUT      /* The speaker is taking too long to answer */
} audio_status_t;

/* ===== Types of Sound Files ===== */
// Audio formats - different ways sound can be stored
typedef enum {
    AUDIO_FORMAT_WAV = 0,    /* WAV files - like simple recordings */
    AUDIO_FORMAT_MP3,        /* MP3 files - like music from the internet */
    AUDIO_FORMAT_OGG,        /* OGG files - another type of music file */
    AUDIO_FORMAT_FLAC,       /* FLAC files - super high quality music */
    AUDIO_FORMAT_RAW_PCM     /* Raw sound data - like direct from a microphone */
} audio_format_t;

/* ===== How Fast to Play Sounds ===== */
// Audio sample rates - how many sound points to play each second
typedef enum {
    AUDIO_SAMPLE_RATE_8K = 8000,    /* 8,000 sound points per second - like a phone call */
    AUDIO_SAMPLE_RATE_11K = 11025,   /* 11,025 sound points per second - a bit better */
    AUDIO_SAMPLE_RATE_16K = 16000,   /* 16,000 sound points per second - like voice recording */
    AUDIO_SAMPLE_RATE_22K = 22050,   /* 22,050 sound points per second - decent music */
    AUDIO_SAMPLE_RATE_32K = 32000,   /* 32,000 sound points per second - good music */
    AUDIO_SAMPLE_RATE_44K = 44100,   /* 44,100 sound points per second - CD quality music! */
    AUDIO_SAMPLE_RATE_48K = 48000    /* 48,000 sound points per second - super high quality */
} audio_sample_rate_t;

/* ===== Sound Settings Box ===== */
// Audio configuration - all the important settings for our sounds
typedef struct {
    audio_format_t format;            /* What kind of sound file we have */
    audio_sample_rate_t sample_rate;  /* How many sound points per second */
    uint8_t channels;                 /* How many speakers (1=mono, 2=stereo) */
    uint8_t bits_per_sample;          /* How detailed each sound point is (8, 16, 24, 32) */
    uint16_t buffer_size;             /* How many sound points we prepare at once */
} audio_config_t;

/* ===== Is the Sound Playing? ===== */
// Audio playback states - different states the speaker can be in
typedef enum {
    AUDIO_STATE_STOPPED = 0,    /* No sound is playing - it's quiet */
    AUDIO_STATE_PLAYING,        /* Sound is playing - we can hear it */
    AUDIO_STATE_PAUSED          /* Sound is paused - waiting to play again */
} audio_state_t;

/* ===== Sound Doorbell ===== */
// Audio callback function type - a function that rings when sound needs more data
typedef void (*audio_callback_t)(void *buffer, size_t size);  /* This is called when the speaker needs more sound data */

/* ===== Turning the Speaker On and Off ===== */

/**
 * Wake up the speaker so we can play sounds
 * @return Message telling us if it worked or not
 */
audio_status_t audio_init(void);  /* This turns on the speaker */

/**
 * Turn off the speaker when we're done making sounds
 */
void audio_deinit(void);  /* This turns off the speaker to save power */

/* ===== Speaker Settings ===== */

/**
 * Tell the speaker how we want sounds to play
 * @param config A box of settings for the speaker
 * @return Message telling us if it worked or not
 */
audio_status_t audio_configure(const audio_config_t *config);  /* This tells the speaker how to behave */

/**
 * Ask the speaker what settings it's using right now
 * @param config A box where we'll put all the speaker settings
 * @return Message telling us if it worked or not
 */
audio_status_t audio_get_config(audio_config_t *config);  /* This asks the speaker about its settings */

/* ===== Playing Sounds ===== */

/**
 * Play a sound or music from a file
 * @param filename The name of the sound file to play
 * @return Message telling us if it worked or not
 */
audio_status_t audio_play_file(const char *filename);  /* This plays a sound file, like an MP3 */

/**
 * Play a sound that's already in memory
 * @param buffer The place in memory where the sound is stored
 * @param size How big the sound data is
 * @param format What kind of sound data it is
 * @return Message telling us if it worked or not
 */
audio_status_t audio_play_buffer(const void *buffer, size_t size, audio_format_t format);  /* This plays a sound from memory */

/* ===== Volume Control ===== */

/**
 * Make the sound louder or quieter
 * @param volume How loud (0 is silent, 100 is super loud)
 * @return Message telling us if it worked or not
 */
audio_status_t audio_set_volume(uint8_t volume);  /* This changes how loud the sound is */

/**
 * Ask how loud the sound is right now
 * @return How loud the sound is (0-100)
 */
uint8_t audio_get_volume(void);  /* This checks how loud we're playing sounds */

/* ===== Sound Playback Controls ===== */

/**
 * Pause the sound (like freeze it but remember where we are)
 * @return Message telling us if it worked or not
 */
audio_status_t audio_pause(void);  /* This is like pressing pause on a music player */

/**
 * Resume playing a paused sound (from where we left off)
 * @return Message telling us if it worked or not
 */
audio_status_t audio_resume(void);  /* This is like pressing play after pausing */

/**
 * Stop playing sound completely
 * @return Message telling us if it worked or not
 */
audio_status_t audio_stop(void);  /* This is like pressing stop on a music player */

/* ===== Finding Your Place in a Sound ===== */

/**
 * Jump to a specific place in the sound
 * @param position_ms Where to jump to (in milliseconds - 1/1000ths of a second)
 * @return Message telling us if it worked or not
 */
audio_status_t audio_seek(uint32_t position_ms);  /* This jumps to a different part of the sound */

/**
 * Find out where we are in the sound
 * @param position_ms A place to store how far into the sound we are
 * @return Message telling us if it worked or not
 */
audio_status_t audio_get_position(uint32_t *position_ms);  /* This tells us where we are in the sound */

/**
 * Check if the sound is playing, paused, or stopped
 * @return The current state (playing, paused, or stopped)
 */
audio_state_t audio_get_state(void);  /* This checks if the sound is playing right now */

/**
 * Find out how long the whole sound is
 * @param duration_ms A place to store the total length of the sound
 * @return Message telling us if it worked or not
 */
audio_status_t audio_get_duration(uint32_t *duration_ms);  /* This tells us how long the whole sound is */

/* ===== Special Sound Helpers ===== */

/**
 * Tell the speaker who to call when it needs more sound data
 * @param callback The function to call when more sound data is needed
 * @return Message telling us if it worked or not
 */
audio_status_t audio_register_callback(audio_callback_t callback);  /* This sets up who to tell when we need more sound */

/**
 * Keep the sound system happy - call this often
 */
void audio_update(void);  /* This makes sure sounds keep playing smoothly */

#endif /* End of AUDIO_H - we're done describing the sound system! */
