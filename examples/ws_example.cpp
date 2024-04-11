//
// basic websockets example for arestcli's REST client
//

#include <ws_client.h>
#include <cpprest/json.h>

#include <cstdlib> // OPEN TODO::: std::system...

using namespace web;
using namespace web::websockets;
using namespace web::websockets::client;


void ws_echo_test(const web::uri& uri, const char* message, bool wspp_debug_trace = false);


void websockets_test()
{
    // 1. localhost + no SSL
    //  --> start test/ws_echo_server.py
    printf("Websockets test - looking for local ws_echo_server.py....\n");

    ws_echo_test(U("ws://localhost:8080"), "Hello from client!");
    printf("Websockets test (basic/local) OK!\n\n");
    
    // 2. localhost with SSL
    //  --> start test/wss_echo_server.py
    printf("Websockets test - looking for local wss_echo_server.py....\n");

    ws_echo_test(U("wss://localhost:8081"), "Hello from SSL client!");
    printf("Websockets test (basic/local) OK!\n\n");

    // 3. SSL + web
    //  --> socketsbay demo working?
    printf("Websockets test - to connect socketsbay.com's WS demo....\n");

    ws_echo_test(U("wss://socketsbay.com/wss/v2/1/demo/"), "Hello from remote client!");
    printf("Websockets test (SSL/web) OK!\n\n");

    
    // OPEN TODO::: 
    //  -- more tests

}


void ws_echo_test(const web::uri& uri, const char* message_txt, bool wspp_debug_trace)
{
    websocket_outgoing_message msg;
    websocket_incoming_message resp;

    websockets::client::websocket_client_config config;
    config.set_validate_certificates(false);    // SSL? -> just use the tunnel
    config.set_enable_ws_debug_trace(wspp_debug_trace);
    //config_ssl.set_open_handshake_timeout(12000);

    websockets::client::websocket_client ws_client(config);
    
    ws_client.connect(uri).get();
        
    msg.set_utf8_message(message_txt);
    ws_client.send(msg).get();
    
    resp = ws_client.receive().get();
    bool echo_ok = resp.extract_string().get() == message_txt;    

    ws_client.close().get();

    if (!echo_ok)
        throw std::exception("wrong echo string received!!!");
}
