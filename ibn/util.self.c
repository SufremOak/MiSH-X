#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "neo32.h"

#define MAX_CMD_LEN 1024
#define MAX_ARGS 64

static void print_usage() {
    printf("Usage: mish [options] [script]\n");
    printf("\nOptions:\n");
    printf("  -h, --help     Show this help message\n");
    printf("  -v, --version  Show version information\n");
    printf("\nIf no script is provided, runs in interactive mode.\n");
}

static void print_version() {
    printf("MiSH version 0.1.0\n");
}

static int process_args(int argc, char *argv[]) {
    for (int i = 1; i < argc; i++) {
        if (strcmp(argv[i], "-h") == 0 || strcmp(argv[i], "--help") == 0) {
            print_usage();
            return 1;
        }
        else if (strcmp(argv[i], "-v") == 0 || strcmp(argv[i], "--version") == 0) {
            print_version();
            return 1;
        }
    }
    return 0;
}

static void interactive_loop() {
    char cmd[MAX_CMD_LEN];
    char *args[MAX_ARGS];
    char *token;
    int arg_count;

    printf("MiSH Interactive Shell\n");
    printf("Type 'exit' to quit\n\n");

    while (1) {
        printf("mish> ");
        if (!fgets(cmd, MAX_CMD_LEN, stdin)) {
            break;
        }

        // Remove trailing newline
        cmd[strcspn(cmd, "\n")] = 0;

        // Exit command
        if (strcmp(cmd, "exit") == 0) {
            break;
        }

        // Parse command into arguments
        arg_count = 0;
        token = strtok(cmd, " ");
        while (token && arg_count < MAX_ARGS) {
            args[arg_count++] = token;
            token = strtok(NULL, " ");
        }

        if (arg_count > 0) {
            // TODO: Execute command with args
            printf("Command not implemented: %s\n", args[0]);
        }
    }
}

int main(int argc, char *argv[]) {
    // Process command line arguments
    if (process_args(argc, argv)) {
        return 0;
    }

    // If script file provided, execute it
    if (argc > 1) {
        printf("Script execution not implemented yet\n");
        return 1;
    }

    // Otherwise run interactive mode
    interactive_loop();
    return 0;
}
