#include "SimpleServer.h"
#include "SocketServer.h"


#include <iostream>
#include <fstream>
#include <sstream>
#include <thread>


using namespace std;


SimpleServer::SimpleServer(ISocket& _server)
    : server(_server){}


int SimpleServer::SetupServer(){

    serverSock = server.socketCreate();
    if(serverSock < 0){
        perror("Server socket creation failed!");
        return 1;
    }

    if(server.socketBind(serverSock) < 0){
        perror("Server binding to the socket failed!");
        return 1;
    }
    printf("Server bound to socket on port %d \n", server.PORT);
    return 0;
}

int SimpleServer::StartServer(){

    if(server.socketListen(serverSock) < 0){
        perror("Listen Failed, Server not listening!");
        return 1;
    }

    printf("Server listening over port %d...\n", server.PORT);


    while(true){
        // loop to handle connections
        cout << "Waiting for incoming connections... \n";

        sockaddr_in clientAddr{};
        int clientSock = server.socketAccept(serverSock, clientAddr);
        if(clientSock < 0 ){
            perror("Server failed to accept client connection!");
            continue;
        }

        cout << "Connection accepted! Processing request! Creating new thread... \n";

        //ProcessClient(clientSock);
        thread clientThread(&SimpleServer::ProcessClient, this, clientSock);
        clientThread.detach();
    }

    return 0;
};


int SimpleServer::SendData(int clientSock, const char* data){
    if(server.socketSend(clientSock, data) < 0){
        perror("Failed to send response");
        return 1;
    }
    cout << "Sent response successfully";
    return 0;
}


void SimpleServer::ProcessClient(int clientSock){
// calls helper functions to process the clients request
 
}


