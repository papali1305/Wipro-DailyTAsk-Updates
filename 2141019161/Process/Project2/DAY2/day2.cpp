//Use exec() to Run grep in each child 

//grep <keyword> <filename>

// Day 1- Set up input and process spawning

//take a list of files names and a keyword from the user. Create a child process (fork()) for each file.

#include <unistd.h>
#include <iostream>
#include <sys/wait.h>
#include <vector>

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

    for (int i = 0; i < num_files; ++i) {
        const char* filename = argv[i + 2];
        pid_t pid = fork();

        if (pid == 0) {
            // Child process runs grep <keyword> <filename>
            execlp("grep", "grep", keyword, filename, (char*)nullptr);
            // If execlp fails
            cerr << "Failed to execute grep for file: " << filename << "\n";
            return 1;
        } else if (pid > 0) {
            // Parent process
            cout << "Child process created for file: " << filename << "\n";
        } else {
            perror("fork failed");
        }
    }

    // Parent process waits for all child processes to finish
    for (int i = 0; i < num_files; ++i) {
        wait(nullptr);
    }

    return 0;
}
        
      

