/***
 * 
 ****/

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
