DROP DATABASE IF EXISTS rest_db;

CREATE DATABASE rest_db;

USE rest_db;

DROP TABLE IF EXISTS locations;

CREATE TABLE locations (
	location_id INT UNSIGNED NOT NULL AUTO_INCREMENT PRIMARY KEY,
    location_name VARCHAR(255) NOT NULL,
    location_type ENUM ("Building", "Room", "Outdoor", "Cargo bay") NOT NULL,
    device_count INT UNSIGNED DEFAULT 0
);

DROP TABLE IF EXISTS devices;

CREATE TABLE devices (
	serial_number INT UNSIGNED NOT NULL AUTO_INCREMENT PRIMARY KEY,
    device_name VARCHAR(255) NOT NULL,
    device_type ENUM("Sensor", "Actuator") NOT NULL,
    creation_date TIMESTAMP NOT NULL DEFAULT (NOW())
);

DROP TABLE IF EXISTS device_location;

CREATE TABLE device_location (
	device_sn INT UNSIGNED NOT NULL PRIMARY KEY,
    location_id INT UNSIGNED NOT NULL,
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
