#include <iostream>
#include <WinSock2.h>
#include "Packet.h"

#pragma comment(lib, "ws2_32")


int main()
{
    WSAData WsaData;
    WSAStartup(MAKEWORD(2, 2), &WsaData);
    SOCKET ListenSocket = {};
    SOCKET ClientSocket = {};
    try
    {
        ListenSocket = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);
        if (ListenSocket == INVALID_SOCKET)
        {
            throw std::exception("Listen socket is invalid.");
        }

        SOCKADDR_IN ListenAddress = {};
        ListenAddress.sin_family = AF_INET;
        ListenAddress.sin_addr.s_addr = ADDR_ANY;
        ListenAddress.sin_port = htons(18080);
        int Result = bind(ListenSocket, reinterpret_cast<SOCKADDR*>(&ListenAddress), sizeof(ListenAddress));
        if (Result < 0)
        {
            throw std::exception("Bind fail.");
        }

        Result = listen(ListenSocket, 2);
        if (Result < 0)
        {
            throw std::exception("Listen fail.");
        }

        SOCKADDR_IN ClientAddress = {};
        int ClientAddressLength = sizeof(ClientAddress);
        ClientSocket = accept(ListenSocket, reinterpret_cast<SOCKADDR*>(&ClientAddress),
                              &ClientAddressLength);
        if (ClientSocket == INVALID_SOCKET)
        {
            throw std::exception("Client socket is invalid.");
        }

        // Send Spawn Location
        HEADER Header = {};
        Header.Length = sizeof(LOCATION);
        Header.Type = Type::Location;
        Result = send(ClientSocket, reinterpret_cast<char*>(&Header), sizeof(Header), 0);
        if (Result < 0)
        {
            throw std::exception("Send fail.");
        }

        LOCATION Location = {};
        Location.X = 0;
        Location.Y = 0;
        Result = send(ClientSocket, reinterpret_cast<char*>(&Location), sizeof(Location), 0);
        if (Result < 0)
        {
            throw std::exception("Send fail.");
        }

        while (true)
        {
            // Receive Movement
            int ReceiveByte = recv(ClientSocket, reinterpret_cast<char*>(&Header), sizeof(Header), MSG_WAITALL);
            if (ReceiveByte < 0)
            {
                throw std::exception("Receive fail.");
            }

            MOVE Move = {};
            ReceiveByte = recv(ClientSocket, reinterpret_cast<char*>(&Move), Header.Length, MSG_WAITALL);
            if (ReceiveByte < 0)
            {
                throw std::exception("Receive fail.");
            }

            Location.X += Move.X;
            Location.Y += Move.Y;
            std::cout << "X : " << Location.X << std::endl << "Y : " << Location.Y << std::endl;

            // Send Location
            Header.Length = sizeof(LOCATION);
            Header.Type = Type::Location;
            Result = send(ClientSocket, reinterpret_cast<char*>(&Header), sizeof(Header), 0);
            if (Result < 0)
            {
                throw std::exception("Send fail.");
            }

            Result = send(ClientSocket, reinterpret_cast<char*>(&Location), sizeof(Location), 0);
            if (Result < 0)
            {
                throw std::exception("Send fail.");
            }
        }
    }
    catch (std::exception& Exception)
    {
        std::cout << "[" << GetLastError() << "] " << Exception.what();
        closesocket(ClientSocket);
        closesocket(ListenSocket);
        WSACleanup();
        return -1;
    }
    closesocket(ClientSocket);
    closesocket(ListenSocket);

    WSACleanup();
    return 0;
}
