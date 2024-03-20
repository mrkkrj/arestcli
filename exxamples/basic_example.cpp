//
//  basic usage example for arestcli's REST client
//

#include <http_client.h>
#include <cpprest/json.h>


using namespace web;
using namespace web::http;
using namespace web::json;


int main()
{
    http::client::http_client clientRestTest(U("http://httpbin.org"));
    web::json::value client_resp;

    // 1. use the continuations directly
    try
    {
        auto resp =
            clientRestTest.request(methods::GET, uri_builder(U("/ip")).to_string())
            .then(
                [](http_response response) 
                {
                    std::cout << "response status code:" << response.status_code() << std::endl;

                    if (response.status_code() == status_codes::OK)
                    {
                        return response.extract_json();
                    }
                    else
                    {
                        return pplx::task_from_result(web::json::value());
                    }
                })
            .then(
                [&client_resp](pplx::task<web::json::value> previousTask)
                {
                    client_resp = previousTask.get();
                })
            .wait();
    }
    catch (web::http::http_exception& exc)
    {
        std::cout << "EXIT -- HTTP exception thrown!!!" << std::endl;
        std::cout << "  txt=" << exc.what() << std::endl;
        return -1;
    }
    catch (web::json::json_exception& exc)
    {
        std::cout << "EXIT -- JSON exception thrown!!!" << std::endl;
        std::cout << "  txt=" << exc.what() << std::endl;
        return -1;
    }
    catch (...)
    {
        std::cout << "EXIT -- exception thrown!!!" << std::endl;
        return -1;
    }

    // show:
    std::wstring respStrg = client_resp.serialize();
    std::string s1(respStrg.begin(), respStrg.end());
    std::cout << "--- response www.httpbin.org/ip:" << s1 << "\n\n";


    // 2. just use get()!
    try
    {
        auto response = clientRestTest.request(methods::GET, uri_builder(U("/anything")).to_string()).get();
        
        std::cout << "response status code:" << response.status_code() << std::endl;

        if (response.status_code() == status_codes::OK)
        {
            client_resp = response.extract_json().get();
        }
        else
        {
            client_resp = web::json::value();
        }
    }
    catch (web::http::http_exception& exc)
    {
        std::cout << "EXIT -- HTTP exception thrown!!!" << std::endl;
        std::cout << "  txt=" << exc.what() << std::endl;
        return -1;
    }
    catch (web::json::json_exception& exc)
    {
        std::cout << "EXIT -- JSON exception thrown!!!" << std::endl;
        std::cout << "  txt=" << exc.what() << std::endl;
        return -1;
    }
    catch (...)
    {
        std::cout << "EXIT -- exception thrown!!!" << std::endl;
        return -1;
    }

    // show:
    respStrg = client_resp.serialize();
    std::string s2(respStrg.begin(), respStrg.end());
    std::cout << " -- response www.httpbin.org/anything:" << s2 << "\n\n";
}
