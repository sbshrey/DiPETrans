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
	1: required string timestamp;
	2: required i64 nonce;
	3: required string prevHash;
	4: required i32 number;
	5: required string miner;
	6: required list<SharedService.Transaction> transactionsList;
	7: required list<SharedService.Uncle> unclesList;
}



// declare services used by workers and clients

service MasterService extends SharedService.SharedService{
	void processBlocks();

	void recvMiningStatus (1: i64 nonce, 2: i32 number);
	// send data to all worker nodes
	// void bcastDataItemsMap ();

	// send transactions to worker nodes to execute
	// void sendTransactions ();

}