#ifndef NEO32_H
#define NEO32_H

#ifdef __cplusplus
namespace Neo32 {
#endif

#ifdef _WIN32
    #include <windows.h>
    #define PATH_SEPARATOR "\\"
    #define PATH_SEPARATOR_CHAR '\\'
    
    // Unix types for Windows
    typedef int pid_t;
    typedef int uid_t;
    typedef int gid_t;
    typedef int mode_t;
    typedef long ssize_t;
    typedef unsigned long size_t;
    typedef int pthread_t;
    typedef void DIR;
    struct dirent {
        char d_name[MAX_PATH];
    };
    struct stat {
        mode_t st_mode;
        size_t st_size;
        time_t st_atime;
        time_t st_mtime;
        time_t st_ctime;
    };
#else
    #include <unistd.h>
    #include <sys/types.h>
    #include <sys/stat.h>
    #include <dirent.h>
    #include <pthread.h>
    #include <errno.h>
    #include <fcntl.h>
    #define PATH_SEPARATOR "/"
    #define PATH_SEPARATOR_CHAR '/'
    
    // Windows types mapped to POSIX equivalents
    typedef int BOOL;
    typedef uint32_t DWORD;
    typedef int HANDLE;  // File descriptors in POSIX
    typedef unsigned int UINT;
    typedef const char* LPCSTR;
    typedef char* LPSTR;
    typedef void* LPVOID;
    typedef size_t SIZE_T;
    typedef pthread_t THREAD_HANDLE;
    
    // Common Windows constants
    #define TRUE 1
    #define FALSE 0
    #define INVALID_HANDLE_VALUE (-1)
    #define MAX_PATH PATH_MAX
    
    // Access rights mapping
    #define GENERIC_READ O_RDONLY
    #define GENERIC_WRITE O_WRONLY
    #define GENERIC_READWRITE O_RDWR
    
    // Share modes mapping
    #define FILE_SHARE_READ 1
    #define FILE_SHARE_WRITE 2
    #define FILE_SHARE_DELETE 4
    
    // Error codes mapping
    #define ERROR_SUCCESS 0
    #define ERROR_FILE_NOT_FOUND ENOENT
    #define ERROR_PATH_NOT_FOUND ENOENT
    #define ERROR_ACCESS_DENIED EACCES
    #define ERROR_INVALID_HANDLE EBADF
#endif

// File operations
HANDLE CreateFile(const char* filename, DWORD access, DWORD share_mode);
BOOL CloseHandle(HANDLE handle);
BOOL ReadFile(HANDLE handle, LPVOID buffer, DWORD bytes_to_read, DWORD* bytes_read);
BOOL WriteFile(HANDLE handle, LPVOID buffer, DWORD bytes_to_write, DWORD* bytes_written);

// Directory operations
BOOL CreateDirectory(const char* path);
BOOL RemoveDirectory(const char* path);
BOOL DirectoryExists(const char* path);

// Path operations
void GetFullPath(const char* relative_path, char* full_path, SIZE_T buffer_size);
void CombinePath(const char* path1, const char* path2, char* result, SIZE_T buffer_size);
BOOL FileExists(const char* filename);

// Process/Thread operations
#ifdef _WIN32
HANDLE CreateThread(LPVOID thread_func, LPVOID param);
#else
THREAD_HANDLE CreateThread(void *(*thread_func)(void*), LPVOID param);
#endif
DWORD GetCurrentProcessId(void);
void Sleep(DWORD milliseconds);

// Memory operations
LPVOID AllocateMemory(SIZE_T size);
BOOL FreeMemory(LPVOID ptr);

// Error handling
DWORD GetLastError(void);
void SetLastError(DWORD error_code);

#ifdef __cplusplus
}
#endif

#endif // NEO32_H
