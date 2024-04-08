/***
 * Device Manager public interface.
 *
 * Defines the methods available to the REST Server to handle endpoint requests.
 * All methods are defined as static so no instance of the class is required.
 *
 ****/

#ifndef DEVICE_MANAGER_H
#define DEVICE_MANAGER_H

#include "crow.h"


class DeviceManager
{
public:

    static crow::response get_device(const crow::request& req);

    static crow::response post_device(const crow::request& req);

    static crow::response get_device_by_sn(const int serial_number);

    static crow::response delete_device_by_sn(const int serial_number);

    static crow::response put_device_by_sn(const crow::request& req, const int serial_number);
};


#endif /* DEVICE_MANAGER_H */
