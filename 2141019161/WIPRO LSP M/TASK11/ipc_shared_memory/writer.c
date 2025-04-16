#include <stdio.h>
#include <stdlib.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <string.h>

#define SHM_SIZE 1024  // size of shared memory

int main() {
    key_t key = ftok("shmfile", 65); // generate unique key
    int shmid = shmget(key, SHM_SIZE, 0666 | IPC_CREAT); // create shared memory

    if (shmid == -1) {
        perror("shmget failed");
        return 1;
    }

    char *str = (char *) shmat(shmid, (void *)0, 0); // attach to memory
    if (str == (char *)-1) {
        perror("shmat failed");
        return 1;
    }

    printf("Writing to shared memory...\n");
    strcpy(str, "Hello from Writer Process using Shared Memory!");

    printf("Done. Detaching and exiting.\n");
    shmdt(str); // detach

    return 0;
}
