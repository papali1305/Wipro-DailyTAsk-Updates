#include <stdio.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <string.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <unistd.h>

int main() {
    pid_t pid; 
    int shmid, *shvar;
    key_t key;

    // Generate a unique key for the shared memory
    key = ftok(".", 65);
    
    // Create a shared memory segment
    shmid = shmget(key, sizeof(int), 0664 | IPC_CREAT);
    printf("key=%x .......shmid=%d\n", key, shmid);
    
    // Attach to the shared memory segment
    shvar = (int *)shmat(shmid, NULL, 0);  // Cast to (int*)
    
    // Initialize the shared memory value
    *shvar = 0;  // Initialize to 0 before using it
    printf("default initial value of shvar=%d\n", *shvar);
    
    // Set initial value
    *shvar = 10;
    
    // Create a child process
    pid = fork();
    if (pid == 0) {
        // Child process updates the shared variable
        *shvar = *shvar + 90;
        printf("child update=%d\n", *shvar);
        exit(0);
    } else {
        // Parent process waits for the child to finish
        wait(NULL);
        // Parent process updates the shared variable
        *shvar = *shvar + 110;
        printf("parent update=%d\n", *shvar);
    }

    // Detach from the shared memory segment
    shmdt(shvar);
    
    // Optionally, remove the shared memory segment
    shmctl(shmid, IPC_RMID, NULL);

    return 0;
}
