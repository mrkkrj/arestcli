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
    printf("Websockets not yet working!!!!\n\n");
    return;

    // basic
    websockets::client::websocket_client_config config;
    //config ---> OPEN TODO:: enable develper traces from here!
    websockets::client::websocket_client wsClient;

    wsClient.connect(U("ws://echo.websocket.org")).get();
    wsClient.close().get();

    printf("Websockets test (basic) OK!\n\n");

    // SSL
    websockets::client::websocket_client_config config_ssl;
    config_ssl.set_validate_certificates(false); // just use the tunnel!

    websockets::client::websocket_client wssClient(config_ssl);

    wssClient.connect(U("wss://socketsbay.com/wss/v2/1/demo/")) // wss://echo.websocket.org ???
#if 0
        .then(
            [=]()
            {
                return wsClient.send("TEST...")
            })
        .then(
            [=](websocket_incoming_message response)
            {
                printf("Response status code: %u.\n", response.status_code());
                return response.body();
            })
                    .then(
                        [=](size_t n)
                        {
                            printf("Received %zd bytes.\n", n);

                            return fileBuffer->close();
                        })
                .wait();

            //if (response.status_code() == status_codes::OK)
            //{
            //    ws_resp = response.extract_json().get();
            //}
            //else
            //{
            //    ws_resp = json::value();
            //}
#else
        .get();
#endif

    wssClient.close().get();

    printf("Websockets test (SSL) OK!\n\n");
}
