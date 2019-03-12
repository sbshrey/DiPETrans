include "SharedService.thrift"


//declare languages iface generation
namespace cpp MasterService

// declare data structures
struct MasterNode {
	1: required i16 masterID;
	2: required string masterIP;
	3: required i32 masterPort;
}





// declare services used by workers and clients

service MasterService extends SharedService.SharedService{
	// send data to all worker nodes
	void bcastGlobalDataItems ();

	// send transactions to worker nodes to execute
	void sendTransactions ();

}