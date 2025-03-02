#include <iostream>
#include <iostream>
#include <cstring>       // For memset
#include <unistd.h>      // For close()
#include <sys/types.h>   // For socket types
#include <sys/socket.h>  // For socket functions
#include <netinet/in.h>  // For sockaddr_in and INADDR_ANY
#include <arpa/inet.h>   // For inet_pton

#define PORT 8080 // Port number to connect to
#define SERVER_IP "127.0.0.1" // IP address of the server (localhost in this case)


int main(){
    // 1. Create socket
    int sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0) {
        std::cerr << "Error creating socket!" << std::endl;
        return -1;
    }

    // 2. Set up the server address
    struct sockaddr_in server_addr;
    memset(&server_addr, 0, sizeof(server_addr));  // Zero out the struct
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(PORT); // Convert port number to network byte order

    // Convert the server IP address to binary form
    if (inet_pton(AF_INET, SERVER_IP, &server_addr.sin_addr) <= 0) {
        std::cerr << "Invalid address or Address not supported!" << std::endl;
        close(sockfd);
        return -1;
    }

    // 3. Connect to the server
    if (connect(sockfd, (struct sockaddr*)&server_addr, sizeof(server_addr)) < 0) {
        std::cerr << "Connection failed!" << std::endl;
        close(sockfd);
        return -1;
    }

}