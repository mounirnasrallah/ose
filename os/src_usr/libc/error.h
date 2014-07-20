#ifndef _ERROR_H_
#define _ERROR_H_
enum errno {
    SUCCESS = 0,    // no error
    FAILURE = -1,   // any error
    EPERM   = 1,    // Operation not permitted
    EINTR   = 2,    // Interrupted system call
    EIO     = 3,    // I/O error
    ENXIO   = 4,    // No such device or address
    E2BIG   = 5,    // Argument list too long
    EAGAIN  = 6,    // Try again
    ENOMEM  = 7,    // Out of memory
    EACCES  = 8,    // Permission denied
    EFAULT  = 9,    // Bad address
    EBUSY   = 10,   // Device or resource busy
    ENODEV  = 11,   // No such device
    EINVAL  = 12,   // Invalid argument
    ENOTTY  = 13,   // Not a typewriter
    EFBIG   = 14,   // File too large
    ENOSPC  = 15,   // No space left on device
    ENOSYS  = 16,   // Function not implemented
    ERANGE  = 17,   // Math result not representable
};
#endif
