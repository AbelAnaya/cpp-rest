/***
 * Device entity manager.
 *
 * This class is responsible for handling devices request in the REST API.
 *
 ****/

#include "Device_Manager.hpp"
#include "SQL_Client.hpp"


crow::response DeviceManager::get_device(const crow::request& req)
{
    SQLClient& sql_client = SQLClient::getInstance();
    return sql_client.getAllDevices(req);
}


crow::response DeviceManager::post_device(const crow::request& req)
{
    crow::json::rvalue json_body = crow::json::load(req.body);

    if ((!json_body) || (!json_body.has("serialNumber")) || (!json_body.has("name")) || (!json_body.has("type")))
        return crow::response(crow::status::BAD_REQUEST);

    SQLClient& sql_client = SQLClient::getInstance();
    const crow::status rc = sql_client.postDevice(json_body);

    return crow::response(rc);
}


crow::response DeviceManager::get_device_by_sn(const int serial_number)
{
    SQLClient& sql_client = SQLClient::getInstance();
    return sql_client.getDevice(serial_number);
}


crow::response DeviceManager::delete_device_by_sn(const int serial_number)
{
    SQLClient& sql_client = SQLClient::getInstance();
    const crow::status rc = sql_client.deleteDevice(std::to_string(serial_number));

    return crow::response(rc);
}


crow::response DeviceManager::put_device_by_sn(const crow::request& req, const int serial_number)
{
    crow::json::rvalue json_body = crow::json::load(req.body);

    if ((!json_body) || (!json_body.has("locationId")))
        return crow::response(crow::status::BAD_REQUEST);

    SQLClient& sql_client = SQLClient::getInstance();

    const crow::status rc = sql_client.putDevice(serial_number, json_body);

    return crow::response(rc);
}
