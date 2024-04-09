/***
 * REST Server manager.
 *
 * This class is reponsible for managing the REST Server, creating it and defining the endpoints.
 * It contains the entry point to the SW.
 *
 ****/

#include "Rest_Server.hpp"
#include "Device_Manager.hpp"
#include "Location_Manager.hpp"


/*********************
 * Type definition   *
 ********************/

typedef std::function<crow::response(const crow::request&)> ResponseHandler_ft;
typedef std::function<crow::response(const int)> ResponseHandlerById_ft;
typedef std::function<crow::response(const crow::request&, const int)> ResponseHandlerFull_ft;


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
    RestServer server("0.0.0.0", 8080);
    server
    .add_route("/devices", crow::HTTPMethod::GET, (ResponseHandler_ft) DeviceManager::get_device)
    .add_route("/devices", crow::HTTPMethod::POST, (ResponseHandler_ft) DeviceManager::post_device)
    .add_route("/devices/<int>", crow::HTTPMethod::GET, (ResponseHandlerById_ft) DeviceManager::get_device_by_sn)
    .add_route("/devices/<int>", crow::HTTPMethod::DELETE, (ResponseHandlerById_ft) DeviceManager::delete_device_by_sn)
    .add_route("/devices/<int>/location", crow::HTTPMethod::PUT, (ResponseHandlerFull_ft) DeviceManager::put_device_by_sn)
    .add_route("/locations", crow::HTTPMethod::GET, (ResponseHandler_ft) LocationManager::get_location)
    .add_route("/locations", crow::HTTPMethod::POST, (ResponseHandler_ft) LocationManager::post_location)
    .add_route("/locations/<int>", crow::HTTPMethod::GET, (ResponseHandlerById_ft) LocationManager::get_location_by_id)
    .add_route("/locations/<int>", crow::HTTPMethod::DELETE, (ResponseHandlerById_ft) LocationManager::delete_location_by_id)
    .start();

    return 0;
}
