#ifndef SOCKETSERVER_H
#define SOCKETSERVER_H

#include <cstring>
#include <iostream>
#include <netinet/in.h>
#include <sys/socket.h>
#include <unistd.h>

using namespace std;


class ISocket {
public:

    virtual int socketCreate() = 0;
    virtual int socketBind(int) = 0;
    virtual int socketListen(int) = 0;
    virtual int socketAccept(int, sockaddr_in) = 0;
    virtual int socketSend(int, const char*) = 0;
    virtual int socketReceive(int, char*) = 0;

    sockaddr serverAddress;
    int PORT;
    int MAX_CONNECTIONS;
};

class SocketServer : public ISocket{
public:
    sockaddr_in serverAddress;
    int PORT;
    int MAX_CONNECTIONS;

    SocketServer(int _PORT, int _MAX_CONNECTIONS)
        : PORT(_PORT), MAX_CONNECTIONS(_MAX_CONNECTIONS){

        serverAddress.sin_family = AF_INET; // specifies IPV4
        serverAddress.sin_port = htons(PORT); // delcare port num and host to network byte order
        serverAddress.sin_addr.s_addr = INADDR_ANY; // listens to all avaliable IP
    }


    int socketCreate() override {
        return socket(AF_INET, SOCK_STREAM,0);
    }

    int socketBind(int serverSocket) override {
        return bind(serverSocket, (struct sockaddr*)&serverAddress, sizeof(serverAddress));
    }

    int socketListen(int serverSocket) override {
        return listen(serverSocket,MAX_CONNECTIONS);
    }

    int socketAccept(int serverSocket, sockaddr_in client_address) override {
        socklen_t client_add_len = sizeof(client_address);
        return accept(serverSocket, (struct sockaddr*)&client_address, &client_add_len);
    }

    int socketSend(int clientSocket, const char* message) override {
        return send(clientSocket, message, strlen(message), 0);
    }

    int socketReceive(int clientSocket, char* message) override {
        return recv(clientSocket, message, sizeof(message),0);
    }

};

#endif