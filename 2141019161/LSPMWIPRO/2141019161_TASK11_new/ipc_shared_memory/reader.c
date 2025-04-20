#include <stdio.h>
#include <stdlib.h>
#include <sys/ipc.h>
#include <sys/shm.h>

#define SHM_SIZE 1024

int main() {
    key_t key = ftok("shmfile", 65);  // Use the same key as writer
    if (key == -1) {
        perror("ftok error");
        exit(EXIT_FAILURE);
    }

    int shmid = shmget(key, SHM_SIZE, 0666);  // Access the existing shared memory
    if (shmid == -1) {
        perror("shmget error");
        exit(EXIT_FAILURE);
    }

    char *shared_data = (char *) shmat(shmid, NULL, 0);  // Attach to shared memory
    if (shared_data == (char *)(-1)) {
        perror("shmat error");
        exit(EXIT_FAILURE);
    }

    // Display the message
    printf("Message received from shared memory: %s\n", shared_data);

    shmdt(shared_data);  // Detach from shared memory

    // Remove the shared memory segment
    shmctl(shmid, IPC_RMID, NULL);

    return EXIT_SUCCESS;
}
