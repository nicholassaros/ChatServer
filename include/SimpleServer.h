#ifndef SIMPLESERVER_H
#define SIMPLESERVER_H

#include "SocketServer.h"

#include <map>
#include <mutex>
#include <vector>


struct ClientData {
    string userName;
    int roomID;
    int socket;
};

class SimpleServer {
private:
    ISocket& server;
    map<int,vector<ClientData>> rooms;
    map<int,vector<ClientData>> connected;
    mutex clientMetadataMtx;
    int serverSock;
    int MAX_CONNECTIONS;

public:
    SimpleServer(ISocket& server);

    int SetupServer();

    int StartServer();

    int SendData(int, const char*);

    int HandleInitialConnection(int);

    void ProcessClient(int); 

    int ParseClientHandshake(int, char*, ClientData&);

};


#endif