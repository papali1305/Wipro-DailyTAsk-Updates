#include <stdio.h>
#include <stdlib.h>
#include <sys/ipc.h>
#include <sys/shm.h>

#define SHM_SIZE 1024

int main() {
    key_t key = ftok("shmfile", 65); // same key as writer
    int shmid = shmget(key, SHM_SIZE, 0666); // get shared memory

    if (shmid == -1) {
        perror("shmget failed");
        return 1;
    }

    char *str = (char *) shmat(shmid, (void *)0, 0); // attach
    if (str == (char *)-1) {
        perror("shmat failed");
        return 1;
    }

    printf("Data read from shared memory: %s\n", str);

    shmdt(str); // detach
    shmctl(shmid, IPC_RMID, NULL); // remove shared memory

    return 0;
}
