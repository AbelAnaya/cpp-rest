/***
 * Location Manager public interface.
 *
 * Defines the methods available to the REST Server to handle endpoint requests.
 * All methods are defined as static so no instance of the class is required.
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
