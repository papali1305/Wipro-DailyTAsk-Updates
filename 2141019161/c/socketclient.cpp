#include <iostream>
#include <cstring>
#include <cstdlib>
#include <unistd.h>
#include <arpa/inet.h>

using namespace std;

int main(int argc, char *argv[]) {
    if (argc != 3) {
        cerr << "Usage: " << argv[0] << " <IP Address> <Port>" << endl;
        return 1;
    }

    int sockfd;
    char buff[1024];
    struct sockaddr_in servaddr;

    // Create socket
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0) {
        cerr << "Error: Unable to create socket" << endl;
        return 1;
    }

    // Prepare server address
    memset(&servaddr, 0, sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = inet_addr(argv[1]);
    servaddr.sin_port = htons(atoi(argv[2]));

    // Connect to server
    if (connect(sockfd, (struct sockaddr *)&servaddr, sizeof(servaddr)) < 0) {
        cerr << "Error: Connection failed" << endl;
        close(sockfd);
        return 1;
    }

    cout << "Connected to the server. Type messages and press Enter to send (Ctrl+C to quit):" << endl;

    // Loop to send and receive messages
    while (true) {
        memset(buff, 0, sizeof(buff));

        // Read user input
        cout << "You: ";
        cin.getline(buff, sizeof(buff));

        // Send message to server
        write(sockfd, buff, strlen(buff));

        // Receive response
        memset(buff, 0, sizeof(buff));
        int n = read(sockfd, buff, sizeof(buff) - 1);
        if (n <= 0) {
            cerr << "Disconnected from server or error reading response." << endl;
            break;
        }

        cout << "Server: " << buff << endl;
    }

    close(sockfd);
    return 0;
}
