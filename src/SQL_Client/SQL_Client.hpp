/**
 *
 **/

#ifndef SQL_CLIENT_H
#define SQL_CLIENT_H

#include <mysql_connection.h>
#include <cppconn/driver.h>
#include <cppconn/resultset.h>

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

    void selectQuery();
};


#endif /* SQL_CLIENT_H */
