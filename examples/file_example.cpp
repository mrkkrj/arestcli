//
// basic file transfer example for arestcli's REST client
//

#include <http_client.h>
#include <cpprest/json.h>
#include <cpprest/astreambuf.h>
#include <cpprest/filestream.h>

using namespace utility;
using namespace web::http;
using namespace web::http::client;
using namespace concurrency::streams;


void fetch_to_file(const string_t& server_url, const string_t& server_resource, const string_t& file_path, int file_mode);


void filetransfer_test()
{
    // 1. simple                      
    fetch_to_file(U("http://httpbin.org"), U("/anything"), U("./rest_test.out"), std::ios::out);
    printf("File stream test (basic) OK!\n\n");

    // 2. overwrite file    
    // OPEN TODO::: overwriting not yet working! Bug in Casablanca as it seems...
#if 0       
    fetch_to_file(U("http://httpbin.org"), U("/anything"), U("./rest_test.out"), std::ios::trunc);
    printf("File stream test (overwrite) OK!\n\n");
#endif

    // 3. HTTPS
#if !defined(CPPREST_EXCLUDE_SSL) 
    fetch_to_file(U("https://httpbin.org"), U("/anything"), U("./rest_test.out"), std::ios::out);
    printf("File stream test (https) OK!\n\n");  
#endif

    // 4. chunked transfer encoding
    fetch_to_file(U("http://www.bing.com"), U("/search?q=mrkkrj"), U("./rest_test.out"), std::ios::out);
    printf("File stream test (chunked) OK!\n\n");


    // OPEN TODO:: long binary file...
    
    // ...
}


void fetch_to_file(
    const string_t& server_url, 
    const string_t& server_resource, 
    const string_t& file_path, 
    int file_mode)
{
    auto fileBuffer = std::make_shared<streambuf<uint8_t>>();

    file_buffer<uint8_t>::open(file_path, file_mode)
        .then(
            [=](streambuf<uint8_t> outFile) -> pplx::task<http_response>
            {
                http_client_config config;

                if (server_url.starts_with(U("https:")))
                {   
                    config.set_validate_certificates(false); // just use the tunnel!
                }

                http_client client(server_url, config);
                *fileBuffer = outFile;

                return client.request(methods::GET, server_resource);
            })
        .then(
            [=](http_response response) -> pplx::task<size_t>
            {
                printf("Response status code: %u.\n", response.status_code());
                return response.body().read_to_end(*fileBuffer);
            })
        .then(
            [=](size_t n)
            {
                printf("Received %d bytes.\n", n);

                return fileBuffer->close();
            })
        .wait();
}

