#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>
#include <unistd.h>
#include <sys/stat.h>

int main() {
    DIR *directory;
    struct dirent *entry;
    char currentPath[1024];

    // Retrieve and display the current working directory
    if (getcwd(currentPath, sizeof(currentPath)) != NULL) {
        printf("Current Directory: %s\n", currentPath);
    } else {
        perror("Error retrieving current directory");
        return EXIT_FAILURE;
    }

    // Attempt to open the current directory
    directory = opendir(".");
    if (directory == NULL) {
        perror("Error opening directory");
        return EXIT_FAILURE;
    }

    // Iterate and display files and subdirectories
    printf("Directory Contents:\n");
    while ((entry = readdir(directory)) != NULL) {
        printf(" - %s\n", entry->d_name);
    }

    closedir(directory);

    // Attempt to change directory to /tmp
    if (chdir("/tmp") != 0) {
        perror("Failed to change directory to /tmp");
        return EXIT_FAILURE;
    }

    printf("Successfully changed to /tmp directory.\n");

    // Create a new directory named "testdir" with 755 permissions
    if (mkdir("testdir", 0755) != 0) {
        perror("Failed to create /tmp/testdir");
        return EXIT_FAILURE;
    }

    printf("Directory '/tmp/testdir' created with 755 permissions.\n");

    return EXIT_SUCCESS;
}
