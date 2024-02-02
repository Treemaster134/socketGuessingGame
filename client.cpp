#include <iostream>
#include <winsock2.h>
#include "game.hpp"

int main() {
    // Initialize WSA variables
    WSADATA wsaData;
    int wsaerr;
    WORD wVersionRequested = MAKEWORD(2, 2);
    wsaerr = WSAStartup(wVersionRequested, &wsaData);

    // Check for initialization success
    if (wsaerr != 0) {
        std::cout << "The winsock dll not found" << std::endl;
        return 0;
    }

    // Create a socket
    SOCKET clientSocket;
    clientSocket = INVALID_SOCKET;
    clientSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

    // Check for socket creation success
    if (clientSocket == INVALID_SOCKET) {
        std::cout << "Error at socket(): " << WSAGetLastError() << std::endl;
        WSACleanup();
        return 0;
    }

    // Connect to the server
    sockaddr_in clientService;
    clientService.sin_family = AF_INET;
    clientService.sin_addr.s_addr = inet_addr("127.0.0.1");  // Replace with the server's IP address
    clientService.sin_port = htons(55555);  // Use the same port as the server

    // Use the connect function
    if (connect(clientSocket, reinterpret_cast<SOCKADDR*>(&clientService), sizeof(clientService)) == SOCKET_ERROR) {
        std::cout << "Client: connect() - Failed to connect: " << WSAGetLastError() << std::endl;
        WSACleanup();
        return 0;
    }
    else
    {
        printf("Succesfully connected to server\n");
    }


    gameState gs;

    while(true)
    {

        // Receive data from the client
        char receiveBuffer[4];
        int rbyteCount = recv(clientSocket, receiveBuffer, 4, 0);
        if (rbyteCount < 0) {
            std::cout << "Client recv error: " << WSAGetLastError() << std::endl;
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

        int sbyteCount = send(clientSocket, buffer, 4, 0);
        if (sbyteCount == SOCKET_ERROR) {
            std::cout << "Client send error: " << WSAGetLastError() << std::endl;
            return -1;
        }

        if(gs.hasNumberBeenGuessed == true)
        {
            printf("YOU GUESSED THE NUMBER, IT WAS: %d", gs.correctNumber);
            return 0;
        }
    }

    return 0;
}