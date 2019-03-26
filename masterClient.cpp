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
  Logger::instance().log(MSG+" starts", Logger::kLogLevelInfo);
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
	
    
  // range-based for
  //for (int i = start; i <= end; ++i) {
  for (auto& data: ethereum_data.items()) {
    Block block;
    block.number = stoi(data.key());//atoi(d.key().c_str());
    block.miner = data.value()["miner"];
    //block.transactionsList.clear();
    //block.unclesList.clear();
    std::cout << block.number << "\t";
    cout <<  data.value()["transactions"].size() << "\t";
    total_transactions+=data.value()["transactions"].size();


    Logger::instance().log(MSG+" Block "+to_string(block.number)+" execution starts", Logger::kLogLevelInfo);
    std::shared_ptr<TTransport> socket(new TSocket("localhost", port));
    std::shared_ptr<TTransport> transport(new TBufferedTransport(socket));
    std::shared_ptr<TProtocol> protocol(new TBinaryProtocol(transport));
    MasterServiceClient client(protocol);
    transport->open();
    t = clock();
    //string i_str = to_string(data.key());
   

    Logger::instance().log(MSG+" Block "+to_string(block.number)+" transactionsList starts", Logger::kLogLevelInfo);
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
    Logger::instance().log(MSG+" Block "+to_string(block.number)+" transactionsList ends", Logger::kLogLevelInfo);



    cout << data.value()["unclesList"].size() << "\t";
    Logger::instance().log(MSG+" Block "+to_string(block.number)+" unclesList starts", Logger::kLogLevelInfo);
    for (auto& u: data.value()["unclesList"]) {
      Uncle uncle;
      uncle.miner = u["miner"];
      uncle.number = u["number"];
      block.unclesList.push_back(uncle);
    }
    Logger::instance().log(MSG+" Block "+to_string(block.number)+" unclesList ends", Logger::kLogLevelInfo);

    //cout << endl;
    //cout << block.transactionsList.size() << endl;
    //sleep(5);
    Logger::instance().log(MSG+" Block "+to_string(block.number)+" processBlock starts", Logger::kLogLevelInfo);
    client.processBlock(block);
    Logger::instance().log(MSG+" Block "+to_string(block.number)+" processBlock ends", Logger::kLogLevelInfo);
    //sleep(5);
    //cin.get();
    //sleep(5);
    elapsed_time = (double) (clock() - t);
    cout << (elapsed_time)/CLOCKS_PER_SEC << "\n";
    transport->close();
    
    Logger::instance().log(MSG+" Block "+to_string(block.number)+" execution ends", Logger::kLogLevelInfo);
  }
  //cout << total_transactions << endl;
  //cout << endl;
  Logger::instance().log(MSG+" ends", Logger::kLogLevelInfo);

  return 0;
}