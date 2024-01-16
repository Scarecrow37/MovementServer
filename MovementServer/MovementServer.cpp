#include <iostream>
#include <WinSock2.h>
#include "Packet.h"

#pragma comment(lib, "ws2_32")


int main()
{
    try
    {
        WSAData WsaData;
        WSAStartup(MAKEWORD(2, 2), &WsaData);

        const SOCKET ListenSocket = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);
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
        const SOCKET ClientSocket = accept(ListenSocket, reinterpret_cast<SOCKADDR*>(&ClientAddress), &ClientAddressLength);
        if (ClientSocket == INVALID_SOCKET)
        {
            throw std::exception("Client socket is invalid.");
        }

        // Send Spawn Location
        HEADER SpawnHeader = {};
        SpawnHeader.Length = sizeof(LOCATION);
        SpawnHeader.Type = Type::Location;
        Result = send(ClientSocket, reinterpret_cast<char*>(&SpawnHeader), sizeof(SpawnHeader), 0);
        if (Result <= 0)
        {
            throw std::exception("Send fail.");
        }
        
        LOCATION SpawnData = {};
        SpawnData.X = 0;
        SpawnData.Y = 0;
        Result = send(ClientSocket, reinterpret_cast<char*>(&SpawnData), sizeof(SpawnData), 0);
        if (Result <= 0)
        {
            throw std::exception("Send fail.");
        }

        // Receive Movement
    }
    catch (std::exception& Exception)
    {
        std::cout << "[" << GetLastError() << "] " << Exception.what();

        return -1;
    }
    return 0;
}
