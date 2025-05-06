#!/bin/sh
# Name: Open Kindle Standard Unix
# Author: SufremOak
# Icon: data:image/png;base64,iVBORw0KGgoAAAANSUhEUgAAABAAAAAQCAYAAAAf8/9hAAAABHNCSVQICAgIfAhkiAAAAAlwSFlzAAAAdgAAAHYBTnsmCAAAABl0RVh0U29mdHdhcmUAd3d3Lmlua3NjYXBlLm9yZ5vuPBoAAADDSURBVDiNY2AYBYMPMDIyMjAyMjL8//+f4f///0ygbAYmJqb/TExM/5mYmP7/+/fvPwMDw39GRkYGmDqwS0F6mZiY/oMMgxkEMxRuAMhAmEEgzWBXgABII8xwuAuYmJj+Mf3//5+RiYnpH8gwmM0wL8BcBTMYxRCYQTBXwAyBGQIzBG4AzEaYQTBDwF6AGQIzBGYIzBCYIXADYDbCDIIZAjMEZgjMEJghMEPgBsBshBkEMwRmCMwQmCEwQ2CGMIyCQQYA8kgk6OA3kn0AAAAASUVORK5CYII=
# UseHooks

set -e

on_install() {
    echo "Installing Open Kindle Standard Unix..." >> /mnt/us/logs/install.log
    echo "Open Kindle Standard Unix installed successfully." >> /mnt/us/logs/install.log
}

LIBSOURCE="
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
}"

if [! command -v "bash" &> /dev/null ]; then
    echo "bash could not be found"
    exit
else
    if [ -f "$HOME/.bashrc" ]; then
        source "$HOME/.bashrc"
    fi
    if [ ! command -v "coreutils" &> /dev/null ]; then
        echo "coreutils could not be found"
        exit
    else
        if [ ! command -v "libc" &> /dev/null ]; then
            echo "libc could not be found"
            exit
        else
            if [ ! command -v "libstdc++" &> /dev/null ]; then
                echo "libstdc++ could not be found"
                exit
            else
                if [ ! command -v "libgcc" &> /dev/null ]; then
                    echo "libgcc could not be found"
                    exit
                else
                    if [ ! command -v "libm" &> /dev/null ]; then
                        echo "libm could not be found"
                        exit
                    fi
                fi
            fi
    fi
fi

if [ -f "/usr/lib/libc.so.6" ]; then
    echo "libc found"
else
    echo "libc not found"
    exit
fi

unix4kindle() {
    local kindle_root="/mnt/us"
    local kindle_documents="$kindle_root/documents"
    local kindle_system="$kindle_root/system"
    local kindle_config="$kindle_root/config"
    local kindle_serial
    local kindle_version
    local kindle_jailbroken
    local kindle_serial_file="/proc/usid"
    local kindle_version_file="/etc/version"
    local kindle_jailbroken_file="/mnt/us/jailbreak"
    local kindle_documents_dir="$kindle_documents"
    local kindle_system_dir="$kindle_system"
    local kindle_config_dir="$kindle_config"

    micro-kdk() {
        local src_path="$1"
        local title="$2"
        local dest_path="$KINDLE_DOCUMENTS/$title"
        
        if [ -f "$src_path" ]; then
            cp "$src_path" "$dest_path"
            echo "Document imported to $dest_path"
        else
            echo "Source file not found: $src_path"
            return 1
        fi
    }

    unixSystem() {
        local systemName="$1"
        local systemPath="$KINDLE_SYSTEM/$systemName"
        local systemFile="$systemPath/$systemName"
        local systemDir="$systemPath"
        local systemConfig="$KINDLE_CONFIG/$systemName"
        local systemConfigFile="$systemConfig/$systemName"
        local systemConfigDir="$systemConfig"
        local systemConfigFilePath="$systemConfig/$systemName"
        local UnixSystemDir="$kindle_root/unix"

        if [ ! -d "$UnixSystemDir" ]; then
            echo "Creating directory: $UnixSystemDir"
            mkdir -p "$UnixSystemDir/{home,usr,bin,lib}"
            touch $UnixSystemDir/home
            mkdir -p $UnixSystemDir/usr/{bin,lib,local}
            mkdir -p $UnixSystemDir/usr/local/{bin,lib}
            if command -v python &> /dev/null; then
                echo "Python is installed on the Kindle."
                echo "Starting micro Unix process"
                python -c "import os; os.system('[PyLog]: Starting Unix System via Python')"
            else
                echo "Python is not installed on the Kindle."
            fi
        fi                
    }
}