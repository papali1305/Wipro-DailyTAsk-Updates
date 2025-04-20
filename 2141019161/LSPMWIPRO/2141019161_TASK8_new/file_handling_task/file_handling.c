#include <stdio.h>
#include <stdlib.h>

int main() {
    FILE *file;
    char content[100];


    file = fopen("data.txt", "w+");
    if (file == NULL) {
        perror("Failed to open or create the file");
        return EXIT_FAILURE;
    }

    //  message to the file
    const char *message = "Hello, File Handling in C!";
    fputs(message, file);

    // Reset the file pointer to the beginning
    rewind(file);

    // Read the contents back into the buffer
    if (fgets(content, sizeof(content), file) != NULL) {
        printf("File Content: %s\n", content);
    } else {
        printf("Failed to read from the file.\n");
    }

    // Close the file
    fclose(file);

    return EXIT_SUCCESS;
}
