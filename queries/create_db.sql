DROP DATABASE IF EXISTS rest_db;

CREATE DATABASE rest_db;

USE rest_db;

DROP TABLE IF EXISTS devices;

CREATE TABLE devices (
	serial_number INT NOT NULL AUTO_INCREMENT PRIMARY KEY,
    name VARCHAR(255) NOT NULL,
    type ENUM("Sensor", "Actuator") NOT NULL,
    creation_date TIMESTAMP NOT NULL DEFAULT (NOW())
);

INSERT INTO devices(name, type) VALUES
("Light sensor", "Sensor"),
("Fan", "Actuator"),
("Light sensor", "Sensor");

CREATE TABLE locations (
	location_id INT NOT NULL AUTO_INCREMENT PRIMARY KEY,
    name VARCHAR(255) NOT NULL,
    type ENUM (
		"Dam",
		"Nuclear Power Plant",
		"Farm"
    ) NOT NULL
);

INSERT INTO locations(name, type) VALUES
("Paris Water Reserve", "Dam"),
("London Electric", "Nuclear Power Plant"),
("Berlin Ecologic", "Farm");