include "SharedService.thrift"


//declare languages iface generation
namespace cpp MasterService

// declare data structures
struct MasterNode {
	1: required i16 masterID;
	2: required string masterIP;
	3: required i32 masterPort;
}

struct Block {
	1: required i32 number;
	2: optional i64 nonce;
	3: optional string prevHash;
	4: optional string miner;
	5: required list<SharedService.Transaction> transactionsList;
	6: optional list<SharedService.Uncle> unclesList;
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