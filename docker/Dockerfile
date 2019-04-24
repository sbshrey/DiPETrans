FROM ubuntu
ENV THRIFT_CONTAINER thrift

### Add apt repos

RUN apt-get update -y 
RUN apt-get install -y automake bison flex g++ git libboost-all-dev libevent-dev libssl-dev libtool make pkg-config 

RUN apt-get install -y wget

RUN wget http://mirrors.estointernet.in/apache/thrift/0.12.0/thrift-0.12.0.tar.gz
RUN tar -xvf thrift-0.12.0.tar.gz

WORKDIR thrift-0.12.0

RUN ./bootstrap.sh
RUN ./configure
RUN make
RUN make install
RUN thrift -version

WORKDIR /
RUN rm -rf thrift-0.12.0.tar.gz

COPY Distributed-Mining /home/Distributed-Mining
WORKDIR /home/Distributed-Mining

ENV LD_LIBRARY_PATH /lib:/usr/lib:/usr/local/lib:${LD_LIBRARY_PATH}
RUN ./compileThrift.sh
RUN make

