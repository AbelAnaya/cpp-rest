/***
 * 
 ****/

#include "Rest_Server.hpp"
#include "Device_Manager.hpp"
#include "crow.h"


/*********************
 * Type definition   *
 ********************/

typedef std::function<crow::response(const crow::request&)> ResponseHandler_ft;
typedef std::function<crow::response(const int)> ResponseHandlerById_ft;


/*************************
 * Function Definition   *
 *************************/

RestServer::RestServer(std::string ip, std::uint16_t port)
{
    this->_port = port;
    this->_ipAddress = ip;
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


/***************
 * Entry Point *
 ***************/

int main()
{
    DeviceManager device_manager;
    RestServer server("127.0.0.1", 8080);
    server
    .add_route("/devices", crow::HTTPMethod::GET, (ResponseHandler_ft) DeviceManager::get_device)
    .add_route("/devices", crow::HTTPMethod::POST, (ResponseHandler_ft) DeviceManager::post_device)
    .add_route("/devices/<int>", crow::HTTPMethod::GET, (ResponseHandlerById_ft) DeviceManager::get_device_by_sn)
    .add_route("/devices/<int>", crow::HTTPMethod::DELETE, (ResponseHandlerById_ft) DeviceManager::get_device_by_sn)
    .add_route("/devices/<int>/location", crow::HTTPMethod::PUT, (ResponseHandlerById_ft) DeviceManager::get_device_by_sn)
    .start();

    return 0;
}
