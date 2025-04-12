// USE wiat() to track child completion 
// ensure the parent waits for all child processes to finish . Handle process status and error 

#include <unistd.h>
#include <iostream>
#include <sys/wait.h>
#include <vector>
#include <cstring> // For strerror
#include <errno.h> // For errno

using namespace std;

int main(int argc, char* argv[]) {
    if (argc < 3) {
        cerr << "Usage: " << argv[0] << " <keyword> <file1> <file2> ...\n";
        return 1;
    }

    const char* keyword = argv[1];
    int num_files = argc - 2;

    cout << "Searching for keyword: " << keyword << "\n";
    cout << "Files to search:\n";
    for (int i = 2; i < argc; ++i) {
        cout << "-" << argv[i] << "\n";
    }

    // Create pipes
    vector<int[2]> pipes(num_files);
    for (int i = 0; i < num_files; ++i) {
        if (pipe(pipes[i]) == -1) {
            perror("pipe failed");
            return 1;
        }
    }

    for (int i = 0; i < num_files; ++i) {
        const char* filename = argv[i + 2];
        pid_t pid = fork();

        if (pid == 0) {
            // Child process
            // Close the read end of the pipe
            close(pipes[i][0]);
            // Redirect stdout to the write end of the pipe
            dup2(pipes[i][1], STDOUT_FILENO);
            close(pipes[i][1]); // Close the original write end

            // Execute grep
            execlp("grep", "grep", keyword, filename, (char*)nullptr);
            // If execlp fails
            cerr << "Failed to execute grep for file: " << filename << "\n";
            return 1;
        } else if (pid > 0) {
            // Parent process
            close(pipes[i][1]); // Close the write end of the pipe
            cout << "Child process created for file: " << filename << "\n";
        } else {
            perror("fork failed");
        }
    }

    // Parent process reads from pipes
    for (int i = 0; i < num_files; ++i) {
        char buffer[1024];
        ssize_t bytesRead;

        // Read from the pipe until EOF
        cout << "Results from " << argv[i + 2] << ":\n"; // Print filename before results
        while ((bytesRead = read(pipes[i][0], buffer, sizeof(buffer) - 1)) > 0) {
            buffer[bytesRead] = '\0'; // Null-terminate the string
            cout << buffer; // Print the output directly
        }

        close(pipes[i][0]); // Close the read end of the pipe
    }

    // Wait for all child processes to finish and handle their exit status
    for (int i = 0; i < num_files; ++i) {
        int status;
        pid_t child_pid = wait(&status); // Wait for any child process

        if (child_pid == -1) {
            perror("wait failed");
        } else {
            if (WIFEXITED(status)) {
                int exit_status = WEXITSTATUS(status);
                if (exit_status == 0) {
                    cout << "Child process " << child_pid << " completed successfully.\n";
                } else {
                    cout << "Child process " << child_pid << " exited with status " << exit_status << ".\n";
                }
            } else {
                cout << "Child process " << child_pid << " did not terminate normally.\n";
            }
        }
    }

    return 0;
}
