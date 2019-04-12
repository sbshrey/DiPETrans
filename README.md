# Distributed-Mining
A Framework for Distributed Mining on Blockchain

# Required Installations
g++
Apache Thrift (0.12.0)


# Compile commands
./compileThrift.sh
make



# Run commands (each command on separate terminal)

starting master server

./masterServer 9090 data/block16/ethereum_data.json

starting 5 worker servers

./workerServer 9091 1

./workerServer 9092 2

./workerServer 9093 3

./workerServer 9094 4

./workerServer 9095 5

# Master client calls master services

starting Master client

./masterClient 9090
