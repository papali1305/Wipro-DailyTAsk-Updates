#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/ipc.h>
#include <sys/shm.h>

#define SHM_SIZE 1024

int main() {
    key_t key = ftok("shmfile", 65);  // Generate unique key using file
    if (key == -1) {
        perror("ftok error");
        exit(EXIT_FAILURE);
    }

    int shmid = shmget(key, SHM_SIZE, 0666 | IPC_CREAT);  // Create shared memory segment
    if (shmid == -1) {
        perror("shmget error");
        exit(EXIT_FAILURE);
    }

    char *shared_data = (char *) shmat(shmid, NULL, 0);  // Attach shared memory
    if (shared_data == (char *)(-1)) {
        perror("shmat error");
        exit(EXIT_FAILURE);
    }

    // Get message from user
    printf("Enter a message to share: ");
    fgets(shared_data, SHM_SIZE, stdin);  // Store in shared memory

    printf("Message written to shared memory.\n");

    shmdt(shared_data);  // Detach

    return EXIT_SUCCESS;
}
