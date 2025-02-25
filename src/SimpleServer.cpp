#include "SimpleServer.h"
#include "SocketServer.h"
#include "ThreadPool.h"


#include <iostream>
#include <fstream>
#include <sstream>
#include <thread>
#include <mutex>


using namespace std;


SimpleServer::SimpleServer(ISocket& _server, ThreadPool& pool)
    : server(_server), pool(pool){}


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


        pool.TaskQueuePush([&](int& clientSock){
            this->ProcessClient(clientSock);
        });
        
        /*
        //ProcessClient(clientSock);
        //thread clientThread(&SimpleServer::ProcessClient, this, clientSock);
        //clientThread.detach();
        */
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
    /*
      TWO CHOICES HERE
        If client socket exists in the client map we want to wait for message and send to all other clients in the room
            -> HandleConnectedClient
        If not we want to execute the handshake
            -> HandleClientHandshake
    */

}


int SimpleServer::ParseClientHandshake(int clientSock, char* raw_response, ClientData& clientData){
    // null terminating the response

    stringstream stream(raw_response);

    // Fill the struct;
    stream >> clientData.userName >> clientData.roomID;
    clientData.socket = clientSock;

    if (!(stream >> clientData.userName >> clientData.roomID)) { // Check if parsing both values succeeded
        perror("Error parsing the request");
    }  

    return 0;
}


int SimpleServer::HandleConnectedClient(int clientSock){
    // Wait for incoming message of client

    char* raw_message;
    int roomId;
    vector<ClientData> clientList;
    ClientData clientData;

    if(server.socketReceive(clientSock,raw_message) < 0){
        perror("Failed to recieve message from client");
    }


    // lock as we are reading from concurrent DataStructure 
    lock_guard<mutex> lock(clientMetadataMtx);
    {
        clientData = connected[clientSock];
        clientList = rooms[clientData.roomID];
    }

    for(const auto& recipient:clientList){
        // send out message to each member in the room
        if(server.socketSend(recipient.socket, raw_message) < 0){
            cout << "Message from " 
                << clientData.userName 
                << " failed to send to " 
                << recipient.userName 
                << endl; 
            perror("Message not sent!");
        }
    }

    return 0;
}


int SimpleServer::HandleClientHandshake(int clientSock){
    // We execute the initial handshake and as client for a room ID
    // if not executed within sertain time we should timeOut
    // we should map clientSock to an room ID map
    ClientData clientData;
    const char* handShake = "Enter connection @<username>:<roomID>";
    char* raw_response;
    int roomID;


    // Send data with request for room ID
    if(server.socketSend(clientSock, handShake) < 0){
        perror("Failed to send handShake to client");
        return 1;
    }

    // Parse out room ID
    if(server.socketReceive(clientSock, raw_response) < 0){
        perror("Failed to recieve handShake from client");
        return 1;
    }

    // null terminating the response
    raw_response[sizeof(raw_response)] = '\n';

    
    if(ParseClientHandshake(clientSock, raw_response, clientData) == 1){
        perror("Failed to parse client response");
        return 1;
    }


    // add to Room Map
    lock_guard<mutex> lock(clientMetadataMtx);
    {
        // add to map when we have aquired lock
        rooms[clientData.roomID].push_back(clientData);
        connected[clientData.socket] = clientData;
    }

    // have threads send out message that CLIENT blah blah joined room
    return 0;
}


