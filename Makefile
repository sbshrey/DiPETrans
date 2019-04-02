CXX = g++ -std=c++11 -g -O3
LFLAGS = -lthrift -lpthread
MSTARGET = masterServer
WSTARGET = workerServer
MCTARGET = masterClient

SRCS = gen-cpp/MasterService.cpp gen-cpp/WorkerService.cpp gen-cpp/MasterService_types.cpp gen-cpp/WorkerService_types.cpp gen-cpp/SharedService.cpp gen-cpp/SharedService_types.cpp gen-cpp/SharedService_constants.cpp Logger.cpp

all: $(MSTARGET).cpp $(WSTARGET).cpp #$(MCTARGET).cpp 
	${CXX} $(MSTARGET).cpp $(SRCS) -o $(MSTARGET) $(LFLAGS)
	${CXX} $(WSTARGET).cpp $(SRCS) -o $(WSTARGET) $(LFLAGS)
	${CXX} $(MCTARGET).cpp $(SRCS) -o $(MCTARGET) $(LFLAGS)
	
