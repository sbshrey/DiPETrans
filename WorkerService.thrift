include  "SharedService.thrift"

namespace cpp WorkerService
namespace py WorkerService



service WorkerService extends SharedService.SharedService {
	// recv data from master
	// void recvDataItemsMap(1: map<string,i64> DataItemsMap);

	SharedService.WorkerResponse recvTransactions(1: list<SharedService.Transaction> TransactionsList, 2: map<string,double> AccountsList);
}