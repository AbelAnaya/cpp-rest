SELECT * from device_location;

SELECT location_id as location,
COUNT(*) as devices from device_location
GROUP BY location_id
ORDER BY devices DESC;

SELECT
devices.device_name, devices.device_type,
locations.location_name, locations.location_type from device_location
JOIN devices ON devices.serial_number = device_location.device_sn
JOIN locations ON locations.location_id = device_location.location_id;

SELECT device_sn, location_id
from device_location WHERE device_sn IN (
	SELECT serial_number
    FROM devices
    WHERE device_type = "Sensor"
)
