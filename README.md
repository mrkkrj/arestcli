# arestcli

arestcli (i.e. Asio REST Client) is a C++ REST client using (non-boost) Asio library underneath.

This is a port of Microsoft's REST-SDK (aka Casablanca) using Asio library instead of boost::Asio!

It was created because BEAST is quite unwieldy (and because it needs th whole of boost!!!). On Windows the usage of Asio is also forced.


## WIP: 
 - Only some tests on Windows done 
 - Websockets, compression and SLL disabled and not yet tested 
 - Only a VisualStudio 2022 solution provided!!! Only working for Debug build, to boot. :-(

## Plans: 
 - Currently a proprietary continuations library by Microsoft is used. I will try to remove this and use C++20 coroutines support in Asio instead.
 - The std::wstring is used in the API at the moment. This is irritating, will try to switch to narrow strings (it's UTF8 everywhere, innnit?)

## Usage:

```cpp
#include <http_client.h>
#include <cpprest/json.h>

using namespace web;
using namespace web::http;
using namespace web::json;

int main()
{
    http::client::http_client clientRestTest(U("http://httpbin.org"));
    web::json::value client_resp;

    try
    {
        auto response = clientRestTest.request(methods::GET, U("/anything")).get();
        
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
        std::cout << "exc=" << exc.what() << std::endl;
        return -1;
    }
    catch (web::json::json_exception& exc)
    {
        std::cout << "exc=" << exc.what() << std::endl;
        return -1;
    }
    catch (...)
    {
        std::cout << "unknown exc!!!" << std::endl;
        return -1;
    }

    // show:
    std::wstring respStrg = client_resp.serialize();
    std::string s2(respStrg.begin(), respStrg.end());
    std::cout << " -- response:" << s2 << "\n\n";
}
```

## Note:
 - the example VisualStudio project expects Asio (just Asio, not boost::Asio!!!) to be installed in ../asio-1.28.0
