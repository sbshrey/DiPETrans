// This autogenerated skeleton file illustrates how to build a server.
// You should copy it to another filename to avoid overwriting it.

#include "gen-cpp/MasterService.h"
#include "gen-cpp/WorkerService.h"
#include "gen-cpp/SharedService.h"

#include <thrift/protocol/TBinaryProtocol.h>
#include <thrift/server/TSimpleServer.h>
#include <thrift/transport/TServerSocket.h>
#include <thrift/transport/TBufferTransports.h>
#include <thrift/transport/TSocket.h>
#include <thrift/concurrency/ThreadManager.h>
#include <thrift/server/TThreadPoolServer.h>
#include <thrift/concurrency/PosixThreadFactory.h>

#include "Logger.h"
#include <iostream>
#include <bits/stdc++.h>
#include <openssl/sha.h>
#include <sstream>
#include <iomanip>


using namespace std;
using namespace ::apache::thrift;
using namespace ::apache::thrift::protocol;
using namespace ::apache::thrift::transport;
using namespace ::apache::thrift::server;
using namespace ::apache::thrift::concurrency;

using namespace  ::MasterService;
using namespace  ::WorkerService;
using namespace  ::SharedService;

std::map<string, int64_t> GlobalDataItemsMap;
std::vector<string> contract_addresses;

string WID = "1";
string dir_path;
string MSG="WorkerServer";

int masterPort = 8090;
string masterIP = "localhost";

Block createBlock(Block b) {
    Block block;
    block.number = b.number; //stoi(data.key());//atoi(d.key().c_str());
    
    for (auto& tx: b.transactionsList) {
      Transaction transaction;
      transaction.transactionID = tx.transactionID;
      transaction.fromAddress = tx.fromAddress;
      transaction.toAddress = tx.toAddress;
      transaction.value = tx.value;
      transaction.input = tx.input; // (double)tx["gas"];
      transaction.creates = tx.creates; // (double)tx["gasPrice"]; 
      block.transactionsList.push_back(transaction);
    }

    return block;
}



string difficulty = "00011111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111";


string sha256(const string str) {
    unsigned char hash[SHA256_DIGEST_LENGTH];
    SHA256_CTX sha256;
    SHA256_Init(&sha256);
    SHA256_Update(&sha256, str.c_str(), str.size());
    SHA256_Final(hash, &sha256);
    //cout << "hash" << hash << endl;
    stringstream ss;
    for(int i = 0; i < SHA256_DIGEST_LENGTH; i++)
    {
        ss << hex << setw(2) << setfill('0') << (int)hash[i];
    }
    return ss.str();
}


string convert_block_to_string(Block block) {
  string str = "";
  //str.append(block.timestamp);
  str.append(to_string(block.nonce));
  str.append(block.prevHash);
  str.append(to_string(block.number));
  str.append(block.miner);

  for (auto& tx : block.transactionsList) {
    str.append(to_string(tx.transactionID));
    str.append(tx.toAddress);
    str.append(tx.fromAddress);
    str.append(to_string(tx.value));
    str.append(tx.input);
    str.append(tx.creates);
  }

  for (auto& u : block.unclesList) {
    str.append(u.miner);
    str.append(to_string(u.number));
  }

  return str;
}

bool execute (Transaction transaction) {
  bool status = false;
  string cmd = "";

  if (transaction.toAddress == "creates") {
    cmd = "./contract_erc20 " + transaction.creates + " " + transaction.fromAddress + " " + transaction.toAddress;
  } else {
    cmd = "./contract_erc20 " + transaction.toAddress + " " + transaction.fromAddress + " " + transaction.input;
  }
  const char *command = cmd.c_str();
  status = system(command);
  return true;
}


class WorkerServiceHandler : virtual public WorkerServiceIf {
 public:
  WorkerServiceHandler() {
    // Your initialization goes here    
      
  }

  void recvTransactions( ::SharedService::WorkerResponse& _return, const std::vector< ::SharedService::Transaction> & TransactionsList, const std::map<std::string, double> & AccountsList) {
    // Your implementation goes here
    ofstream nttfile;
    nttfile.open(dir_path + "be_ntt_"+WID+".log",std::ofstream::out | std::ofstream::app);
    ofstream cttfile;
    cttfile.open(dir_path + "be_ctt_"+WID+".log",std::ofstream::out | std::ofstream::app);
    double normal_txn_time = 0;
    double contract_txn_time = 0;

    Logger::instance().log(MSG+" worker "+ WID +" recvTransactions starts", Logger::kLogLevelInfo);
    auto start = chrono::steady_clock::now();
    int successful_transactions = 0;
    int failed_transactions = 0;
    int total_transactions = 0;


    int sc_cnt = 0;
    int nsc_cnt = 0;

    double tx_fees = 0;

    Logger::instance().log(MSG+" AccountsList starts", Logger::kLogLevelInfo);
    for (auto const& account: AccountsList)
    {
      _return.accountList[account.first] = account.second;
    }
    Logger::instance().log(MSG+" AccountsList ends", Logger::kLogLevelInfo);

    Logger::instance().log(MSG+" TransactionsList starts", Logger::kLogLevelInfo);
    //cout << endl;
    for (auto const& tx: TransactionsList) {
      //cout << "tx" << tx.transactionID << "\t";
      auto txn_start = chrono::steady_clock::now();
      auto txn_end = chrono::steady_clock::now();
      double fee;


      bool status= false;
      if (tx.creates != "") {
        contract_addresses.push_back(tx.creates);
        status = execute(tx);
        txn_end = chrono::steady_clock::now();
        contract_txn_time += chrono::duration_cast<chrono::microseconds>(txn_end - txn_start).count();
        sc_cnt++;
      } else {
        std::vector<string>::iterator it = std::find (contract_addresses.begin(), contract_addresses.end(), tx.toAddress); 
        if (it != contract_addresses.end()) {  
            status = execute(tx);
            txn_end = chrono::steady_clock::now();
            contract_txn_time += chrono::duration_cast<chrono::microseconds>(txn_end - txn_start).count();
            sc_cnt++;
        } 
        else {
          if (_return.accountList[tx.fromAddress] >= double(tx.value)) {
            _return.accountList[tx.fromAddress] = _return.accountList[tx.fromAddress] - double(tx.value) - fee;
            _return.accountList[tx.toAddress] = _return.accountList[tx.toAddress] + double(tx.value);
            status = true;  
          } else {
            status = false;
          }
          txn_end = chrono::steady_clock::now();
          normal_txn_time += chrono::duration_cast<chrono::microseconds>(txn_end - txn_start).count();
          nsc_cnt++;
        }
      }
        
      //cout << endl;
      if (status) {
        successful_transactions++;
      } else {
        failed_transactions++;
      }
      tx_fees += fee;
      _return.transactionIDList.push_back(tx.transactionID);
      total_transactions++;
    }

    cout << "WID " << WID << "\tsc " << sc_cnt << "\tnsc " << nsc_cnt << endl;

    cttfile << contract_txn_time << endl;
    nttfile << normal_txn_time << endl;

    _return.transactionFees = tx_fees;
    Logger::instance().log(MSG+" TransactionsList ends", Logger::kLogLevelInfo);

    Logger::instance().log(MSG+" worker "+ WID +" recvTransactions ends", Logger::kLogLevelInfo);
    auto end = chrono::steady_clock::now();
  }

  void mineBlock(const  ::MasterService::Block& block, const int16_t nonce, const int16_t interval) {
    // Your implementation goes here
    //block.nonce = nonce;
    Block newBlock = createBlock(block);

    newBlock.nonce = nonce;
    string hash;
    while (true) {
      string block_str = convert_block_to_string(newBlock);
      hash = sha256(block_str);
      if (hash.compare(difficulty) < 0) {
        //cout << newBlock.nonce << endl;
        break;
      }

      newBlock.nonce+=interval;
    }
  
    
    std::shared_ptr<TTransport> socket(new TSocket(masterIP, masterPort));
    std::shared_ptr<TTransport> transport(new TBufferedTransport(socket));
    std::shared_ptr<TProtocol> protocol(new TBinaryProtocol(transport));
    MasterServiceClient masterClient(protocol);

    Logger::instance().log(MSG+" Block "+to_string(block.number) +" connection to master "+WID+" starts", Logger::kLogLevelInfo);
    transport->open();


    Logger::instance().log(MSG+" Block "+to_string(block.number) +" worker "+WID+" mineBlock() starts", Logger::kLogLevelInfo);

    // sent transaction to execute     
    
    masterClient.recvMiningStatus(newBlock.nonce, newBlock.number); // returns local worker response
    //cout << worker->threadID << ":" << sendTransactionMap[worker->workerID].size() << "\t";

    Logger::instance().log(MSG+" Block "+to_string(block.number) +" WorkerID "+WID+" mineBlock() ends", Logger::kLogLevelInfo);
    
    transport->close();
    Logger::instance().log(MSG+" Block "+to_string(block.number) +" connection to master "+WID+" ends", Logger::kLogLevelInfo);
    

    //_return.number = newBlock.number;
    //_return.nonce = newBlock.nonce;

    //printf("mineBlock\n");
  }


};

int main(int argc, char **argv) {
  int port = atoi(argv[1]);
  WID = argv[2];
  dir_path = argv[3];

  ::apache::thrift::stdcxx::shared_ptr<WorkerServiceHandler> handler(new WorkerServiceHandler());
  ::apache::thrift::stdcxx::shared_ptr<TProcessor> processor(new WorkerServiceProcessor(handler));
  ::apache::thrift::stdcxx::shared_ptr<TServerTransport> serverTransport(new TServerSocket(port));
  ::apache::thrift::stdcxx::shared_ptr<TTransportFactory> transportFactory(new TBufferedTransportFactory());
  ::apache::thrift::stdcxx::shared_ptr<TProtocolFactory> protocolFactory(new TBinaryProtocolFactory());

  shared_ptr<ThreadManager> threadManager = ThreadManager::newSimpleThreadManager(5);
  shared_ptr<PosixThreadFactory> threadFactory = shared_ptr<PosixThreadFactory>(new PosixThreadFactory());
  threadManager->threadFactory(threadFactory);
  threadManager->start();

  TThreadPoolServer server(processor, serverTransport, transportFactory, protocolFactory, threadManager);
  server.serve();
  return 0;
}
