CXX = g++ -std=c++11 -O3
LFLAGS = -lthrift -lpthread -lcrypto
MSTARGET = masterServer
WSTARGET = workerServer
MCTARGET = masterClient
MSMTARGET = masterServer-mining
SERTARGET = serial-contract
SRCS = utils.cpp contract.cpp gen-cpp/MasterService.cpp gen-cpp/WorkerService.cpp gen-cpp/MasterService_types.cpp gen-cpp/WorkerService_types.cpp gen-cpp/SharedService.cpp gen-cpp/SharedService_types.cpp gen-cpp/SharedService_constants.cpp Logger.cpp

all: $(MSTARGET).cpp $(WSTARGET).cpp #$(MCTARGET).cpp 
	${CXX} $(MSTARGET).cpp $(SRCS) -o $(MSTARGET) $(LFLAGS)
	${CXX} $(WSTARGET).cpp $(SRCS) -o $(WSTARGET) $(LFLAGS)
	${CXX} $(MCTARGET).cpp $(SRCS) -o $(MCTARGET) $(LFLAGS)
	${CXX} $(SERTARGET).cpp $(SRCS) -o $(SERTARGET) $(LFLAGS)
	#${CXX} $(MSMTARGET).cpp $(SRCS) -o $(MSMTARGET) $(LFLAGS)
