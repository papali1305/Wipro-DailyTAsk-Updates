#include <iostream> 
#include <sys/socket.h> 

using namespace std;

int main() {
int sockfd;
sockfd = socket(AF_INET,SOCK_STREAM,0);
cout << sockfd << endl ;
return 0;
}

