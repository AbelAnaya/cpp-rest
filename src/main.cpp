#include "crow.h"
#include <string>

int main() {
    crow::SimpleApp app;

    CROW_ROUTE(app, "/devices/<int>")
        ([](int serial_number) {
            crow::json::wvalue x({{"message", "Hello World from REST API, device SN: " + std::to_string(serial_number)}});
            return x;
        });

    app.multithreaded().run();

    return 0;
}
