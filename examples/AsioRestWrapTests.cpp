// AsioRestWrap.cpp : Diese Datei enthält die Funktion "main". Hier beginnt und endet die Ausführung des Programms.
//

#include <http_client.h>
//#include <filestream.h>

#include <cpprest/json.h>
#include <pplx/pplxtasks.h>

using namespace utility;
using namespace web::http;
using namespace web::http::client;
//using namespace concurrency::stream;

using namespace web::json;


int main()
{
    // std::cout << "Hello World!\n";

#if 0
    if (argc != 3)
    {
        printf("Usage: BingRequest.exe search_term output_file\n");
        return -1;
    }
    
    const string_t searchTerm = args[1];
    const string_t outputFileName = args[2];

    // Open a stream to the file to write the HTTP response body into.
    auto fileBuffer = std::make_shared<streambuf<uint8_t>>();
    file_buffer<uint8_t>::open(outputFileName, std::ios::out).then([=](streambuf<uint8_t> outFile) -> pplx::task<http_response>
        {
            *fileBuffer = outFile;

    // Create an HTTP request.
    // Encode the URI query since it could contain special characters like spaces.
    http_client client(U("http://www.bing.com/"), client_config_for_proxy());

    return client.request(methods::GET, uri_builder(U("/search")).append_query(U("q"), searchTerm).to_string());
        })

        // Write the response body into the file buffer.
            .then([=](http_response response) -> pplx::task<size_t>
                {
                    printf("Response status code %u returned.\n", response.status_code());

        return response.body().read_to_end(*fileBuffer);
                })
            // Write the response body into the file buffer.
                    .then([=](http_response response) -> pplx::task<size_t>
                        {
                            printf("Response status code %u returned.\n", response.status_code());

                return response.body().read_to_end(*fileBuffer);
                        })

                    // Close the file buffer.
                            .then([=](size_t)
                                {
                                    return fileBuffer->close();
                                })

                            // Wait for the entire response body to be written into the file.
                                    .wait();
#else

    // Create an HTTP request.
    //  - Encode the URI query since it could contain special characters like spaces.
    const string_t searchTerm = U("mrkkrj");

    //http_client client(U("http://www.bing.com/")/*, client_config_for_proxy()*/);
    http_client clientRestTest(U("http://httpbin.org"));

    web::json::value client_resp;

    try
    {
        auto resp =
            //client.request(methods::GET, uri_builder(U("/search")).append_query(U("q"), searchTerm).to_string())
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
    std::wstring resp = client_resp.serialize();
    std::string s(resp.begin(), resp.end());
    std::cout << "--- response www.httpbin.org/ip:" << s << "\n\n";


    // 2.
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

    // TEST:::
    resp = client_resp.serialize();
    std::string s2(resp.begin(), resp.end());
    std::cout << " -- response www.httpbin.org/anything:" << s2 << "\n\n";

#endif

}

