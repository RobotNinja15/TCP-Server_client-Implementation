// tcp_client_windows.cpp
#include <iostream>
#include <winsock2.h>
#include <string>
#pragma comment(lib, "ws2_32.lib")  // Link with ws2_32.lib
#define _WINSOCK_DEPRECATED_NO_WARNINGS

#define PORT 54000
#define BUFFER_SIZE 1024

int main() {
    WSADATA wsaData;
    SOCKET client_socket;
    sockaddr_in server_addr{};
    char buffer[BUFFER_SIZE];

    // Initialize Winsock
    if (WSAStartup(MAKEWORD(2, 2), &wsaData)) {
        std::cerr << "WSAStartup failed\n";
        return 1;
    }

    // Create socket
    client_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (client_socket == INVALID_SOCKET) {
        std::cerr << "Socket creation failed\n";
        WSACleanup();
        return 1;
    }

    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(PORT);
    server_addr.sin_addr.s_addr = inet_addr("127.0.0.1");  // localhost

    // Connect
    if (connect(client_socket, (sockaddr*)&server_addr, sizeof(server_addr)) < 0) {
        std::cerr << "Connection failed\n";
        closesocket(client_socket);
        WSACleanup();
        return 1;
    }

    std::cout << "Connected to server.\n";

    // Communication loop
    while (true) {
        std::cout << "You: ";
        std::string message;
        std::getline(std::cin, message);
        if (message == "exit") break;

        send(client_socket, message.c_str(), message.length(), 0);

        memset(buffer, 0, BUFFER_SIZE);
        int bytes_received = recv(client_socket, buffer, BUFFER_SIZE, 0);
        if (bytes_received <= 0) {
            std::cout << "Server closed connection.\n";
            break;
        }

        std::cout << "Server: " << buffer << std::endl;
    }

    closesocket(client_socket);
    WSACleanup();
    return 0;
}
