#ifndef FS_MANAGER_H
#define FS_MANAGER_H

#include <stdint.h>
#include <stddef.h>

// File system status codes
typedef enum {
    FS_OK = 0,
    FS_ERROR_INIT,
    FS_ERROR_MOUNT,
    FS_ERROR_UNMOUNT,
    FS_ERROR_OPEN,
    FS_ERROR_CLOSE,
    FS_ERROR_READ,
    FS_ERROR_WRITE,
    FS_ERROR_SEEK,
    FS_ERROR_TELL,
    FS_ERROR_TRUNCATE,
    FS_ERROR_MKDIR,
    FS_ERROR_REMOVE,
    FS_ERROR_RENAME,
    FS_ERROR_STAT,
    FS_ERROR_FULL,
    FS_ERROR_NOT_FOUND,
    FS_ERROR_INVALID_NAME,
    FS_ERROR_DENIED,
    FS_ERROR_EXIST,
    FS_ERROR_NOT_READY,
    FS_ERROR_INVALID_PARAM,
    FS_ERROR_NO_PATH,
    FS_ERROR_TIMEOUT
} fs_status_t;

// File open mode
typedef enum {
    FS_READ = 0,       // Read only
    FS_WRITE,          // Write only
    FS_READWRITE,      // Read and write
    FS_APPEND,         // Append to file
    FS_CREATE,         // Create new file (error if exists)
    FS_CREATE_ALWAYS   // Create new or overwrite
} fs_open_mode_t;

// File seek origin
typedef enum {
    FS_SEEK_SET = 0,   // Seek from beginning of file
    FS_SEEK_CUR,       // Seek from current position
    FS_SEEK_END        // Seek from end of file
} fs_seek_origin_t;

// File info structure
typedef struct {
    char name[MAX_FILENAME_LENGTH];  // File name
    uint8_t is_dir;                  // 1 if directory, 0 if file
    uint32_t size;                   // File size in bytes
    uint32_t date;                   // Last modified date
    uint32_t time;                   // Last modified time
} fs_file_info_t;

// File handle type
typedef struct fs_file_s* fs_file_t;

// Directory handle type
typedef struct fs_dir_s* fs_dir_t;

/**
 * Initialize the file system
 * @return Status code
 */
fs_status_t fs_init(void);

/**
 * Deinitialize the file system
 * @return Status code
 */
fs_status_t fs_deinit(void);

/**
 * Update file system status - call periodically
 */
void fs_update(void);

/**
 * Mount file system
 * @param mount_point Mount point path
 * @return Status code
 */
fs_status_t fs_mount(const char *mount_point);

/**
 * Unmount file system
 * @param mount_point Mount point path
 * @return Status code
 */
fs_status_t fs_unmount(const char *mount_point);

/**
 * Open a file
 * @param path File path
 * @param mode Open mode
 * @param file Pointer to file handle
 * @return Status code
 */
fs_status_t fs_open(const char *path, fs_open_mode_t mode, fs_file_t *file);

/**
 * Close a file
 * @param file File handle
 * @return Status code
 */
fs_status_t fs_close(fs_file_t file);

/**
 * Read from a file
 * @param file File handle
 * @param buffer Buffer to read data into
 * @param size Number of bytes to read
 * @param bytes_read Pointer to store number of bytes actually read
 * @return Status code
 */
fs_status_t fs_read(fs_file_t file, void *buffer, size_t size, size_t *bytes_read);

/**
 * Write to a file
 * @param file File handle
 * @param buffer Buffer containing data to write
 * @param size Number of bytes to write
 * @param bytes_written Pointer to store number of bytes actually written
 * @return Status code
 */
fs_status_t fs_write(fs_file_t file, const void *buffer, size_t size, size_t *bytes_written);

/**
 * Seek to position in file
 * @param file File handle
 * @param offset Offset in bytes
 * @param origin Seek origin
 * @return Status code
 */
fs_status_t fs_seek(fs_file_t file, int32_t offset, fs_seek_origin_t origin);

/**
 * Get current position in file
 * @param file File handle
 * @param position Pointer to store position
 * @return Status code
 */
fs_status_t fs_tell(fs_file_t file, uint32_t *position);

/**
 * Truncate file to specified size
 * @param file File handle
 * @param size New size
 * @return Status code
 */
fs_status_t fs_truncate(fs_file_t file, uint32_t size);

/**
 * Sync file changes to disk
 * @param file File handle
 * @return Status code
 */
fs_status_t fs_sync(fs_file_t file);

/**
 * Create directory
 * @param path Directory path
 * @return Status code
 */
fs_status_t fs_mkdir(const char *path);

/**
 * Remove file or empty directory
 * @param path Path to file or directory
 * @return Status code
 */
fs_status_t fs_remove(const char *path);

/**
 * Rename file or directory
 * @param old_path Old path
 * @param new_path New path
 * @return Status code
 */
fs_status_t fs_rename(const char *old_path, const char *new_path);

/**
 * Get file or directory info
 * @param path Path to file or directory
 * @param info Pointer to info structure to populate
 * @return Status code
 */
fs_status_t fs_stat(const char *path, fs_file_info_t *info);

/**
 * Open directory for reading
 * @param path Directory path
 * @param dir Pointer to directory handle
 * @return Status code
 */
fs_status_t fs_opendir(const char *path, fs_dir_t *dir);

/**
 * Close directory
 * @param dir Directory handle
 * @return Status code
 */
fs_status_t fs_closedir(fs_dir_t dir);

/**
 * Read next directory entry
 * @param dir Directory handle
 * @param info Pointer to info structure to populate
 * @return Status code (FS_ERROR_NOT_FOUND when no more entries)
 */
fs_status_t fs_readdir(fs_dir_t dir, fs_file_info_t *info);

/**
 * Get free space on file system
 * @param mount_point Mount point path
 * @param bytes_free Pointer to store free space
 * @return Status code
 */
fs_status_t fs_get_free_space(const char *mount_point, uint64_t *bytes_free);

/**
 * Get total space on file system
 * @param mount_point Mount point path
 * @param bytes_total Pointer to store total space
 * @return Status code
 */
fs_status_t fs_get_total_space(const char *mount_point, uint64_t *bytes_total);

/**
 * Format file system
 * @param mount_point Mount point path
 * @return Status code
 */
fs_status_t fs_format(const char *mount_point);

#endif // FS_MANAGER_H
