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

    crow::status postDevice(const crow::json::rvalue& parameters);
    crow::status deleteDevice(const std::string& table, const std::string& column, const std::string& id);
    crow::status putDevice(int serial_number, const crow::json::rvalue& parameters);
    crow::response getDevice(const int id);
    crow::response getAllDevices(const crow::request& req);
};


#endif /* SQL_CLIENT_H */
