#include <iostream>
#include <vector>
//#include "json_spirit/ciere/json/value.hpp"
#include "nlohmann/json.hpp"
#include <fstream>
#include <cstdlib>

#include <thrift/protocol/TBinaryProtocol.h>
#include <thrift/transport/TSocket.h>
#include <thrift/transport/TTransportUtils.h>

#include "Logger.h"
#include "gen-cpp/MasterService.h"
#include "gen-cpp/SharedService.h"
//#include "gen-cpp/WorkerService.h"




using namespace std;
using namespace apache::thrift;
using namespace apache::thrift::protocol;
using namespace apache::thrift::transport;

using json = nlohmann::json;

using namespace ::MasterService;
using namespace ::SharedService;

string MSG="MasterClient";

//192.168.0.11

int main(int argc, char const *argv[]) {
  int port = atoi(argv[1]);
  std::shared_ptr<TTransport> socket(new TSocket("192.168.0.11", port));
  std::shared_ptr<TTransport> transport(new TBufferedTransport(socket));
  std::shared_ptr<TProtocol> protocol(new TBinaryProtocol(transport));
  MasterServiceClient client(protocol);
  transport->open();
  client.processBlocks();
  transport->close();
  return 0;
}
