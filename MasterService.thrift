include "SharedService.thrift"


//declare languages iface generation
namespace cpp MasterService
namespace py MasterService

// declare data structures
struct MasterNode {
	1: required i16 masterID;
	2: required string masterIP;
	3: required i32 masterPort;
}

struct Block {
	1: required i32 number;
	2: required string miner;
	3: required list<SharedService.Transaction> transactionsList;
	4: required list<SharedService.Uncle> unclesList;
}



// declare services used by workers and clients

service MasterService extends SharedService.SharedService{
	void processBlock (1:Block block);

	// send data to all worker nodes
	// void bcastDataItemsMap ();

	// send transactions to worker nodes to execute
	// void sendTransactions ();

}