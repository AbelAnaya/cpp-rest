/***
 *
 ****/

#ifndef LOCATION_MANAGER_H
#define LOCATION_MANAGER_H

#include "crow.h"


class LocationManager
{
public:

    static crow::response get_location(const crow::request& req);

    static crow::response post_location(const crow::request& req);

    static crow::response get_location_by_id(const int locationId);

    static crow::response delete_location_by_id(const int locationId);
};


#endif /* LOCATION_MANAGER_H */
