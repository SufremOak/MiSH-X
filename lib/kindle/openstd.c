#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <errno.h>

// Kindle-specific paths
#define KINDLE_ROOT "/mnt/us"
#define KINDLE_DOCUMENTS "/mnt/us/documents"
#define KINDLE_SYSTEM "/mnt/base-us"
#define KINDLE_CONFIG "/var/local/config"

// Common Kindle file operations
int kindle_file_exists(const char *path) {
    return access(path, F_OK) == 0;
}

int kindle_is_jailbroken() {
    return kindle_file_exists("/mnt/us/jailbreak");
}

char* kindle_get_serial() {
    static char serial[32];
    FILE *f = fopen("/proc/usid", "r");
    if (!f) return NULL;
    
    fgets(serial, sizeof(serial), f);
    fclose(f);
    
    // Remove newline if present
    serial[strcspn(serial, "\n")] = 0;
    return serial;
}

// Kindle system info
char* kindle_get_version() {
    static char version[64];
    FILE *f = fopen("/etc/version", "r");
    if (!f) return NULL;
    
    fgets(version, sizeof(version), f);
    fclose(f);
    version[strcspn(version, "\n")] = 0;
    return version;
}

// Kindle file system operations
int kindle_create_directory(const char *path) {
    return mkdir(path, 0755);
}

int kindle_delete_file(const char *path) {
    return unlink(path);
}

// Kindle document management
int kindle_import_document(const char *src_path, const char *title) {
    char dest_path[PATH_MAX];
    snprintf(dest_path, sizeof(dest_path), "%s/%s", KINDLE_DOCUMENTS, title);
    
    int src_fd = open(src_path, O_RDONLY);
    if (src_fd < 0) return -1;
    
    int dest_fd = open(dest_path, O_WRONLY | O_CREAT, 0644);
    if (dest_fd < 0) {
        close(src_fd);
        return -1;
    }
    
    char buf[4096];
    ssize_t bytes_read;
    
    while ((bytes_read = read(src_fd, buf, sizeof(buf))) > 0) {
        if (write(dest_fd, buf, bytes_read) != bytes_read) {
            close(src_fd);
            close(dest_fd);
            return -1;
        }
    }
    
    close(src_fd);
    close(dest_fd);
    return 0;
}

// Kindle system operations
int kindle_reboot() {
    return system("reboot");
}

int kindle_poweroff() {
    return system("poweroff");
}

// Error handling
const char* kindle_strerror() {
    return strerror(errno);
}
