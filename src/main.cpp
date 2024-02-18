#include "HttpRequest.hpp"
#include <iostream>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>

#define PORT 5678

int main() {

    int socketFd = socket(AF_INET, SOCK_STREAM, 0);
    // HttpRequest request = new HttpRequest()

    if (socketFd < 0) {
        std::cout << "Invalid socket creation!" << std::endl;
        return (1);
    }

    struct sockaddr_in address;

    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(PORT);

    int bindFd = bind(socketFd, (sockaddr *)&address, sizeof(address));
    if (bindFd < 0) {
        std::cout << "Cannot bind socket!" << std::endl;
        return (1);
    }

    if (listen(socketFd, 3) < 0) {
        std::cout << "Cannot listen socket" << std::endl;
    }

    
    return (0);
}