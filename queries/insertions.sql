use rest_db;

INSERT INTO locations(location_id, location_name, location_type) VALUES
(0x10, "Manufacturing building", "Building"),
(0x11, "Room 134", "Room"),
(0x12, "Front garden", "Outdoor"),
(0x13, "Bay C", "Cargo bay");

INSERT INTO devices(serial_number, device_name, device_type) VALUES
(0x1, "Light sensor", "Sensor"),
(0x2, "Fan", "Actuator"),
(0x3, "Light sensor", "Sensor"),
(0x4, "End of stroke", "Sensor");

INSERT INTO device_location(device_sn, location_id) VALUES
(0x1, 0x10),
(0x2, 0x12),
(0x3, 0x13);
