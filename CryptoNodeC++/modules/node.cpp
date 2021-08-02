#include <jsonrpccpp/server/connectors/httpserver.h>
#include <jsonrpccpp/server.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <iostream>
#include <string.h>
#include <unistd.h>
#include <cassert>
#include <thread>
#include <vector>
#include <mutex>

#define BITCOINCORE_BROADCAST_LISTENER_PORT 31415               // pi = 3.1415
#define JSON_RPC_SERVER_PORT 27182                              // e = 2.7182
#define IN_SOCK_BUFFER_SIZE 1024
using namespace jsonrpc;

std::mutex mtx;

class SimpleServer : public AbstractServer<SimpleServer>
{
    public:
    SimpleServer(HttpServer &server) : AbstractServer<SimpleServer>(server)
    {
        this->bindAndAddMethod(Procedure(
            "sayHello", PARAMS_BY_NAME, 
            JSON_STRING, "name", 
            JSON_STRING, NULL),
            &SimpleServer::sayHello);

        this->bindAndAddNotification(Procedure(
            "notifyServer", PARAMS_BY_NAME, NULL),
            &SimpleServer::notifyServer);
    }
    // method
    void sayHello(const Json::Value &request, Json::Value &respnse)
    {
        respnse = "Hello from server: " + request["name"].asString();
    }
    // notification
    void notifyServer(const Json::Value &request)
    {
        (void)request;
        std::cout << "server received some Notification from " << request["name"].asString() << std::endl;
    }
};

class BitcoinNode
{
    public:
    int socket_server;        
    HttpServer server;                                      // socket server file descriptor
    SimpleServer jsonrpc_server;                            // jsonrpc server pointer
    std::vector<std::thread> th_vec;
    
    public:
    BitcoinNode()
    :
    server(JSON_RPC_SERVER_PORT),
    jsonrpc_server(server)
    {
        socket_server = socket(AF_INET, SOCK_STREAM, 0);
        // HttpServer server(JSON_RPC_SERVER_PORT);
        // jsonrpc_server = SimpleServer(server);

        th_vec.push_back(std::thread(BitcoinCoreListener, this));
        // JsonRpcSever();
    }

    static void BitcoinCoreListener(BitcoinNode* node)
    {
        int in_sock, val_read;
        int opt = 1;
        sockaddr_in address;
        // setup server file descriptor
        assert(!setsockopt(node->socket_server, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)));
        // address settings
        address.sin_family = AF_INET;
        address.sin_addr.s_addr = INADDR_ANY;
        address.sin_port = htons(BITCOINCORE_BROADCAST_LISTENER_PORT);
        socklen_t addrlen = sizeof(address);
        // binding and listening
        assert(!bind(node->socket_server, (sockaddr*)&address, addrlen));
        assert(!listen(node->socket_server, 64));
        mtx.lock();
        std::cout << "[STATUS] BitcoinCoreBroadcastListener up and running" << std::endl;
        mtx.unlock();

        char buffer[IN_SOCK_BUFFER_SIZE];
        while(true)
        {
            memset(buffer, 0, IN_SOCK_BUFFER_SIZE);
            if((in_sock = accept(node->socket_server, (sockaddr*)&address, &addrlen)) == -1)
                return;
            val_read = read(in_sock, buffer, IN_SOCK_BUFFER_SIZE-1);
            mtx.lock();
            std::cout << buffer << std::endl;
            mtx.unlock();
        }
    }

    void JsonRpcSever()
    {
        assert(jsonrpc_server.StartListening());
        std::cout << "[STATUS] JsonRpcSever up and running" << std::endl;
    }

    ~BitcoinNode()
    {
        std::cout << "[STATUS] BitcoinCoreBroadcastListener shutdown" << std::endl;
        shutdown(socket_server, SHUT_RD);
        close(socket_server);
        for(std::thread& th : th_vec)
            th.join();
        jsonrpc_server.StopListening();
        // delete jsonrpc_server;
        
    }
};

int main()
{   
    BitcoinNode bitcoinnode;
    std::cout << "main" << std::endl;
    // bitcoinnode.jsonrpc_server.StartListening();
    // std::cout << "[STATUS] JsonRpcSever up and running" << std::endl;
    // getchar();
    // bitcoinnode.jsonrpc_server.StopListening();
    // std::cout << "[STATUS] JsonRpcSever shutdown" << std::endl;
}