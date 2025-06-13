// tcp_server_windows.cpp
#include <iostream>
#include <winsock2.h>
#pragma comment(lib, "ws2_32.lib")  // Link with ws2_32.lib

#define PORT 54000
#define BUFFER_SIZE 1024

int main() {
    WSADATA wsaData;
    SOCKET server_socket, client_socket;
    sockaddr_in server_addr{}, client_addr{};
    int client_size = sizeof(client_addr);
    char buffer[BUFFER_SIZE];

    // Initialize Winsock
    if (WSAStartup(MAKEWORD(2, 2), &wsaData)) {
        std::cerr << "WSAStartup failed\n";
        return 1;
    }

    // Create socket
    server_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (server_socket == INVALID_SOCKET) {
        std::cerr << "Socket creation failed\n";
        WSACleanup();
        return 1;
    }

    // Setup address
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_port = htons(PORT);

    // Bind
    if (bind(server_socket, (sockaddr*)&server_addr, sizeof(server_addr)) == SOCKET_ERROR) {
        std::cerr << "Bind failed\n";
        closesocket(server_socket);
        WSACleanup();
        return 1;
    }

    // Listen
    if (listen(server_socket, SOMAXCONN) == SOCKET_ERROR) {
        std::cerr << "Listen failed\n";
        closesocket(server_socket);
        WSACleanup();
        return 1;
    }

    std::cout << "Server listening on port " << PORT << "...\n";

    // Accept
    client_socket = accept(server_socket, (sockaddr*)&client_addr, &client_size);
    if (client_socket == INVALID_SOCKET) {
        std::cerr << "Accept failed\n";
        closesocket(server_socket);
        WSACleanup();
        return 1;
    }

    std::cout << "Client connected.\n";

    // Communication loop
    while (true) {
        memset(buffer, 0, BUFFER_SIZE);
        int bytes_received = recv(client_socket, buffer, BUFFER_SIZE, 0);
        if (bytes_received <= 0) {
            std::cout << "Connection closed.\n";
            break;
        }

        std::cout << "Client: " << buffer << std::endl;
        send(client_socket, buffer, bytes_received, 0);  // Echo
    }

    closesocket(client_socket);
    closesocket(server_socket);
    WSACleanup();
    return 0;
}
