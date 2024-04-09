/**
 * SQL Connection manager.
 *
 * This class manages SQL connection to mySQL database and handles REST API SQL requests.
 * Implemented as Singleton
 *
 **/

#include "SQL_Client.hpp"
#include <cppconn/exception.h>
#include <cppconn/statement.h>
#include <memory>


/********************************
 * Static functions definition  *
 ********************************/

static void _add_int_filter_to_query
(
    std::string& query,
    const crow::request& req,
    const std::string& db_member,
    const std::string& api_param,
    int& n_params
)
{
    char* param = req.url_params.get(api_param);

    if (param != nullptr) {
        query += db_member + "=" + param;
        if (n_params > 1) {
            query += " AND ";
            n_params--;
        }
    }
}


static void _add_string_filter_to_query
(
    std::string& query,
    const crow::request& req,
    const std::string& db_member,
    const std::string& api_param,
    int& n_params
)
{
    char* param = req.url_params.get(api_param);

    if (param != nullptr) {
        query += db_member + "=" + "\"" + param + "\"";
        if (n_params > 1) {
            query += " AND ";
            n_params--;
        }
    }
}


static void _add_date_filter_to_query
(
    std::string& query,
    const crow::request& req,
    int& n_params
)
{
    char* start_date = req.url_params.get("creationDateStart");
    char* end_date = req.url_params.get("creationDateEnd");

    if ((start_date != nullptr) && (end_date != nullptr)) {
        query += (std::string) "creation_date BETWEEN \"" + start_date + "\" AND \"" + end_date + "\"";
        if (n_params > 2) {
            query += " AND ";
            n_params -= 2;
        }
    }
}


static bool _check_id_exists
(
    sql::Connection* conn,
    int id,
    const std::string& table,
    const std::string& column
)
{
    bool rc;

    std::unique_ptr<sql::Statement> stmt(conn->createStatement());

    std::string check_query = "SELECT * FROM " + table + " WHERE " + column + " = " + std::to_string(id);
    std::unique_ptr<sql::ResultSet> res(stmt->executeQuery(check_query));

    res->rowsCount() > 0 ? rc = true : rc = false;

    return rc;
}


/*****************************************
 * Public API Class functions definition *
 *****************************************/

SQLClient::SQLClient()
{
    driver = get_driver_instance();
    con = driver->connect("localhost:3306", "root", "root");

    con->setSchema("rest_db");
}


SQLClient::~SQLClient()
{
    con->close();
    delete con;
}


SQLClient& SQLClient::getInstance()
{
    static SQLClient sql_client;
    return sql_client;
}


crow::status SQLClient::postDevice(const crow::json::rvalue& parameters)
{
    bool device_exists = _check_id_exists(con, parameters["serialNumber"].i(), "devices", "serial_number");

    std::string query = "";

    if (device_exists)
    {
        query += "UPDATE devices SET device_name = \"" + (std::string) parameters["name"].s()
            + "\", device_type = \"" + (std::string) parameters["type"].s()
            + "\" WHERE serial_number = " + std::to_string(parameters["serialNumber"].i());
    }
    else
    {
        query += "INSERT INTO devices VALUES("
            + std::to_string(parameters["serialNumber"].i()) + ", \""
            + (std::string) parameters["name"].s() + "\", \""
            + (std::string) parameters["type"].s() + "\", "
            + "NOW())";
    }

    std::unique_ptr<sql::Statement> stmt(con->createStatement());

    crow::status exec_status = crow::status::BAD_REQUEST;

    try
    {
        stmt->execute(query);

        exec_status = crow::status::OK;
    }
    catch (sql::SQLException &e)
    {
        std::cout << "# ERR: SQLException in " << __FILE__;
        std::cout << " (" << __FUNCTION__ << ") on line " << __LINE__ << std::endl;
        std::cout << "# ERR: " << e.what();
        std::cout << " (MySQL error code: " << e.getErrorCode();
        std::cout << ", SQLState: " << e.getSQLState() << " )" << std::endl;

        exec_status = crow::status::BAD_REQUEST;
    }

    if ((exec_status) && (parameters.has("locationId")))
    {
        // Add location to device reusing putDevice method
        exec_status = putDevice(parameters["serialNumber"].i(), parameters["locationId"].i());
    }

    return exec_status;
}


crow::status SQLClient::deleteDevice(const std::string& id)
{
    std::string query = "DELETE FROM devices WHERE serial_number = " + id;
    std::unique_ptr<sql::Statement> stmt(con->createStatement());

    stmt->execute(query);

    const int affectedRows = stmt->getUpdateCount();

    return affectedRows > 0 ? crow::status::OK : crow::status::NOT_FOUND;
}


crow::status SQLClient::putDevice(int serial_number, int locationId)
{
    const bool device_exists = _check_id_exists(con, serial_number, "devices", "serial_number");
    const bool location_exists = _check_id_exists(con, locationId, "locations", "location_id");

    if (device_exists && location_exists)
    {
        const bool device_location_exists = _check_id_exists(con, serial_number, "device_location", "device_sn");

        std::string query = "";

        if (device_location_exists)
        {
            query += "UPDATE device_location SET location_id = " + std::to_string(locationId)
                                            + " WHERE device_sn = " + std::to_string(serial_number);
        }
        else
        {
            query += "INSERT INTO device_location VALUES(" + std::to_string(serial_number)
                                                           + "," + std::to_string(locationId) + ")";
        }

        std::unique_ptr<sql::Statement> stmt(con->createStatement());
        stmt->execute(query);

        const int affectedRows = stmt->getUpdateCount();

        return affectedRows > 0 ? crow::status::OK : crow::status::BAD_REQUEST;
    }
    else
    {
        return crow::status::BAD_REQUEST;
    }

}


crow::status SQLClient::deleteLocation(int id)
{
    const bool location_has_device = _check_id_exists(con, id, "device_location", "location_id");

    std::string query_delete_location = "DELETE FROM locations WHERE location_id = " + std::to_string(id);

    std::unique_ptr<sql::Statement> stmt(con->createStatement());

    // Remove relation from any device in device_location
    if (location_has_device)
    {
        std::string query_delete_device_location = "DELETE FROM device_location WHERE location_id = "
                                                                    + std::to_string(id);
        stmt->execute(query_delete_device_location);
    }

    // Remove location
    stmt->execute(query_delete_location);

    const int affectedRows = stmt->getUpdateCount();

    return affectedRows > 0 ? crow::status::OK : crow::status::NOT_FOUND;
}


crow::response SQLClient::getDevice(const int id)
{
    std::string query = "SELECT serial_number, device_name, \
     device_type, creation_date, locations.location_id, \
     locations.location_name, locations.location_type FROM devices \
     LEFT JOIN device_location ON serial_number = device_sn \
     LEFT JOIN locations ON locations.location_id = device_location.location_id \
     WHERE serial_number = " + std::to_string(id);

    std::cout << "Query: " + query << std::endl;

    std::unique_ptr<sql::Statement> stmt(con->createStatement());
    std::unique_ptr<sql::ResultSet> res(stmt->executeQuery(query));

    if (res->rowsCount() > 0)
    {
        res->next();

        crow::json::wvalue body_response({
            {"serialNumber", res->getInt("serial_number")},
            {"name", (std::string) res->getString("device_name")},
            {"type", (std::string) res->getString("device_type")},
            {"creationDate", (std::string) res->getString("creation_date")},
            {"locationId", res->getInt("location_id")},
            {"locationName", (std::string) res->getString("location_name")},
            {"locationType", (std::string) res->getString("location_type")},
        });

        return crow::response(crow::status::OK, body_response);
    }
    else
    {
        return crow::response(crow::status::NOT_FOUND);
    }
}


crow::response SQLClient::getAllDevices(const crow::request& req)
{
    std::string query = "SELECT serial_number, device_name, \
     device_type, creation_date, locations.location_id, \
     locations.location_name, locations.location_type FROM devices \
     LEFT JOIN device_location ON serial_number = device_sn \
     LEFT JOIN locations ON locations.location_id = device_location.location_id";

    int n_params = req.url_params.keys().size();

    if (n_params != 0) {
        query += " WHERE ";

        _add_string_filter_to_query(query, req, "device_name", "name", n_params);
        _add_string_filter_to_query(query, req, "device_type", "type", n_params);
        _add_int_filter_to_query(query, req, "locations.location_id", "locationId", n_params);
        _add_string_filter_to_query(query, req, "locations.location_name", "locationName", n_params);
        _add_string_filter_to_query(query, req, "locations.location_type", "locationType", n_params);
        _add_date_filter_to_query(query, req, n_params);
    }

    std::cout << "Query: " + query << std::endl;

    std::unique_ptr<sql::Statement> stmt(con->createStatement());
    std::unique_ptr<sql::ResultSet> res(stmt->executeQuery(query));

    std::vector<crow::json::wvalue> body_response;
    int iteration = 0;

    while(res->next())
    {
        crow::json::wvalue device_item(
            {
                {"serialNumber", res->getInt("serial_number")},
                {"name", (std::string) res->getString("device_name")},
                {"type", (std::string) res->getString("device_type")},
                {"creationDate", (std::string) res->getString("creation_date")},
                {"locationId", res->getInt("location_id")},
                {"locationName", (std::string) res->getString("location_name")},
                {"locationType", (std::string) res->getString("location_type")},
            }
        );

        body_response.push_back(device_item);

        iteration++;
    }

    return (crow::json::wvalue) body_response;
}


crow::status SQLClient::postLocation(const crow::json::rvalue& parameters)
{
    bool location_exists = _check_id_exists(con, parameters["locationId"].i(), "locations", "location_id");
    std::string query = "";

    if (location_exists)
    {
        query += "UPDATE locations SET location_name = \"" + (std::string) parameters["locationName"].s()
            + "\", location_type = \"" + (std::string) parameters["locationType"].s()
            + "\" WHERE location_id = " + std::to_string(parameters["locationId"].i());
    }
    else
    {
        query += "INSERT INTO locations (location_id, location_name, location_type) VALUES("
            + std::to_string(parameters["locationId"].i()) + ", \""
            + (std::string) parameters["locationName"].s() + "\", \""
            + (std::string) parameters["locationType"].s() + "\")";
    }

    std::unique_ptr<sql::Statement> stmt(con->createStatement());

    bool exec_status = false;

    try
    {
        stmt->execute(query);

        exec_status = true;
    }
    catch (sql::SQLException &e)
    {
        std::cout << "# ERR: SQLException in " << __FILE__;
        std::cout << " (" << __FUNCTION__ << ") on line " << __LINE__ << std::endl;
        std::cout << "# ERR: " << e.what();
        std::cout << " (MySQL error code: " << e.getErrorCode();
        std::cout << ", SQLState: " << e.getSQLState() << " )" << std::endl;

        exec_status = false;
    }

    return exec_status ? crow::status::OK : crow::status::BAD_REQUEST;
}


crow::response SQLClient::getLocation(const int id)
{
    std::string query = "SELECT location_id, location_name, location_type FROM locations \
     WHERE location_id = " + std::to_string(id);

    std::cout << "Query: " + query << std::endl;

    std::unique_ptr<sql::Statement> stmt(con->createStatement());
    std::unique_ptr<sql::ResultSet> res(stmt->executeQuery(query));

    if (res->rowsCount() > 0)
    {
        res->next();

        crow::json::wvalue body_response({
            {"locationId", res->getInt("location_id")},
            {"locationName", (std::string) res->getString("location_name")},
            {"locationType", (std::string) res->getString("location_type")},
        });

        return crow::response(crow::status::OK, body_response);
    }
    else
    {
        return crow::response(crow::status::NOT_FOUND);
    }
}


crow::response SQLClient::getAllLocations(const crow::request& req)
{
    std::string query = "SELECT location_id, location_name, location_type FROM locations";

    int n_params = req.url_params.keys().size();

    if (n_params != 0) {
        query += " WHERE ";

        _add_string_filter_to_query(query, req, "location_name", "locationName", n_params);
        _add_string_filter_to_query(query, req, "location_type", "locationType", n_params);
    }

    std::cout << "Query: " + query << std::endl;

    std::unique_ptr<sql::Statement> stmt(con->createStatement());
    std::unique_ptr<sql::ResultSet> res(stmt->executeQuery(query));

    std::vector<crow::json::wvalue> body_response;
    int iteration = 0;

    while(res->next())
    {
        crow::json::wvalue device_item(
            {
                {"locationId", res->getInt("location_id")},
                {"locationName", (std::string) res->getString("location_name")},
                {"locationType", (std::string) res->getString("location_type")},
            }
        );

        body_response.push_back(device_item);

        iteration++;
    }

    return (crow::json::wvalue) body_response;
}
