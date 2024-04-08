/***
 * REST Server public interface.
 *
 * Defines the methods available for the REST Server class.
 *
 ****/

#ifndef REST_SERVER_H
#define REST_SERVER_H

#include "crow.h"


class RestServer
{
public:

    RestServer(std::string ip, std::uint16_t port);

    template <typename Func>
    RestServer& add_route(std::string route, crow::HTTPMethod method, Func callback);

    RestServer& start(void);


private:
    std::string _ipAddress;
    std::uint16_t _port;
    crow::SimpleApp app;
};


#endif /* REST_SERVER_H */
