#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>
#include <unistd.h>
#include <sys/stat.h>

int main() {
    DIR *dir;
    struct dirent *entry;
    char cwd[1024];

    // Get and print current directory
    if (getcwd(cwd, sizeof(cwd)) != NULL) {
        printf("Current directory: %s\n", cwd);
    } else {
        perror("getcwd error");
        return 1;
    }

    // Open current directory
    dir = opendir(".");
    if (dir == NULL) {
        perror("opendir error");
        return 1;
    }

    // List files and directories
    printf("Listing files:\n");
    while ((entry = readdir(dir)) != NULL) {
        printf(" - %s\n", entry->d_name);
    }
    closedir(dir);

    // Change directory to /tmp
    if (chdir("/tmp") != 0) {
        perror("chdir error");
        return 1;
    }

    printf("Changed to /tmp directory.\n");

    // Create testdir with 755 permissions
    if (mkdir("testdir", 0755) != 0) {
        perror("mkdir error");
        return 1;
    }

    printf("Created directory /tmp/testdir with 755 permissions.\n");

    return 0;
}
