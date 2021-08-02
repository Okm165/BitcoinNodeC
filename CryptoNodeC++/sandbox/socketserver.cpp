#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <cassert>
#include <iostream>
#include <string.h>
#define PORT 8080

int main()
{
    int server_fd, incoming_socket, valread;
    sockaddr_in address;
    int opt = 1;
    char buffer[1024];
    const char* hello = "Hello from server";

    assert((server_fd = socket(AF_INET, SOCK_STREAM, 0)) != -1);
    assert(!setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof(opt)));

    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(PORT);

    assert(bind(server_fd, (sockaddr*)&address, sizeof(address)) == 0);
    assert(listen(server_fd, 10) == 0);
    std::cout << "listening..." << std::endl;
    socklen_t addrlen = sizeof(address);
    assert((incoming_socket = accept(server_fd, (sockaddr*)&address, &addrlen)) != -1);

    do
    {
        memset(buffer, 0, 1023);
        valread = read(incoming_socket, buffer, 1023);
        std::cout << buffer << std::endl;
    }
    while(valread == 1023);
}