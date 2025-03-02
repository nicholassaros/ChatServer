#include "../include/SimpleServer.h"
#include "../include/SocketServer.h"
#include "../include/ThreadPool.h"

using namespace std;

int main() {

    SocketServer socketServer = SocketServer(8001,20);
    ThreadPool pool = ThreadPool(5); // declare pool with 5 threads

    SimpleServer server = SimpleServer(socketServer, pool);

    server.SetupServer();
    server.StartServer();
};