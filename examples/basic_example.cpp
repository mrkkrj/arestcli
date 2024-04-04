//
//  basic usage example for arestcli's REST client
//

#include <http_client.h>
#include <cpprest/json.h>

#include "file_example.cpp"
#include "ws_example.cpp"

using namespace web;
using namespace web::http;
using namespace web::json;

void print_exception();
void print_status_code(const http::http_response& http_resp);
void print_result(const json::value& client_resp, const char* label);


int main()
{
    http::client::http_client clientRestTest(U("http://httpbin.org"));
    json::value client_resp;

    // 1. use the continuations directly
    try
    {
        auto resp =
            clientRestTest.request(methods::GET, uri_builder(U("/ip")).to_string())
            .then(
                [](http_response response) 
                {
                    print_status_code(response);

                    if (response.status_code() == status_codes::OK)
                    {
                        return response.extract_json();
                    }
                    else
                    {
                        return pplx::task_from_result(json::value());
                    }
                })
            .then(
                [&client_resp](pplx::task<json::value> previousTask)
                {
                    client_resp = previousTask.get();
                })
            .wait();
    }
    catch (...)
    {
        print_exception();
        return -1;
    }

    print_result(client_resp, "www.httpbin.org/ip");

    // 2. just use get() and block!
    try
    {
        auto response = clientRestTest.request(methods::GET, U("/anything")).get();
        print_status_code(response);

        if (response.status_code() == status_codes::OK)
        {
            client_resp = response.extract_json().get();
        }
        else
        {
            client_resp = json::value();
        }
    }
    catch (...)
    {
        print_exception();
        return -1;
    }

    print_result(client_resp, "www.httpbin.org/anything");

    // 3. HTTPS 
#if !defined(CPPREST_EXCLUDE_SSL) 

    http::client::http_client_config config_ssl;
    config_ssl.set_validate_certificates(false); // just use the tunnel!

    http::client::http_client clientSslTest(U("https://httpbin.org"), config_ssl);

    try
    {
        auto response = clientSslTest.request(methods::GET, U("headers")).get();
        print_status_code(response);

        if (response.status_code() == status_codes::OK)
        {
            client_resp = response.extract_json().get();
        }
        else
        {
            client_resp = json::value();
        }
    }
    catch (...)
    {
        print_exception();
        return -1;
    }

    print_result(client_resp, "www.httpbin.org/headers (SSL)");


    // 3.a HTTPS with validation    
    
    // ... OPEN TODO:::

#endif

    // 4. file transfer
    try
    {
        filetransfer_test();
    }
    catch (...)
    {
        print_exception();
        return -1;
    }

    // 5. websockets
#if !defined(CPPREST_EXCLUDE_WEBSOCKETS) 
    try
    {
        websockets_test();
    }
    catch (...)
    {
        print_exception();
        return -1;
    }
#endif
}


void print_exception()
{
    try
    {
        throw; // Lippincott pattern 
    }
    catch (web::http::http_exception& exc)
    {
        std::cout << "EXIT -- HTTP exception thrown!!!" << std::endl;
        std::cout << "  txt=" << exc.what() << std::endl;
    }
    catch (json::json_exception& exc)
    {
        std::cout << "EXIT -- JSON exception thrown!!!" << std::endl;
        std::cout << "  txt=" << exc.what() << std::endl;
    }
    catch (const std::system_error& exc)
    {
        std::cout << "EXIT -- system exception thrown!!!" << std::endl;
        std::cout << "  txt=" << exc.what() << std::endl;
    }
    catch (const std::exception& exc)
    {
        std::cout << "EXIT -- std::exception thrown!!!" << std::endl;
        std::cout << "  txt=" << exc.what() << std::endl;
    }
    catch (...)
    {
        std::cout << "EXIT -- exception thrown!!!" << std::endl;
    }
}


void print_result(const json::value& client_resp, const char* label)
{
    std::wstring resp = client_resp.serialize();
    std::string s(resp.begin(), resp.end());
    std::cout << "response " << label << " ---> " << s << "\n\n";
}


void print_status_code(const http::http_response& http_resp)
{
    std::cout << "response status code:" << http_resp.status_code() << std::endl;
}
