// UDP Client Program in C++
#include <iostream> 
#include <cstring> 
#include <ctime> 
#include <unistd.h> 
#include <arpa/inet.h> 
#include <sys/socket.h> 
#include <sys/types.h> 
#include <netinet/in.h>


using namespace std;

int main(int argc, char* argv[]) {
    int sockfd, n, len;
    char recvline[1024];
    struct sockaddr_in servaddr;

    sockfd = socket(AF_INET, SOCK_DGRAM, 0);

    memset(&servaddr, 0, sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = inet_addr("127.0.0.1");


    servaddr.sin_port = htons(atoi(argv[1]));
    len = sizeof(servaddr);

    cout << "Give a number for server: ";
    cin >> n;

    sendto(sockfd, &n, sizeof(n), 0, (struct sockaddr*)&servaddr, len);

    recvfrom(sockfd, recvline, sizeof(recvline) - 1, 0, (struct sockaddr*)&servaddr, (socklen_t*)&len);

    recvline[50] = '\0'; // Ensure null termination
    cout << "\nServer sent: " << recvline << endl;

    close(sockfd);
    return 0;
}
