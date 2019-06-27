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
	3: required map<string,double> balances;
	4: required map<string,map<string,double>> allowed;
	5: required map<i64,i64> votes;
	6: required list<Transaction> transactions;
	7: required map<string,PlayerRoll> playerRolls;
	8: required double _totalSupply;
	9: required double _maxTotalSupply;
	10: required double _dropReward;
	11: required double _maxDropReward;
	12: required double _rewardBonusTimePeriod;
	13: required double _nextRewardBonus; 
	14: required double _rewardTimePeriod;
	15: required double _rewardStart;
	16: required double _rewardEnd;
	17: required double _currentAirdropped;
	18: required double publicMiningReward;
	19: required double publicMiningSupply;
	20: required double publicMineCallsCount;
	21: required double houseEdge;
	22: required double houseEdgeDivisor;
	23: required double totalGenesisAddresses;
	24: required string genesisCallerAddress;
	25: required map<string,double> genesisInitialSupply;
	26: required map<string,double> genesisRewardPerBlock;
	27: required double maxBlocks;
	28: required map<string,bool> isGenesisAddress;
	29: required double overallSupply;
	30: required bool setupRunning;



	//2: optional map<string,allowedMap> file1; // allowed map file <filename, allowedMap struct>
	//3: optional map<string,balanceMap> file2; // balance map file <filename, balanceMap struct>
	//4: optional map<string,string> file3; // owner file <filename, owner name>
}

struct WorkerResponse {
	1: required map<string,DataItem> dataItemMap;
	2: required list<i16> transactionIDList;
	3: required set<string> contractAddresses;
}


struct Block {
    1: required i32 number;
    2: required i64 nonce;
    3: required string prevHash;
    4: required list<Transaction> transactionsList;
    5: required map<string,DataItem> finalDataItemMap;
    6: required map<i16,list<i64>> sendTxnMap;
}

service SharedService {
	
}
