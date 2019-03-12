#include <iostream>
#include <vector>

#include <thrift/protocol/TBinaryProtocol.h>
#include <thrift/transport/TSocket.h>
#include <thrift/transport/TTransportUtils.h>

#include "gen-cpp/MasterService.h"
//#include "gen-cpp/WorkerService.h"

using namespace std;
using namespace apache::thrift;
using namespace apache::thrift::protocol;
using namespace apache::thrift::transport;

using namespace ::MasterService;



int main(int argc, char const *argv[])
{
	int port = atoi(argv[1]);
	std::shared_ptr<TTransport> socket(new TSocket("localhost", port));
	std::shared_ptr<TTransport> transport(new TBufferedTransport(socket));
	std::shared_ptr<TProtocol> protocol(new TBinaryProtocol(transport));
	MasterServiceClient client(protocol);

  transport->open();
  client.bcastGlobalDataItems();
  sleep(2);
  client.sendTransactions();
  sleep(2);
  transport->close();

return 0;
}