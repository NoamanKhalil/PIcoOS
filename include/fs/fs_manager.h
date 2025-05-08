/* =================== PIcoOS File and Folder Organizer =================== */
/* This file helps us keep all our files and folders neat and tidy! */

#ifndef FS_MANAGER_H    /* This is a special guard that makes sure we only include this file once */
#define FS_MANAGER_H

#include <stdint.h>   /* This gives us special number types */
#include <stddef.h>   /* This gives us special size types */

/* ===== File Organization Problem Messages ===== */
// File system status codes - messages about how our file organizer is doing
typedef enum {
    FS_OK = 0,                 /* Everything is working fine! */
    FS_ERROR_INIT,             /* Problem starting the file organizer */
    FS_ERROR_MOUNT,            /* Problem connecting to the toy box */
    FS_ERROR_UNMOUNT,          /* Problem disconnecting from the toy box */
    FS_ERROR_OPEN,             /* Can't open a file */
    FS_ERROR_CLOSE,            /* Can't close a file */
    FS_ERROR_READ,             /* Can't read from a file */
    FS_ERROR_WRITE,            /* Can't write to a file */
    FS_ERROR_SEEK,             /* Can't find a spot in the file */
    FS_ERROR_TELL,             /* Can't tell where we are in the file */
    FS_ERROR_TRUNCATE,         /* Can't make a file smaller */
    FS_ERROR_MKDIR,            /* Can't make a new folder */
    FS_ERROR_REMOVE,           /* Can't throw something away */
    FS_ERROR_RENAME,           /* Can't change a name */
    FS_ERROR_STAT,             /* Can't find information about something */
    FS_ERROR_FULL,             /* The toy box is completely full! */
    FS_ERROR_NOT_FOUND,        /* We can't find what we're looking for */
    FS_ERROR_INVALID_NAME,     /* That's not a good name for a file */
    FS_ERROR_DENIED,           /* We're not allowed to do that */
    FS_ERROR_EXIST,            /* Something with that name already exists */
    FS_ERROR_NOT_READY,        /* The file organizer isn't ready yet */
    FS_ERROR_INVALID_PARAM,    /* Someone gave wrong instructions */
    FS_ERROR_NO_PATH,          /* That path doesn't exist */
    FS_ERROR_TIMEOUT           /* The file organizer is taking too long to answer */
} fs_status_t;

/* ===== Ways to Open Files ===== */
// File open mode - different ways we can use a file
typedef enum {
    FS_READ = 0,       /* Just looking - we can only see what's inside */
    FS_WRITE,          /* Just changing - we can only put new stuff in */
    FS_READWRITE,      /* Looking and changing - we can do both */
    FS_APPEND,         /* Adding to the end - like adding to a story */
    FS_CREATE,         /* Making a brand new file (but only if no file has this name) */
    FS_CREATE_ALWAYS   /* Making a brand new file (even if we have to replace an old one) */
} fs_open_mode_t;

/* ===== Where to Look in a File ===== */
// File seek origin - where to start looking from in a file
typedef enum {
    FS_SEEK_SET = 0,   /* Start from the beginning - like the first page of a book */
    FS_SEEK_CUR,       /* Start from where we are now - like our current page */
    FS_SEEK_END        /* Start from the end - like the last page of a book */
} fs_seek_origin_t;

/* ===== Information About a File or Folder ===== */
// File info structure - details about a file or folder
typedef struct {
    char name[MAX_FILENAME_LENGTH];  /* The name of the file or folder */
    uint8_t is_dir;                  /* Is it a folder? 1=yes (it's a folder), 0=no (it's a file) */
    uint32_t size;                   /* How big is the file in bytes */
    uint32_t date;                   /* What day was it last changed */
    uint32_t time;                   /* What time was it last changed */
} fs_file_info_t;

/* ===== Special Tags for Open Files and Folders ===== */
// File handle type - like a special tag we put on a file when we open it
typedef struct fs_file_s* fs_file_t;  /* This is our special tag for an open file */

// Directory handle type - like a special tag we put on a folder when we open it
typedef struct fs_dir_s* fs_dir_t;  /* This is our special tag for an open folder */

/* ===== Setting Up Our File Organizer ===== */

/**
 * Start the file organizer so we can manage files and folders
 * @return Message telling us if it worked or not
 */
fs_status_t fs_init(void);  /* This turns on our file organizer */

/**
 * Turn off the file organizer when we're done using it
 * @return Message telling us if it worked or not
 */
fs_status_t fs_deinit(void);  /* This turns off our file organizer */

/**
 * Keep the file organizer happy - call this often
 */
void fs_update(void);  /* This makes sure our file organizer runs smoothly */

/* ===== Connecting to Toy Boxes ===== */

/**
 * Connect a toy box (like a memory card) to our file organizer
 * @param mount_point What name to give our toy box
 * @return Message telling us if it worked or not
 */
fs_status_t fs_mount(const char *mount_point);  /* This is like plugging in a toy box */

/**
 * Disconnect a toy box from our file organizer
 * @param mount_point The name of our toy box
 * @return Message telling us if it worked or not
 */
fs_status_t fs_unmount(const char *mount_point);  /* This is like putting away a toy box */

/* ===== Opening and Closing Files ===== */

/**
 * Open a file so we can look at it or change it
 * @param path Where to find the file (like "toybox/diary.txt")
 * @param mode How we want to use the file (just look, just change, or both)
 * @param file A place to store our special tag for the open file
 * @return Message telling us if it worked or not
 */
fs_status_t fs_open(const char *path, fs_open_mode_t mode, fs_file_t *file);  /* This is like opening a book */

/**
 * Close a file when we're done using it
 * @param file Our special tag for the open file
 * @return Message telling us if it worked or not
 */
fs_status_t fs_close(fs_file_t file);  /* This is like closing a book when we're done */

/* ===== Reading and Writing Files ===== */

/**
 * Read information from a file
 * @param file Our special tag for the open file
 * @param buffer A place to put what we read
 * @param size How much we want to read
 * @param bytes_read A place to store how much we actually read
 * @return Message telling us if it worked or not
 */
fs_status_t fs_read(fs_file_t file, void *buffer, size_t size, size_t *bytes_read);  /* This is like reading pages from a book */

/**
 * Write information to a file
 * @param file Our special tag for the open file
 * @param buffer The information we want to write
 * @param size How much we want to write
 * @param bytes_written A place to store how much we actually wrote
 * @return Message telling us if it worked or not
 */
fs_status_t fs_write(fs_file_t file, const void *buffer, size_t size, size_t *bytes_written);  /* This is like writing in a notebook */

/* ===== Finding Places in Files ===== */

/**
 * Jump to a specific spot in the file
 * @param file Our special tag for the open file
 * @param offset How far to jump (can be forward or backward)
 * @param origin Where to start jumping from (beginning, current spot, or end)
 * @return Message telling us if it worked or not
 */
fs_status_t fs_seek(fs_file_t file, int32_t offset, fs_seek_origin_t origin);  /* This is like flipping to a certain page */

/**
 * Ask where we are in the file right now
 * @param file Our special tag for the open file
 * @param position A place to store our current spot in the file
 * @return Message telling us if it worked or not
 */
fs_status_t fs_tell(fs_file_t file, uint32_t *position);  /* This is like using a bookmark to remember our page */

/* ===== Changing Files ===== */

/**
 * Make a file smaller by cutting off the end
 * @param file Our special tag for the open file
 * @param size How big we want the file to be
 * @return Message telling us if it worked or not
 */
fs_status_t fs_truncate(fs_file_t file, uint32_t size);  /* This is like tearing pages out of a book */

/**
 * Make sure all our changes are saved
 * @param file Our special tag for the open file
 * @return Message telling us if it worked or not
 */
fs_status_t fs_sync(fs_file_t file);  /* This is like making sure our homework is saved */

/* ===== Managing Files and Folders ===== */

/**
 * Create a new folder to put things in
 * @param path Where to make the new folder (like "toybox/my_stuff")
 * @return Message telling us if it worked or not
 */
fs_status_t fs_mkdir(const char *path);  /* This is like making a new drawer for toys */

/**
 * Delete a file or empty folder
 * @param path Which file or folder to delete
 * @return Message telling us if it worked or not
 */
fs_status_t fs_remove(const char *path);  /* This is like throwing something in the trash */

/**
 * Change the name of a file or folder
 * @param old_path The old name and location
 * @param new_path The new name and location
 * @return Message telling us if it worked or not
 */
fs_status_t fs_rename(const char *old_path, const char *new_path);  /* This is like putting a new label on something */

/* ===== Getting Information About Files and Folders ===== */

/**
 * Ask for details about a file or folder
 * @param path Which file or folder to check
 * @param info A place to store all the details we learn
 * @return Message telling us if it worked or not
 */
fs_status_t fs_stat(const char *path, fs_file_info_t *info);  /* This is like reading the label on a toy */

/* ===== Looking Through Folders ===== */

/**
 * Open a folder so we can look at what's inside
 * @param path Which folder to open
 * @param dir A place to store our special tag for the open folder
 * @return Message telling us if it worked or not
 */
fs_status_t fs_opendir(const char *path, fs_dir_t *dir);  /* This is like opening a drawer to see what's inside */

/**
 * Close a folder when we're done looking
 * @param dir Our special tag for the open folder
 * @return Message telling us if it worked or not
 */
fs_status_t fs_closedir(fs_dir_t dir);  /* This is like closing a drawer when we're done */

/**
 * Look at the next thing in the folder
 * @param dir Our special tag for the open folder
 * @param info A place to store details about what we found
 * @return Message telling us if it worked (or if we reached the end)
 */
fs_status_t fs_readdir(fs_dir_t dir, fs_file_info_t *info);  /* This is like picking up the next toy from a drawer */

/* ===== Checking Space and Cleaning Up ===== */

/**
 * Find out how much free space is left in our toy box
 * @param mount_point Which toy box to check
 * @param bytes_free A place to store how much space is left
 * @return Message telling us if it worked or not
 */
fs_status_t fs_get_free_space(const char *mount_point, uint64_t *bytes_free);  /* This checks if we have room for more toys */

/**
 * Find out how big our toy box is in total
 * @param mount_point Which toy box to check
 * @param bytes_total A place to store the total size
 * @return Message telling us if it worked or not
 */
fs_status_t fs_get_total_space(const char *mount_point, uint64_t *bytes_total);  /* This tells us how big our toy box is */

/**
 * Erase everything in the toy box and start fresh
 * @param mount_point Which toy box to erase
 * @return Message telling us if it worked or not
 */
fs_status_t fs_format(const char *mount_point);  /* This is like dumping everything out and starting over */

#endif /* End of FS_MANAGER_H - we're done describing our file organizer! */
