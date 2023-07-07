#include <iostream>
#include <winsock2.h>
#include <ws2tcpip.h>
#include <string>
#include <thread>

#define DEFAULT_PORT 12345
#define BUFFER_SIZE 1024

void SendThread(SOCKET clientSocket) {
    std::string userInput;
    while (true) {
        std::cout << "Enter a message (or 'q' to quit): ";
        std::getline(std::cin, userInput);

        if (userInput == "q")
            break;

        // send the message to the server
        if (send(clientSocket, userInput.c_str(), userInput.length(), 0) == SOCKET_ERROR) {
            std::cerr << "Failed to send data to server" << std::endl;
            break;
        }
    }
}

void ReceiveThread(SOCKET clientSocket) {
    char buffer[BUFFER_SIZE];
    while (true) {
        // receive the message from the server
        int bytesReceived = recv(clientSocket, buffer, BUFFER_SIZE, 0);
        if (bytesReceived == SOCKET_ERROR) {
            std::cerr << "Failed to receive data from server" << std::endl;
            break;
        }

        if (bytesReceived == 0) {
            std::cout << "Server disconnected" << std::endl;
            break;
        }

        std::cout << "Server echoed: " << std::string(buffer, bytesReceived) << std::endl;
    }
}

int main() {
    // contains information about the Windows Sockets implementation.
    WSADATA wsaData;
    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
        std::cerr << "Failed to initialize winsock" << std::endl;
        return 1;
    }

    // create a socket for the client
    SOCKET clientSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (clientSocket == INVALID_SOCKET) {
        std::cerr << "Failed to create socket" << std::endl;
        WSACleanup();
        return 1;
    }
    //26-30 poate fi modificata mai eficient cu un fisier de configurare 
    // connect to the server
    sockaddr_in serverAddress;
    serverAddress.sin_family = AF_INET;
    inet_pton(AF_INET, "127.0.0.1", &(serverAddress.sin_addr)); // Change this IP address to the server's IP
    serverAddress.sin_port = htons(DEFAULT_PORT);

    if (connect(clientSocket, (struct sockaddr*)&serverAddress, sizeof(serverAddress)) == SOCKET_ERROR) {
        std::cerr << "Failed to connect to server" << std::endl;
        closesocket(clientSocket);
        WSACleanup();
        return 1;
    }

    std::cout << "Connected to server" << std::endl;

    std::thread sendThread(SendThread, clientSocket);
    std::thread receiveThread(ReceiveThread, clientSocket);
    sendThread.join();
    receiveThread.join();


    closesocket(clientSocket);
    WSACleanup();

    return 0;
}
