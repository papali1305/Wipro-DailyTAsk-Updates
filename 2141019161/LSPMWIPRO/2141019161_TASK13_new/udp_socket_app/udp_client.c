// udp_client.c - Manual UDP chat client
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

#define SERVER_PORT 8080
#define SERVER_IP "127.0.0.1"
#define MAX_LEN 1024

int main() {
    int sockfd;
    char buffer[MAX_LEN];
    struct sockaddr_in server_addr;
    socklen_t addr_len = sizeof(server_addr);

    // Create UDP socket
    if ((sockfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0) {
        perror("Socket creation failed");
        exit(EXIT_FAILURE);
    }

    // Setup server address
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(SERVER_PORT);
    server_addr.sin_addr.s_addr = inet_addr(SERVER_IP);

    printf("📲 UDP Client started. Type messages below.\n");

    while (1) {
        // Manual message input
        printf("📝 Enter message to server: ");
        fgets(buffer, MAX_LEN, stdin);
        buffer[strcspn(buffer, "\n")] = '\0'; // Remove newline

        sendto(sockfd, buffer, strlen(buffer), 0, (struct sockaddr *)&server_addr, addr_len);

        if (strncmp(buffer, "exit", 4) == 0) {
            printf("Client ended the chat.\n");
            break;
        }

        // Receive reply from server
        int n = recvfrom(sockfd, buffer, MAX_LEN, 0, (struct sockaddr *)&server_addr, &addr_len);
        buffer[n] = '\0';
        printf("\n👨‍💻 Server: %s\n", buffer);

        if (strncmp(buffer, "exit", 4) == 0) {
            printf("Server ended the chat.\n");
            break;
        }
    }

    close(sockfd);
    return 0;
}
