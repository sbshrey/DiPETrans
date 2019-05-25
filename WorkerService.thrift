include "SharedService.thrift"
include "MasterService.thrift"

namespace cpp WorkerService

struct result {
	1: required i32 number;
	2: required i64 nonce;
}

service WorkerService extends SharedService.SharedService {
	// recv data from master
	// void recvDataItemsMap(1: map<string,i64> DataItemsMap);

	SharedService.WorkerResponse recvTransactions(1: list<SharedService.Transaction> TransactionsList, 2: map<string, SharedService.DataItem> dataItemMap, 3: set<string> contractAddresses);

	oneway void mineBlock(1: MasterService.Block block, 2: i64 nonce, 3: i16 interval);
}