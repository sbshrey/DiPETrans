include  "SharedService.thrift"

namespace cpp WorkerService


service WorkerService extends SharedService.SharedService {
	// recv data from master
	void recvGlobalDataItems(1: map<string,i32> GlobalDataItemsMap);

	map<string,map<i32,list<i16>>> recvTransactions(1: list<SharedService.Transaction> TransactionList);
}