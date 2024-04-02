FROM ubuntu:latest

RUN apt-get update

RUN DEBIAN_FRONTEND=nointeractive apt-get install -y automake make cmake build-essential mysql-server

RUN mkdir -p /app
ADD . /app
WORKDIR /app

CMD ["/app/restapi"]
