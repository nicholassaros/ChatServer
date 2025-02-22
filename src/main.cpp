#include "../include/SimpleServer.h"
#include "../include/SocketServer.h"

using namespace std;

int main() {

    SocketServer socketServer = SocketServer(8001,20);
    SimpleServer server = SimpleServer(socketServer);

    server.SetupServer();
    server.StartServer();
};