//
// basic websockets example for arestcli's REST client
//

#include <ws_client.h>
#include <cpprest/json.h>

using namespace web;
using namespace web::websockets;
using namespace web::websockets::client;


void websockets_test()
{
    websocket_outgoing_message msg;
    websocket_incoming_message resp;

    // 1. localhost + no SSL
    //  --> start test/ws_echo_server.py
    printf("Websockets test - looking for local ws_echo_server.py....\n");

    websockets::client::websocket_client_config config;
    //config.set_enable_ws_debug_trace(true);

    websockets::client::websocket_client wsClient(config);
    wsClient.connect(U("ws://localhost:8080")).get(); 

    msg.set_utf8_message("Hello from client!");

    wsClient.send(msg).get();
    resp = wsClient.receive().get();

    if (resp.extract_string().get() != "Hello from client!")
    {
        printf("Websockets test (basic/local) ERR!\n\n");
    }
    else
    {
        printf("Websockets test (basic/local) OK!\n\n");
    }
    
    wsClient.close().get();
    
    // 2. localhost with SSL
    //  --> start test/wss_echo_server.py
    printf("Websockets test - looking for local wss_echo_server.py....\n");

    websockets::client::websocket_client_config config_ssl;
    config_ssl.set_validate_certificates(false); // just use the tunnel!
    //config_ssl.set_enable_ws_debug_trace(true);
    //config_ssl.set_open_handshake_timeout(12000);

    websockets::client::websocket_client wssClient(config_ssl);
    wssClient.connect(U("wss://localhost:8081")).get();

    msg.set_utf8_message("Hello from SSL client!");

    wssClient.send(msg).get();
    resp = wssClient.receive().get();

    if (resp.extract_string().get() != "Hello from SSL client!")
    {
        printf("Websockets test (SSL/local) ERR!\n\n");
    }
    else
    {
        printf("Websockets test (SSL/local) OK!\n\n");
    }

    wssClient.close().get();

    // 3. SSL + web
    //  -- OPEN TODO::: cannot reuse wssClient (runs on Assert() in ws_client_wspp.cpp)!!!! :-O 
    websockets::client::websocket_client wssClientWeb(config_ssl); 
    wssClientWeb.connect(U("wss://socketsbay.com/wss/v2/1/demo/")).get();

    msg.set_utf8_message("Hello from client!");
    wssClientWeb.send(msg).get();

    resp = wssClientWeb.receive().get();
    if (resp.extract_string().get() != "Hello from client!")
    {
        printf("Websockets test (SSL/web) ERR!\n\n");
    }

    wssClientWeb.close().get();

    printf("Websockets test (SSL/web) OK!\n\n");
}
