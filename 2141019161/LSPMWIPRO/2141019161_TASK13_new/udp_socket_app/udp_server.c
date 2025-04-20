// udp_server.c - Manual UDP chat server
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

#define PORT 8080
#define MAX_LEN 1024

int main() {
    int sockfd;
    char buffer[MAX_LEN];
    struct sockaddr_in server_addr, client_addr;
    socklen_t addr_len = sizeof(client_addr);

    // Create UDP socket
    if ((sockfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0) {
        perror("Socket creation failed");
        exit(EXIT_FAILURE);
    }

    // Setup server address
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(PORT);
    server_addr.sin_addr.s_addr = INADDR_ANY;

    // Bind socket to the server address
    if (bind(sockfd, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0) {
        perror("Bind failed");
        close(sockfd);
        exit(EXIT_FAILURE);
    }

    printf("📡 UDP Server started on port %d\n", PORT);

    while (1) {
        // Receive message from client
        int n = recvfrom(sockfd, buffer, MAX_LEN, 0, (struct sockaddr *)&client_addr, &addr_len);
        buffer[n] = '\0';
        printf("\n👤 Client: %s\n", buffer);

        // Exit if client says "exit"
        if (strncmp(buffer, "exit", 4) == 0) {
            printf("Client ended the chat.\n");
            break;
        }

        // Send manual response
        printf("📝 Enter reply to client: ");
        fgets(buffer, MAX_LEN, stdin);
        buffer[strcspn(buffer, "\n")] = '\0'; // Remove newline

        sendto(sockfd, buffer, strlen(buffer), 0, (struct sockaddr *)&client_addr, addr_len);

        if (strncmp(buffer, "exit", 4) == 0) {
            printf("Server ended the chat.\n");
            break;
        }
    }

    close(sockfd);
    return 0;
}
