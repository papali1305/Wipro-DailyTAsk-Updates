#include <iostream> 
#include <cstring> 
#include <ctime> 
#include <unistd.h> 
#include <arpa/inet.h> 
#include <sys/socket.h> 
#include <sys/types.h> 
#include <netinet/in.h>
#include <cstdio>

int main() { 
    int listenfd, connfd, len; 
    struct sockaddr_in servaddr, clientaddr; 
    char buff[1024]; 
    time_t ticks;

    listenfd = socket(AF_INET, SOCK_STREAM, 0);
    if (listenfd < 0) {
        std::cerr << "Socket creation failed" << std::endl;
        return 1;
    }

    memset(&servaddr, 0, sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
    servaddr.sin_port = htons(0);  // Let OS assign port

    if (bind(listenfd, (struct sockaddr *)&servaddr, sizeof(servaddr)) < 0) {
        std::cerr << "Bind failed" << std::endl;
        return 1;
    }

    len = sizeof(servaddr);
    getsockname(listenfd, (struct sockaddr *)&servaddr, (socklen_t *)&len);
    std::cout << "After bind, ephemeral port = " << ntohs(servaddr.sin_port) << std::endl;

    listen(listenfd, 5);

    connfd = accept(listenfd, (struct sockaddr *)&clientaddr, (socklen_t *)&len);
    if (connfd < 0) {
        std::cerr << "Accept failed" << std::endl;
        return 1;
    }

    ticks = time(NULL);
    snprintf(buff, sizeof(buff), "%s", ctime(&ticks));
    write(connfd, buff, strlen(buff));
    write(connfd, "ITER.\n", 6);

    // Chat loop
    while (true) {
        memset(buff, 0, sizeof(buff));
        int n = read(connfd, buff, sizeof(buff));
        if (n <= 0) {
            std::cout << "Client disconnected or read error." << std::endl;
            break;
        }

        std::cout << "From client: " << buff;

        std::cout << "To client: ";
        std::cin.getline(buff, sizeof(buff));
        write(connfd, buff, strlen(buff));
    }

    close(connfd);
    close(listenfd);
    return 0;
}

