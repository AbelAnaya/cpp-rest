FROM ubuntu:latest

RUN apt-get update

RUN DEBIAN_FRONTEND=nointeractive apt-get install -y automake make cmake build-essential libmysqlclient-dev \
libmysqlcppconn-dev

RUN DEBIAN_FRONTEND=nointeractive apt-get install -y wget libboost-all-dev

RUN mkdir -p /app
ADD . /app
WORKDIR /app

# Download CROW 1.0 (REST API Library)
RUN wget https://github.com/CrowCpp/Crow/releases/download/v1.0/crow-v1.0.deb

RUN dpkg -i crow-v1.0.deb

RUN cmake -S . -B build/
RUn make -C build/

CMD ["/app/build/cpp_rest"]
