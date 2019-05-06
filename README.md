# Distributed-Mining
A Framework for Distributed Mining on Blockchain

# Required Installations
g++
Apache Thrift (0.12.0)


# Compile commands
./compileThrift.sh

make

g++ -O3 -std=c++11 Logger.cpp serial-contract.cpp -o serial-contract -lcrypto

g++ -O3 -std=c++11 Logger.cpp serial-mining.cpp -o serial-mining -lcrypto

g++ -O3 -std=c++11 contract_erc20.cpp -o contract_erc20 -lcrypto


# Run commands (each command on separate terminal)

make changes to run.py file

change path for data folder in run.py file (currently running on simulated data for different ratios)

run python run.py (for transaction execution only)

run python run-mining.py (for transaction execution + PoW) 
