use rest_db;

INSERT INTO locations(location_id, location_name, location_type) VALUES
(10, "Manufacturing building", "Building"),
(11, "Room 134", "Room"),
(12, "Front garden", "Outdoor"),
(13, "Bay C", "Cargo bay");

INSERT INTO devices(serial_number, device_name, device_type) VALUES
(1, "Light sensor", "Sensor"),
(2, "Fan", "Actuator"),
(3, "Light sensor", "Sensor"),
(4, "End of stroke", "Sensor");

INSERT INTO device_location(device_sn, location_id) VALUES
(1, 10),
(2, 12),
(3, 13);
