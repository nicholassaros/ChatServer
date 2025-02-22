#ifndef SIMPLESERVER_H
#define SIMPLESERVER_H

#include "SocketServer.h"


class SimpleServer {
private:
    ISocket& server;
    int serverSock;
    int MAX_CONNECTIONS;

public:
    SimpleServer(ISocket& server);

    int SetupServer();

    int StartServer();

    int SendData(int, const char*);

    void ProcessClient(int); 

};


#endif