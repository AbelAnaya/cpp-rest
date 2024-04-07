/***
 * 
 ****/

#include "Rest_Server.hpp"
#include "crow.h"

RestServer::RestServer(std::string ip, std::uint16_t port)
{
    this->_port = port;
    this->_ipAddress = ip;
}

crow::response get_device(const crow::request& req)
{
    std::ostringstream os;
    os << "Params: " << req.url_params << "\n\n";

    if (req.url_params.get("type") != nullptr)
    {
        os << "The value of 'type' is " << req.url_params.get("type") << '\n';
    }

    return crow::response{os.str()};
}

crow::response post_device(const crow::request& req)
{
    auto json_body = crow::json::load(req.body);

    if (!json_body)
        return crow::response(crow::status::BAD_REQUEST);

    return crow::response(crow::status::OK, json_body["name"].s());
}

crow::response get_device_by_sn(int serial_number)
{
    crow::json::wvalue body_response({{"message", "Hello World from REST API, device SN: " + std::to_string(serial_number)}});
    return crow::response(crow::status::OK, body_response);
}

template <typename Func>
RestServer& RestServer::add_route(std::string route, crow::HTTPMethod method, Func callback)
{
    this->app.route_dynamic(route.c_str())
             .methods(method)
             (callback);

    return *this;
}

RestServer& RestServer::start(void)
{
    this->app.bindaddr(this->_ipAddress)
             .port(this->_port)
             .multithreaded()
             .run();

    return *this;
}

int main()
{
    RestServer server("127.0.0.1", 8080);
    server
    .add_route("/devices", crow::HTTPMethod::GET, (std::function<crow::response(const crow::request&)>) get_device)
    .add_route("/devices", crow::HTTPMethod::POST, (std::function<crow::response(const crow::request&)>) post_device)
    .add_route("/devices/<int>", crow::HTTPMethod::GET, (std::function<crow::response(int)>) get_device_by_sn)
    .add_route("/devices/<int>", crow::HTTPMethod::DELETE, (std::function<crow::response(int)>) get_device_by_sn)
    .add_route("/devices/<int>/location", crow::HTTPMethod::PUT, (std::function<crow::response(int)>) get_device_by_sn)
    .start();

    return 0;
}
