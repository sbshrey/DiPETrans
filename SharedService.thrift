namespace cpp SharedService

struct Transaction {
	1: required i64 transactionID;
	2: required string fromAddress;
	3: required string toAddress;
	4: required double value;
	5: required string input;
	6: required string creates;
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


struct PlayerRoll {
	1: required i64 playerNumber;
	2: required double playerBetValue;
	3: required string playerAddress;
	4: required double playerProfit;
}

struct DataItem {
	//1: required string address;
	1: required double value;
	2: required string owner;
	3: optional map<string,double> balances;
	4: optional map<string,map<string,double>> allowed;
	5: optional map<i64,i64> votes;
	6: optional list<Transaction> transactions;
	7: optional map<string,PlayerRoll> playerRolls;
	//2: optional map<string,allowedMap> file1; // allowed map file <filename, allowedMap struct>
	//3: optional map<string,balanceMap> file2; // balance map file <filename, balanceMap struct>
	//4: optional map<string,string> file3; // owner file <filename, owner name>
}

struct WorkerResponse {
	1: required map<string,DataItem> dataItemMap;
	2: required list<i16> transactionIDList;
	3: optional double transactionFees;
}




service SharedService {
	
}