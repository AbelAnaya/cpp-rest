/**
 *
 **/

#ifndef SQL_CLIENT_H
#define SQL_CLIENT_H

#include <mysql_connection.h>
#include <cppconn/driver.h>
#include <cppconn/resultset.h>
#include <crow.h>

class SQLClient
{
private:

    sql::Driver *driver;
    sql::Connection *con;

    static SQLClient *instancePtr;

    SQLClient();
    ~SQLClient();

public:

    // deleting copy constructor.
    SQLClient(const SQLClient &obj) = delete;

    static SQLClient& getInstance();

    /* Device management */
    crow::status postDevice(const crow::json::rvalue& parameters);
    crow::status putDevice(int serial_number, const crow::json::rvalue& parameters);
    crow::response getDevice(const int id);
    crow::response getAllDevices(const crow::request& req);

    /* Location management */
    crow::status postLocation(const crow::json::rvalue& parameters);
    crow::response getLocation(const int id);
    crow::response getAllLocations(const crow::request& req);

    /* Common functions */
    crow::status deleteEntity(const std::string& table, const std::string& column, const std::string& id);
};


#endif /* SQL_CLIENT_H */
