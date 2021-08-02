#include <iostream>
#include <jsonrpccpp/server.h>
#include <jsonrpccpp/server/connectors/httpserver.h>
#include <stdio.h>
#include <string>

using namespace jsonrpc;
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

int main()
{
    HttpServer server(8383);
    SimpleServer serv(server);
    if(serv.StartListening())
    {
        std::cout << "Server started successfully" << std::endl;
        getchar();
        serv.StopListening();
    }
    return 0;
}