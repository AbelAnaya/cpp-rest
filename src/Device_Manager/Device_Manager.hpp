/***
 * 
 ****/

#include "crow.h"


class DeviceManager {

public:

    static crow::response get_device(const crow::request& req);

    static crow::response post_device(const crow::request& req);

    static crow::response get_device_by_sn(const int serial_number);

    static crow::response delete_device_by_sn(const int serial_number);

    static crow::response put_device_by_sn(const int serial_number);
};
