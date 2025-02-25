#ifndef SIMPLESERVER_H
#define SIMPLESERVER_H

#include "SocketServer.h"
#include "ThreadPool.h"

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
    ThreadPool& pool;
    map<int,vector<ClientData>> rooms;
    map<int,ClientData> connected;
    mutex clientMetadataMtx;
    int serverSock;
    int MAX_CONNECTIONS;

public:
    SimpleServer(ISocket& server, ThreadPool& pool);

    int SetupServer();

    int StartServer();

    int SendData(int, const char*);

    int HandleClientHandshake(int);

    int HandleConnectedClient(int);

    void ProcessClient(int); 

    int ParseClientHandshake(int, char*, ClientData&);

};


#endif