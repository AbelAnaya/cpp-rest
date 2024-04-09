# Introduction

Factory Device Manager (FDM) is a HTTP RESTful API to manage device and locations
in a factory floor.

The REST API allows to retrieve and save devices and locations into a MySQL
Database.

Each device can be associated to a single location.
Each location can be associated to many devices.

See more about database relations in [Database Design](#database-design)

# Design documentation

The design has been divided in 3 parts:

-   RESTful API Design.
-   Database Design.
-   SW Approach. Language and Libraries.

## RESTful API Design

The API has been designed following RESTful principles and it has been
specified using OpenAPI specification.

Main resources are:
- **/devices**
- **/locations**

For each one of these resources several methods and query parameters are
allowed.

For more details about the REST API, use the OpenAPI specification provided in
`openapi/openapi.yaml` with an OpenAPI viewer tool.

During the design phase Swagger.io Editor was used as OpenAPI tool:
https://swagger.io/tools/swagger-editor/

## Database Design

The database design has been specified using DBML. It is available in
`dbml/rest_db.dml` or as png image in `dbml/rest_db.png`

DBML files can be processed using an online tool: https://dbdiagram.io/d

The database consists of 3 tables:
- **devices:** Stores all devices informations
- **locations:** Stores all locations available
- **device_location:** Stores relations between devices and locations i.e: in which
  location is placed each device.

An unsigned integer auto incremental has been used for location and device identifiers.
It stands as the primary key of locations and devices tables. Originally it was
decided to use a binary UUID, but due to the complications that it brings in the
API management it has been replaced by an integer identifier for this demo project.

device_location table is formed by two foreign keys: device serial number and
location identifier, primary keys in devices and locations respectively.

Device serial number is the primary key of device_location, ensuring that a device
can only be assigned to a single location. Logically it would be impossible to have
the same device in two different locations.

Indexes for device name and location name has been added to each respective table
in order to speed up queries looking for this properties, which are expected to be
the most popular ones.

Locations table contains a trigger mechanism in the device count property.
This property has not been published in the REST API, but it could be used in the future
to quickly query the number of devices that are in a given location.

As represented in the DBML. The relations between tables are:
- devices.serial_number - device_location.device_sn --> One to one relation
- locations.location_id < device_location.location_id --> One to many relation

## SW Approach. Language and Libraries.

C++ has been selected as the implementation language. The REST API has been
implemented using Crow library, which provides a fast development for REST API.

SQL management has been implemented using mysqlclient and mysqlcppconn libraries.

# Build and run commands

The setup of the project consists of a process executing the REST API Server
and a MySQL server running with the defined database and tables.

Docker has been used to containerize the REST API Server, and also to deploy
a MySQL server instance that interacts with the REST API.

To start everything:
    - Build REST API Docker Image
    - Run REST API Container
    - Run MySQL Container
    - Start interaction with REST API using curl commands.

All these steps are detailed in the following sections.

## Build REST API Docker image

While in root folder of this repository:

```sh
docker build . -t restapi:latest
```

## Run REST API Container

```sh
docker run --net host restapi:latest
```

> `--net host` has been used in order to quickly connect with a running instance
> of a mysql database in the host. The correct setup would be to create a subnet
> in docker where the two containers interact isolated from the host, but due
> to the additional complexity in the deployment and the requirement of some
> additional steps that need to be executed by the reviewer it has been
> discarded in favor of using host's network directly.

## Run MYSQL Container

Docker container with mysql image to interact with REST API. A local MySQL
server running in the hosts machine would also be valid.

If local MySQL is used, please execute `queries/start_query/create_db.sql` to
create database and tables.

Ensure the database is reachable in host machine by using IP Address 0.0.0.0 on
port 3306.

Using mysql container with the command bellow, would be enough.

```sh
docker run \
-v ~/fdm/queries/start_query:/docker-entrypoint-initdb.d \
-p 3306:3306 \
--name mysql-server \
-e MYSQL_ROOT_PASSWORD=root \
-d mysql:latest
```

# Interaction with REST API

`curl` commands are provided to interact with the REST API using the CLI.

Execute these commands in the machine running the containers.

For those commands that return a body in the HTTP response an output example
is provided.

For the others, if successfull `curl` command returns nothing if HTTP 200, and
prints a message if there is any error, indicating the error code i.e: 400,
404, 500

## Devices

### Get devices (GET)

Get all devices:

```sh
curl -X 'GET' 'http://localhost:8080/devices' -H 'accept: application/json'
```

Output example:
```json
[
    {
        "creationDate":"2024-04-09 12:21:00",
        "locationId":0,
        "locationName":"",
        "locationType":"",
        "name":"P3KL Fan",
        "serialNumber":1,
        "type":"Actuator"

    },
    {
        "creationDate":"2024-04-09 11:58:57",
        "locationId":1,
        "locationName":"Market Room",
        "locationType":"Room",
        "name":"XLNS3 Light Sensor",
        "serialNumber":2,
        "type":"Sensor"
    }
]
```

Get all devices with filters:

```sh
curl -X 'GET' 'http://localhost:8080/devices?name=XLNS3%20Light%20Sensor&type=Sensor&locationId=1&locationName=Market%20Room&locationType=Room&creationDateStart=2024-04-02&creationDateEnd=2024-04-16' -H 'accept: application/json'
```

Output example:
```json
[
    {
        "creationDate":"2024-04-09 11:58:57",
        "locationId":1,
        "locationName":"Market Room",
        "locationType":"Room",
        "name":"XLNS3 Light Sensor",
        "serialNumber":2,
        "type":"Sensor"
    }
]
```

### Get device by identifier (GET)

Get device by id:

```sh
curl -X 'GET' 'http://localhost:8080/devices/2' -H 'accept: application/json'
```

Output example:
```json
{
    "creationDate":"2024-04-09 11:58:57",
    "locationId":1,
    "locationName":"Market Room",
    "locationType":"Room",
    "name":"XLNS3 Light Sensor",
    "serialNumber":2,
    "type":"Sensor"
}
```

### Add or update device (POST)

Add or update a new device:

```sh
curl -X 'POST' 'http://localhost:8080/devices' -H 'accept: application/json' -d '{
"name": "XLNS3 Light Sensor",
"type": "Sensor",
"serialNumber": 2
}'
```

Add or update a new device and link it to an existing location:

```sh
curl -X 'POST' 'http://localhost:8080/devices' -H 'accept: application/json' -d '{
"name": "XLNS3 Light Sensor",
"type": "Sensor",
"serialNumber": 2,
"locationId": 1
}'
```

### Remove device (DELETE)

Remove device:

```sh
curl -X 'DELETE' 'http://localhost:8080/devices/2' -H 'accept: application/json'
```

> Note that in order to be able to remove a device it must not be linked to any
> location.
> This version of REST API doesn't allow removing a location from a device,
> a workaround would be to create a dummy location, change device's location to
> new dummy location, delete dummy location, and finally delete device.
>
> Apologies for this inconvenience. The issue was detected during testing and
> since it can be workarounded, it has been kept in the SW due to time
> constraints.

### Edit device location (PUT)

Edit device location. Expects an existing location:

```sh
curl -X 'PUT' 'http://localhost:8080/devices/2/location' -H 'accept: application/json' -d '{ "locationId": 2 }'
```

## Locations

### Get locations (GET)

Get all locations:

```sh
curl -X 'GET' 'http://localhost:8080/locations' -H 'accept: application/json'
```

Output example:
```json
[
    {
        "locationId":1,
        "locationName":"Market Room",
        "locationType":"Room"

    },
    {
        "locationId":2,
        "locationName":"Engineering Room",
        "locationType":"Room"

    }
]
```

Get all locations with filters:

```sh
curl -X 'GET' 'http://localhost:8080/locations?locationName=Market%20Room&locationType=Room' -H 'accept: application/json'
```

Output example:
```json
[
    {
        "locationId":1,
        "locationName":"Market Room",
        "locationType":"Room"
    }
]
```

### Get location by identifier (GET)

Get location by id:

```sh
curl -X 'GET' 'http://localhost:8080/locations/2' -H 'accept: application/json'
```

Output example:
```json
{
    "locationId":2,
    "locationName":"Engineering Room",
    "locationType":"Room"
}
```

### Add or update location (POST)

Add or update a location:

```sh
curl -X 'POST' 'http://localhost:8080/locations' -H 'accept: application/json' -d '{
"locationId": 1,
"locationName": "Market Room",
"locationType": "Room"
}'
```

### Remove location (DELETE)

Remove location:

```sh
curl -X 'DELETE' 'http://localhost:8080/locations/1' -H 'accept: application/json'
```
