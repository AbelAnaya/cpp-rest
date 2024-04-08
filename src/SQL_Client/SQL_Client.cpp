/**
 *
 **/

#include "SQL_Client.hpp"
#include <cppconn/exception.h>
#include <cppconn/statement.h>
#include <memory>


SQLClient::SQLClient()
{
    driver = get_driver_instance();
    con = driver->connect("localhost:3306", "root", "2515");

    // Connect to the MySQL rest_db database
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

void SQLClient::updateQuery()
{
    std::unique_ptr<sql::Statement> stmt(con->createStatement());
    std::unique_ptr<sql::ResultSet> res(stmt->executeQuery(
        "UPDATE device_location SET ");
}

void SQLClient::selectQuery()
{
    std::unique_ptr<sql::Statement> stmt(con->createStatement());
    std::unique_ptr<sql::ResultSet> res(stmt->executeQuery(
        "SELECT * FROM devices \
         JOIN device_location ON serial_number = device_sn \
         JOIN locations ON device_location.location_id = locations.location_id \
        "));

    while (res->next()) {
        std::cout << "\t... MySQL replies: ";
        std::cout << res->getString("device_name") << std::endl;
    }
}
