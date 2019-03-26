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

//namespace json = ciere::json;

using json = nlohmann::json;

using namespace ::MasterService;
using namespace ::SharedService;

string MSG="MasterClient";

int main(int argc, char const *argv[]) {
  clock_t t;
  double elapsed_time;
  // read ethereum_data.json

  //int32_t start = atoi(argv[2]);
  //int32_t end = atoi(argv[3]);

  //cout << start << endl;
  //cout << end << endl;
  json ethereum_data;
  string filename = argv[2];
  std::ifstream ethereum_data_file(filename);//, std::ifstream::binary);
  int total_transactions = 0;
  ethereum_data_file >> ethereum_data;

	int port = atoi(argv[1]);
	std::shared_ptr<TTransport> socket(new TSocket("localhost", port));
	std::shared_ptr<TTransport> transport(new TBufferedTransport(socket));
	std::shared_ptr<TProtocol> protocol(new TBinaryProtocol(transport));
	MasterServiceClient client(protocol);
  transport->open();
    
  // range-based for
  //for (int i = start; i <= end; ++i) {
  for (auto& data: ethereum_data.items()) {
    t = clock();
    //string i_str = to_string(data.key());
    std::cout << data.key() << '\t';

    Block block;
    block.number = stoi(data.key());//atoi(d.key().c_str());
    block.miner = data.value()["miner"];

    cout <<  data.value()["transactions"].size() << "\t";
    total_transactions+=data.value()["transactions"].size();

    int16_t txid=0;
    for (auto& tx: data.value()["transactions"]) {
      Transaction transaction;
      transaction.transactionID = txid++;
      transaction.fromAddress = tx["from"];
      transaction.toAddress = tx["to"];
      transaction.value = (double)tx["value"];
      transaction.gas = (double)tx["gas"];
      transaction.gasPrice = (double)tx["gasPrice"]; 
      block.transactionsList.push_back(transaction);
    }

    cout << data.value()["unclesList"].size() << "\t";

    for (auto& u: data.value()["unclesList"]) {
      Uncle uncle;
      uncle.miner = u["miner"];
      uncle.number = u["number"];
      block.unclesList.push_back(uncle);
    }

    cout << endl;
    cout << block.transactionsList.size() << endl;
    sleep(5);
    client.processBlock(block);
    sleep(5);
    //cin.get();
    //sleep(5);
    elapsed_time = (double) (clock() - t);
    cout << (elapsed_time)/CLOCKS_PER_SEC;
  }
  transport->close();
  cout << total_transactions << endl;
  

  return 0;
}