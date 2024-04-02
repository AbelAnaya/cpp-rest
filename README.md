
# Insert new item to DB with binary serial number
INSERT INTO devices(serial_number, device_name, device_type) VALUES(0xABCD, "Light 3FDXMK", "Sensor");

# Launch MySQL Container with startup script

docker run -p 3306:3306 --name mysql-server \
-v ~/cpp_rest/start_queries:/docker-entrypoint-initdb.d \
-e MYSQL_ROOT_PASSWORD=root \
-e MYSQL_USER=root \
-e MYSQL_PASSWORD=root \
-d mysql:latest
