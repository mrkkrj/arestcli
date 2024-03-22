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


void filetransfer_test()
{
    // 1.
    auto fileBuffer = std::make_shared<streambuf<uint8_t>>();
    file_buffer<uint8_t>::open(U("./rest_test.out"), std::ios::out)
        .then(
            [=](streambuf<uint8_t> outFile) -> pplx::task<http_response>
            {
                *fileBuffer = outFile;
                http_client client(U("http://httpbin.org"));

                return client.request(methods::GET, U("/anything"));
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

                    
    printf("File stream test OK!\n");
    return;

    // OPEN TODO:: https
    // OPEN TODO:: overwrite file
    // OPEN TODO:: long file...
    // OPEN TODO::: bing --- not yet working!
#if 0
    // 2.
    const string_t searchTerm = U("mrkkrj");
    const string_t outputFileName = U("./rest_test.out");

    // Open a stream to the file to write the HTTP response body into.
    auto fileBuffer = std::make_shared<streambuf<uint8_t>>();
    file_buffer<uint8_t>::open(outputFileName, std::ios::out)
        .then(
            [=](streambuf<uint8_t> outFile) -> pplx::task<http_response>
            {
                *fileBuffer = outFile;
                http_client client(U("http://www.bing.com/"));

                return client.request(methods::GET, uri_builder(U("/search")).append_query(U("q"), searchTerm).to_string());
            })
        .then(
            [=](http_response response) -> pplx::task<size_t>
            {
                printf("Response status code %u returned.\n", response.status_code());

                return response.body().read_to_end(*fileBuffer);
            })
        .then(
            [=](size_t n)
            {
                printf("Received %d bytes.\n", n);

                return fileBuffer->close();
            })
        .wait();

     printf("File stream tests OK!\n");
#endif
}

