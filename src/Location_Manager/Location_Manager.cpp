/***
 *
 ****/

#include "Location_Manager.hpp"
#include "SQL_Client.hpp"


crow::response LocationManager::get_location(const crow::request& req)
{
    SQLClient& sql_client = SQLClient::getInstance();
    return sql_client.getAllLocations(req);
}


crow::response LocationManager::post_location(const crow::request& req)
{
    crow::json::rvalue json_body = crow::json::load(req.body);

    if ((!json_body) || (!json_body.has("locationId"))
        || (!json_body.has("locationName")) || (!json_body.has("locationType")))
    {
        return crow::response(crow::status::BAD_REQUEST);
    }

    SQLClient& sql_client = SQLClient::getInstance();
    const crow::status rc = sql_client.postLocation(json_body);

    return crow::response(rc);
}


crow::response LocationManager::get_location_by_id(const int locationId)
{
    SQLClient& sql_client = SQLClient::getInstance();
    return sql_client.getLocation(locationId);
}


crow::response LocationManager::delete_location_by_id(const int locationId)
{
    SQLClient& sql_client = SQLClient::getInstance();
    const crow::status rc = sql_client.deleteEntity("locations", "location_id", std::to_string(locationId));

    return crow::response(rc);
}
