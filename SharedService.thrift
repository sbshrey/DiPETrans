namespace cpp SharedService
namespace py SharedService

struct Transaction {
	1: required i16 transactionID;
	2: required string fromAddress;
	3: required string toAddress;
	4: required double value;
	5: required string input;
	6: required string creates;
}

struct Account {
	1: required string address;
	2: required double value;
	3: optional string privateKey;
	4: optional string publicKey;
}

struct WorkerNode {
	1: required i16 workerID;
	2: required string workerIP;
	3: required i32 workerPort;
}

struct Uncle {
	1: required string miner;
	2: required i32 number;
}

struct WorkerResponse {
	1: required map<string,double> accountList;
	2: required list<i16> transactionIDList;
	3: required double transactionFees;
}

service SharedService {
	//void ping();
}