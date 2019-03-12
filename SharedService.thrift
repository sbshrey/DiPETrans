namespace cpp SharedService

struct Transaction {
	1: required i16 transactionID;
	2: required string senderAddress;
	3: required string receiverAddress;
	4: required i32 amount;
}

struct Account {
	1: required string address;
	2: required i32 amount;
	3: optional string privateKey;
	4: optional string publicKey;
}

struct WorkerNode {
	1: required i16 workerID;
	2: required string workerIP;
	3: required i32 workerPort;
}

service SharedService {
	//void ping();
}