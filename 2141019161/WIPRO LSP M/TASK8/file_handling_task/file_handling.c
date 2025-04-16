#include <stdio.h>
#include <stdlib.h>

int main() {
    FILE *fp;
    char buffer[100];

    // Open the file in write+read mode. Create if it doesn't exist.
    fp = fopen("data.txt", "w+");
    if (fp == NULL) {
        perror("Error opening file");
        return 1;
    }

    // Write to the file
    fputs("Hello, File Handling in C!", fp);

    // Move file pointer to the beginning
    rewind(fp);

    // Read contents back
    fgets(buffer, sizeof(buffer), fp);

    // Print to console
    printf("File Content: %s\n", buffer);

    // Close the file
    fclose(fp);

    return 0;
}
