#include <iostream>
#include <jsonrpccpp/client.h>
#include <jsonrpccpp/client/connectors/httpclient.h>

using namespace jsonrpc;
int main()
{
    HttpClient client ("http://localhost:8383");
    Client cli(client);

    Json::Value params;
    params["name"] = "Bartosz Nowak";

    std::cout << cli.CallMethod("sayHello", params) << std::endl;
    cli.CallNotification("notifyServer", params);
}