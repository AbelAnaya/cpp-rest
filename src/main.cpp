#include <cpprest/http_listener.h>
#include <cpprest/json.h>

using namespace web;
using namespace web::http;
using namespace web::http::experimental::listener;

int main() {
    http_listener listener("http://localhost:8080/restdemo");

    listener.support(methods::GET, [](http_request request) {
        json::value response;
        response["response"] = json::value::string("Hello from the REST API!");

        request.reply(status_codes::OK, response);
    });

    try {
        listener
            .open()
            .then([&listener]() { std::cout << "Starting to listen at: " << listener.uri().to_string() << std::endl; })
            .wait();

        while (true);
    }
    catch (std::exception const & e) {
        std::cout << e.what() << std::endl;
    }

    return 0;
}
