/***
 * 
 ****/

#include "crow.h"
#include "Device_Manager.hpp"
#include "SQL_Client.hpp"


crow::response DeviceManager::get_device(const crow::request& req)
{
    std::ostringstream os;

    SQLClient& sql_client = SQLClient::getInstance();

    sql_client.selectQuery();

    os << "Params: " << req.url_params << "\n\n";

    if (req.url_params.get("type") != nullptr)
    {
        os << "The value of 'type' is " << req.url_params.get("type") << '\n';
    }

    return crow::response{os.str()};
}


crow::response DeviceManager::post_device(const crow::request& req)
{
    auto json_body = crow::json::load(req.body);

    if (!json_body)
        return crow::response(crow::status::BAD_REQUEST);

    return crow::response(crow::status::OK, json_body["name"].s());
}


crow::response DeviceManager::get_device_by_sn(const int serial_number)
{
    crow::json::wvalue body_response({{"message", "Get device SN: " + std::to_string(serial_number)}});
    return crow::response(crow::status::OK, body_response);
}


crow::response DeviceManager::delete_device_by_sn(const int serial_number)
{
    crow::json::wvalue body_response({{"message", "Delete device SN: " + std::to_string(serial_number)}});
    return crow::response(crow::status::OK, body_response);
}


crow::response DeviceManager::put_device_by_sn(const int serial_number)
{
    crow::json::wvalue body_response({{"message", "Put device SN: " + std::to_string(serial_number)}});
    return crow::response(crow::status::OK, body_response);
}
