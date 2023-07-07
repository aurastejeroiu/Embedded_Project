#include <iostream>
#include <winsock2.h>
#include <thread>
#include <vector>
#include <mutex>

#define DEFAULT_PORT 12345
#define BUFFER_SIZE 1024

std::mutex clientMutex; // Mutex for synchronizing access to client sockets
std::vector<SOCKET> clientSockets; // Vector to store client sockets

// Function to handle client communication
void handleClient(SOCKET clientSocket) {
    char buffer[BUFFER_SIZE];
    int bytesRead;

    while (true) {
        bytesRead = recv(clientSocket, buffer, BUFFER_SIZE, 0);
        if (bytesRead == SOCKET_ERROR) {
            std::cerr << "Failed to receive data from client" << std::endl;
            break;
        }

        if (bytesRead == 0) {
            std::cout << "Client disconnected" << std::endl;
            break;
        }

        std::cout << "Received message: " << std::string(buffer, bytesRead) << std::endl;
        
        std::lock_guard<std::mutex> lock(clientMutex); // Lock the mutex before sending data
        for (const auto& client : clientSockets) {
            if (client != clientSocket) {
                if (send(client, buffer, bytesRead, 0) == SOCKET_ERROR) {
                    std::cerr << "Failed to send data to client" << std::endl;
                    break;
                }
            }
        }
    }

    closesocket(clientSocket);

    std::lock_guard<std::mutex> lock(clientMutex); // Lock the mutex before removing the client socket
    clientSockets.erase(std::remove(clientSockets.begin(), clientSockets.end(), clientSocket), clientSockets.end());
}
//De ce se pot numi la fel lock-urile in aceasi functie?

int main() {
    // contains information about the Windows Sockets implementation.
    WSADATA wsaData;
    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
        std::cerr << "Failed to initialize winsock" << std::endl;
        return 1;
    }

    // create a socket for the server
    SOCKET serverSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (serverSocket == INVALID_SOCKET) {
        std::cerr << "Failed to create socket" << std::endl;
        WSACleanup();
        return 1;
    }

    // bind the socket to a specific IP address and port
    sockaddr_in serverAddress;
    serverAddress.sin_family = AF_INET;
    serverAddress.sin_addr.s_addr = INADDR_ANY;
    serverAddress.sin_port = htons(DEFAULT_PORT);

    if (bind(serverSocket, (struct sockaddr*)&serverAddress, sizeof(serverAddress)) == SOCKET_ERROR) {
        std::cerr << "Failed to bind socket" << std::endl;
        closesocket(serverSocket);
        WSACleanup();
        return 1;
    }

    // listen for incoming connections
    if (listen(serverSocket, SOMAXCONN) == SOCKET_ERROR) {
        std::cerr << "Failed to listen on socket" << std::endl;
        closesocket(serverSocket);
        WSACleanup();
        return 1;
    }

    std::cout << "Server listening on port " << DEFAULT_PORT << std::endl;

    std::vector<std::thread> clientThreads; // Vector to store client threads

    while (true) {
        // accept a client connection
        SOCKET clientSocket;
        sockaddr_in clientAddress;
        int clientAddressLength = sizeof(clientAddress);

        clientSocket = accept(serverSocket, (struct sockaddr*)&clientAddress, &clientAddressLength);
        if (clientSocket == INVALID_SOCKET) {
            std::cerr << "Failed to accept client connection" << std::endl;
            closesocket(serverSocket);
            WSACleanup();
            return 1;
        }

        std::cout << "Client connected" << std::endl;

        {
            std::lock_guard<std::mutex> lock(clientMutex); // Lock the mutex before adding the client socket
            clientSockets.push_back(clientSocket);
        }

        // Create a new thread for each client
        clientThreads.emplace_back(handleClient, clientSocket);
    }

    // Join all client threads before exiting
    for (auto& thread : clientThreads) {
        thread.join();
    }

    closesocket(serverSocket);

    WSACleanup();

    return 0;
}
