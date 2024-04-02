DROP DATABASE IF EXISTS rest_db;

CREATE DATABASE rest_db;

USE rest_db;

DROP TABLE IF EXISTS locations;

CREATE TABLE locations (
	location_id BINARY(16) PRIMARY KEY DEFAULT (UUID_TO_BIN(UUID())),
    location_name VARCHAR(255) NOT NULL,
    location_type ENUM ("Building", "Room", "Outdoor", "Cargo bay") NOT NULL,
    device_count INT DEFAULT 0
);

DROP TABLE IF EXISTS devices;

CREATE TABLE devices (
	serial_number BINARY(16) PRIMARY KEY DEFAULT (UUID_TO_BIN(UUID())),
    device_name VARCHAR(255) NOT NULL,
    device_type ENUM("Sensor", "Actuator") NOT NULL,
    creation_date TIMESTAMP NOT NULL DEFAULT (NOW())
);

DROP TABLE IF EXISTS device_location;

CREATE TABLE device_location (
	device_sn BINARY(16) NOT NULL PRIMARY KEY,
    location_id BINARY(16) NOT NULL,
    FOREIGN KEY (device_sn) REFERENCES devices(serial_number),
    FOREIGN KEY (location_id) REFERENCES locations(location_id)
);

/* Device count per location trigger */
DELIMITER $$
CREATE TRIGGER increase_device_count
AFTER INSERT ON device_location
FOR EACH ROW
BEGIN
	UPDATE locations SET device_count = device_count + 1
	WHERE location_id = NEW.location_id;
END $$

DELIMITER $$
CREATE TRIGGER decrease_device_count
AFTER DELETE ON device_location
FOR EACH ROW
BEGIN
	UPDATE locations SET device_count = device_count - 1
	WHERE location_id = OLD.location_id;
END $$
