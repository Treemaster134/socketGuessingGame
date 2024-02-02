#include <cstring> 
#include <iostream>
#include <winsock2.h> 
#include <unistd.h> 
#include "game.hpp"

int main() {
    //credit to https://medium.com/@tharunappu2004/creating-a-simple-tcp-server-in-c-using-winsock-b75dde86dd39 for basic setup

    // Initialize WSA variables
    WSADATA wsaData;
    int wsaerr;
    WORD wVersionRequested = MAKEWORD(2, 2);
    wsaerr = WSAStartup(wVersionRequested, &wsaData);
    
    if (wsaerr != 0) 
    {
        // Check for initialization success
        std::cout << "The Winsock dll not found!" << std::endl;
        return 0;
    }

    SOCKET serverSocket;
    serverSocket = INVALID_SOCKET;
    serverSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

    // Check for socket creation success
    if (serverSocket == INVALID_SOCKET) {
        std::cout << "Error at socket(): " << WSAGetLastError() << std::endl;
        WSACleanup();
        return 0;
    }
    
    char address[15];
    printf("Enter ip address: ");
    std::cin.getline(address, 15);
    
    sockaddr_in service;
    service.sin_family = AF_INET;
    service.sin_addr.s_addr = inet_addr(address);  // Replace with your desired IP address
    service.sin_port = htons(55555);  // Choose a port number

    // Use the bind function
    if (bind(serverSocket, reinterpret_cast<SOCKADDR*>(&service), sizeof(service)) == SOCKET_ERROR) {
        std::cout << "bind() failed: " << WSAGetLastError() << std::endl;
        closesocket(serverSocket);
        WSACleanup();
        return 0;
    }

    // Listen for incoming connections
    if (listen(serverSocket, 1) == SOCKET_ERROR) {
        std::cout << "listen(): Error listening on socket: " << WSAGetLastError() << std::endl;
    }

    SOCKET acceptSocket;
    acceptSocket = accept(serverSocket, nullptr, nullptr);

    // Check for successful connection
    if (acceptSocket == INVALID_SOCKET) {
        std::cout << "accept failed: " << WSAGetLastError() << std::endl;
        WSACleanup();
        return -1;
    }
    else
    {
        printf("Player 2 succesfully connected\n");
    }
    
    srand((unsigned) time(NULL));
    gameState gs;
    gs.correctNumber = 1 + (rand() % 127);

    while(true)
    {
        // Send a message to the client
        printClue(gs);
        gs.lastGuess = fixedInput();
        gs.lastGuessMore = isNumberMore(gs);
        gs.hasNumberBeenGuessed = isNumberGuessed(gs);
        printClue(gs);

        char buffer[4];
        buffer[0] = (char)gs.correctNumber;
        buffer[1] = (char)gs.lastGuess;
        buffer[2] = (char)gs.lastGuessMore;
        buffer[3] = (char)gs.hasNumberBeenGuessed;

        int sbyteCount = send(acceptSocket, buffer, 4, 0);
        if (sbyteCount == SOCKET_ERROR) {
            std::cout << "Server send error: " << WSAGetLastError() << std::endl;
            return -1;
        }

        if(gs.hasNumberBeenGuessed == true)
        {
            printf("YOU GUESSED THE NUMBER, IT WAS: %d", gs.correctNumber);
            return 0;
        }

        // Receive data from the client
        char receiveBuffer[4];
        int rbyteCount = recv(acceptSocket, receiveBuffer, 4, 0);
        if (rbyteCount < 0) {
            std::cout << "Server recv error: " << WSAGetLastError() << std::endl;
            return 0;
        }

        gs.correctNumber = (uint8_t)receiveBuffer[0];
        gs.lastGuess = (uint8_t)receiveBuffer[1];
        gs.lastGuessMore = (bool)receiveBuffer[2];
        gs.hasNumberBeenGuessed = (bool)receiveBuffer[3];

        if(gs.hasNumberBeenGuessed == true)
        {
            printf("PLAYER 2 GUESSED THE NUMBER, IT WAS: %d", gs.correctNumber);
            return 0;
        }
    }
    
    return 0;
}