// This autogenerated skeleton file illustrates how to build a server.
// You should copy it to another filename to avoid overwriting it.

#include "gen-cpp/MasterService.h"
#include "gen-cpp/WorkerService.h"
#include "gen-cpp/SharedService.h"

#include "nlohmann/json.hpp"


//#include <thrift/server/TNonblockingServer.h>
#include <thrift/protocol/TBinaryProtocol.h>
#include <thrift/server/TSimpleServer.h>
#include <thrift/transport/TServerSocket.h>
#include <thrift/transport/TBufferTransports.h>
//#include <thrift/TFramedTransport.h>
#include <thrift/concurrency/Exception.h>
#include <thrift/concurrency/PosixThreadFactory.h>
#include <thrift/transport/PlatformSocket.h>
#include <thrift/concurrency/ThreadManager.h>
#include <thrift/server/TThreadPoolServer.h>
#include <thrift/server/TThreadedServer.h>
#include <thrift/transport/TServerSocket.h>
#include <thrift/transport/TSocket.h>
#include <thrift/transport/TTransportUtils.h>
//#include <thrift/transport/TNonblockingServerTransport.h>
//#include <thrift/transport/TNonblockingServerSocket.h>
#include <thrift/TToString.h>

#include <iostream>
#include <string>
#include <pthread.h>
#include <thread>
#include <cstdlib>
#include <vector>
//#include <random>
//#include <functional>
//include <algorithm>  
#include "Logger.h"

#include <execinfo.h>
#include <signal.h>
#include <stdlib.h>
#include <unistd.h>


#include <openssl/sha.h>
#include <sstream>
#include <iomanip>

//#define NUM_WORKERS 5
//#define NUM_THREADS 5
//#define NUM_ACCOUNTS 50
//#define NUM_TRANSACTIONS 10
//#define RANDOM_SEED 10


using namespace std;

using json = nlohmann::json;

using namespace ::apache::thrift;
using namespace ::apache::thrift::protocol;
using namespace ::apache::thrift::transport;
using namespace ::apache::thrift::concurrency;
using namespace ::apache::thrift::server;

using namespace  ::MasterService;
using namespace  ::WorkerService;
using namespace  ::SharedService;


Block block;
bool minerStatus = false;
string filename;
string block_hash = "0000000000000000000000000000000000000000000000000000000000000000";
string difficulty = "00111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111";



vector<WorkerNode> WorkerList;

//vector<Transaction> TransactionList;
//vector<Account> AccountList;




map<string,double> DataItemsMap;
map<int16_t, map<set<string>,set<int16_t>>> GlobalConflictsMap; // map<list<addresses>, list<txid>>
map<string,list<int16_t>> LocalConflictsMap; // map<address,list<txid>>

map<int16_t,std::vector<WorkerResponse>> GlobalWorkerResponsesList;

map<string,list<string>> AdjacencyMap;


// object to store json ethereum blocks data
json ethereum_data;

int NUM_WORKERS = 5;
int NUM_THREADS = 5;


//pthread_t global_threads[10];

const string MSG="MasterServer";
const double base_reward = 3000000000000000000;

struct thread_data {
   int16_t  threadID;
   int16_t workerID;
   string workerIP;
   int32_t workerPort;
   string miner;
   int number;
};


// Global Data Structures used by multiple functions

map<int16_t,vector<Transaction>> sendTransactionMap;
map<int16_t, set<int16_t>> ccTransactionMap;


void signal_handler(int sig) {
  void *array[10];
  size_t size;

  // get void*'s for all entries on the stack
  size = backtrace(array, 10);

  // print out all the frames to stderr
  fprintf(stderr, "Error: signal %d:\n", sig);
  backtrace_symbols_fd(array, size, STDERR_FILENO);
  exit(1);
}


void createBlock(json::iterator data) {
  //for (auto& data: ethereum_data.items()) {
  
  //Block block;
  Logger::instance().log(MSG+" Block "+to_string(block.number)+" creation starts", Logger::kLogLevelInfo);
  block.number = stoi(data.key());//atoi(d.key().c_str());
  block.timestamp = data.value()["timestamp"];
  block.nonce = 0;
  block.prevHash = "0000000000000000000000000000000000000000000000000000000000000000";
  block.miner = data.value()["miner"];

  Logger::instance().log(MSG+" Block "+to_string(block.number)+" transactionsList starts", Logger::kLogLevelInfo);
  int16_t txid=0;
  for (auto& tx: data.value()["transactions"]) {
    Transaction transaction;
    transaction.transactionID = txid++;
    transaction.fromAddress = tx["from"];
    if (tx["to"].is_null()) {
      transaction.toAddress = "creates";
    } else {
      transaction.toAddress = tx["to"];
    }

    if (tx["value"].is_number()) {
      transaction.value = tx["value"].get<double>();
    } else {
      transaction.value = stod(tx["value"].get<string>());
    }
    //transaction.toAddress = tx["to"];
    //transaction.value = (double)tx["value"];
    transaction.input = tx["input"];
    if (!tx["creates"].is_null()) {
      transaction.creates = tx["creates"];
      //contract_addresses.push_back(transaction.creates);
    } else {
      transaction.creates = "";
    }
    //transaction.creates = tx["creates"]; 
    block.transactionsList.push_back(transaction);
  }
  Logger::instance().log(MSG+" Block "+to_string(block.number)+" transactionsList ends", Logger::kLogLevelInfo);
  
  Logger::instance().log(MSG+" Block "+to_string(block.number)+" unclesList starts", Logger::kLogLevelInfo);
  for (auto& u: data.value()["unclesList"]) {
    Uncle uncle;
    uncle.miner = u["miner"];
    uncle.number = u["number"];
    block.unclesList.push_back(uncle);
  }
  Logger::instance().log(MSG+" Block "+to_string(block.number)+" unclesList ends", Logger::kLogLevelInfo);

  Logger::instance().log(MSG+" Block "+to_string(block.number)+" creation starts", Logger::kLogLevelInfo);
}

// Clears memory of global data structures after every block creation
void clear_memory() {
  sendTransactionMap.clear();
  ccTransactionMap.clear();
  GlobalConflictsMap.clear();
  LocalConflictsMap.clear();
  GlobalWorkerResponsesList.clear();
  AdjacencyMap.clear();
  block.transactionsList.clear();
  block.unclesList.clear();
}

string sha256(const string str) {
    unsigned char hash[SHA256_DIGEST_LENGTH];
    SHA256_CTX sha256;
    SHA256_Init(&sha256);
    SHA256_Update(&sha256, str.c_str(), str.size());
    SHA256_Final(hash, &sha256);
    stringstream ss;
    for(int i = 0; i < SHA256_DIGEST_LENGTH; i++)
        ss << hex << setw(2) << setfill('0') << (int)hash[i];
    return ss.str();
}

string convert_block_to_string(Block block) {
  string str = "";
  str.append(block.timestamp);
  str.append(to_string(block.nonce));
  str.append(block.prevHash);
  str.append(to_string(block.number));
  str.append(block.miner);

  for (auto& tx : block.transactionsList) {
    str.append(to_string(tx.transactionID));
    str.append(tx.toAddress);
    str.append(tx.fromAddress);
    str.append(std::to_string(tx.value));
    str.append(tx.input);
    str.append(tx.creates);
  }

  for (auto& u : block.unclesList) {
    str.append(u.miner);
    str.append(to_string(u.number));
  }
  return str;
}

// Recursive DFS to find all connected components
void DFSUtil (int ccID, string v, map<string,bool> &visited) {
  // Mark the current node as visited and print it
  visited[v] = true;

  for (auto const& tx: LocalConflictsMap[v])
    ccTransactionMap[ccID].insert(tx);
  // Recur for all vertices
  // adjacent to this vertex
  
  for (auto const& vc: AdjacencyMap[v]) {
    if (visited[vc] == false) {
      DFSUtil(ccID,vc,visited);
    }
  }
}

// Creates graph and starts search for identifying connected components
// Connected components are considered as dependent transactions club together
// and are executed serially on the workers
void analyze(vector<Transaction> TransactionList) {
  std::set<string> AddressList;

  Logger::instance().log(MSG+" AdjacencyMap starts", Logger::kLogLevelInfo);
  for (auto const& tx: TransactionList) {
    LocalConflictsMap[tx.fromAddress].push_back(tx.transactionID);
    if (tx.toAddress == "creates") {
      LocalConflictsMap[tx.creates].push_back(tx.transactionID);
      AdjacencyMap[tx.creates].push_back(tx.fromAddress); 
      AddressList.insert(tx.creates);
      AdjacencyMap[tx.fromAddress].push_back(tx.creates);
    } else {
      LocalConflictsMap[tx.toAddress].push_back(tx.transactionID);
      AdjacencyMap[tx.toAddress].push_back(tx.fromAddress);
      AddressList.insert(tx.toAddress);
      AdjacencyMap[tx.fromAddress].push_back(tx.toAddress);
    }
    
    AddressList.insert(tx.fromAddress);
    
  }
  Logger::instance().log(MSG+" AdjacencyMap ends", Logger::kLogLevelInfo);

  map<string,bool> visited;

  // Mark all vertices as not visited
  for (auto const& address: AddressList) {
    visited[address] = false;
  }

  int ccID = 1;
  int id = 0;
  int minID = 0;

  for (auto const& address: AddressList) {
    if (visited[address] == false) {
      // print all reachable vertices
      DFSUtil(ccID,address,visited);
      
      // Creation of Map to send dependent transactions to workers
      // Load balanced ny identifying the least loaded worker and 
      // appended the next connected components transactions to that worker

      if (ccTransactionMap[ccID].size() > 0) {
        for (auto const& txid: ccTransactionMap[ccID]) {
          // load balancing across workers 
          sendTransactionMap[WorkerList[id].workerID].push_back(TransactionList[txid]);
        }

        if (ccID >= 5) {
          map<int16_t,vector<Transaction>>::iterator it1;
          map<int16_t,vector<Transaction>>::iterator it2;
          for (it1 = sendTransactionMap.begin(); it1 != sendTransactionMap.end(); ++it1)
          {
            for (it2 = sendTransactionMap.begin() ; it2 != sendTransactionMap.end(); ++it2)
            {
              if (it1->second.size() > it2->second.size()) {
                id = it2->first - 1;
              }
            }
          }
        } else {
          id = (id+1)%NUM_WORKERS;
        }
        ccID++;
      }
    }
  }
}


// multi threaded function call to spin parallel connection to each worker from master

void *connectWorker (void *threadarg) {
  map< string,double> LocalDataItemsMap;

  struct thread_data *worker;
  worker = (struct thread_data *) threadarg;

  Logger::instance().log(MSG+" Block "+to_string(worker->number) +" thread "+ to_string(worker->workerID) +" starts", Logger::kLogLevelInfo);
      

  std::shared_ptr<TTransport> socket(new TSocket(worker->workerIP, worker->workerPort));
  std::shared_ptr<TTransport> transport(new TBufferedTransport(socket));
  std::shared_ptr<TProtocol> protocol(new TBinaryProtocol(transport));
  WorkerServiceClient workerClient(protocol);

  Logger::instance().log(MSG+" Block "+to_string(worker->number) +" connection to worker "+to_string(worker->workerID)+" starts", Logger::kLogLevelInfo);
  transport->open();

  Logger::instance().log(MSG+" Block "+to_string(worker->number) +" LocalDataItemsMap generation for worker "+to_string(worker->workerID)+" starts", Logger::kLogLevelInfo);
  
  // Creates data items map of address which are going to be modified by the transactions sent to worker
  // instead of sending complete map of addresses

  for (auto& tx: sendTransactionMap[worker->workerID]) {
    LocalDataItemsMap[tx.fromAddress] = DataItemsMap[tx.fromAddress];
    LocalDataItemsMap[tx.toAddress] = DataItemsMap[tx.toAddress];
  }
  Logger::instance().log(MSG+" Block "+to_string(worker->number) +" LocalDataItemsMap generation for worker "+to_string(worker->workerID)+" ends", Logger::kLogLevelInfo);
  
  Logger::instance().log(MSG+" Block "+to_string(worker->number) +" WorkerID "+to_string(worker->workerID)+" recvTransactions() starts", Logger::kLogLevelInfo);

  WorkerResponse LocalWorkerResponse;
  
  workerClient.recvTransactions(LocalWorkerResponse,sendTransactionMap[worker->workerID],LocalDataItemsMap); // returns local worker response

  cout << worker->threadID << ":" << sendTransactionMap[worker->workerID].size() << "\t";
  Logger::instance().log(MSG+" Block "+to_string(worker->number) +" WorkerID "+to_string(worker->workerID)+" recvTransactions() ends", Logger::kLogLevelInfo);
  
  map<string,double>::iterator it;
  for (it = LocalWorkerResponse.accountList.begin(); it != LocalWorkerResponse.accountList.end(); ++it)
  {
    DataItemsMap[it->first] = it->second;
  }

  DataItemsMap[worker->miner] += LocalWorkerResponse.transactionFees; 

  transport->close();
  Logger::instance().log(MSG+" Block "+to_string(worker->number) +" connection to worker "+to_string(worker->workerID)+" ends", Logger::kLogLevelInfo);
  Logger::instance().log(MSG+" Block "+to_string(worker->number) +" thread "+ to_string(worker->workerID) +" ends", Logger::kLogLevelInfo);
      
  pthread_exit(&worker->threadID);
}




class MasterServiceHandler : virtual public MasterServiceIf {
 public:
  MasterServiceHandler() {
    // Your initialization goes here
    
    int port = 9091;
    vector<string> workers_ip_list {"172.17.0.3", "172.17.0.4", "172.17.0.5","172.17.0.6", "172.17.0.7"};

    string ip = "localhost";



    for (int id=1; id<=NUM_WORKERS; id++) {
      WorkerNode workerNode;
      workerNode.workerID = id;
      workerNode.workerIP = workers_ip_list[id-1];
      workerNode.workerPort = port++;

      WorkerList.push_back(workerNode);
    }

    Logger::instance().log(MSG+" Initializing accounts with 100000000000000000000000 wei to execute transactions starts", Logger::kLogLevelInfo);
    
    string line;
    ifstream accounts_file ("data/block16/accounts.json");
    json accounts;
    accounts_file >> accounts;

    // iterate the array
    for (json::iterator it = accounts.begin(); it != accounts.end(); ++it) {
      DataItemsMap[it.key()] = it.value();
    }
    Logger::instance().log(MSG+" Initializing accounts with 100000000000000000000000 wei to execute transactions ends", Logger::kLogLevelInfo);
    
    //string filename = argv[3];
    std::ifstream ethereum_data_file(filename);//, std::ifstream::binary);
    //int total_transactions = 0;
    ethereum_data_file >> ethereum_data;    

    
    
  }

  // process block received from masterClient

  void processBlocks() {
    // open a file in write mode.
    
    // Your implementation goes here
    for (json::iterator data = ethereum_data.begin(); data != ethereum_data.end(); ++data) {
      ofstream outfile;
      outfile.open("masterServer.log",std::ofstream::out | std::ofstream::app);
    //for (auto& data: ethereum_data.items()) {
      // block creation starts
      auto start = chrono::steady_clock::now();
      createBlock(data);
      //if (block.number >= 4370100) break;  
      
      block.prevHash = block_hash;
      outfile << block.number << "\t" << block.transactionsList.size() << "\t" << block.unclesList.size() << "\t";
      auto end1 = chrono::steady_clock::now();
      outfile << chrono::duration_cast<chrono::microseconds>(end1 - start).count() << "\t";
      // miner status set to false for each block
      // it is to stop mining once solution is found
      minerStatus = false;
      double uncle_reward = 0;


      if (block.transactionsList.size() > 0) {
        Logger::instance().log(MSG+" Block "+to_string(block.number)+" analyze starts", Logger::kLogLevelInfo);
        analyze(block.transactionsList);
        Logger::instance().log(MSG+" Block "+to_string(block.number)+" analyze ends", Logger::kLogLevelInfo);

        //sendTransactions
        
        pthread_t threads[NUM_THREADS];
        pthread_attr_t attr;
        struct thread_data td[NUM_THREADS];
        int rc;
        int thID = 0;
        void *status;

        // Initialize and set thread joinable
        pthread_attr_init(&attr);
        pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_JOINABLE);

        for (auto const& worker : WorkerList) {
          td[thID].threadID = thID;
          td[thID].workerID = worker.workerID;
          td[thID].workerIP = worker.workerIP;
          td[thID].workerPort = worker.workerPort;
          td[thID].miner = block.miner;
          td[thID].number = block.number;

          Logger::instance().log(MSG+" Block "+to_string(block.number)+" thread " + to_string(thID) +" starts", Logger::kLogLevelInfo); 
          rc = pthread_create(&threads[thID], &attr, connectWorker, (void *)&td[thID]);
          
          if (rc) {
             exit(-1);
          }
          thID = (thID+1) % NUM_THREADS;
        }
        
        // free attribute and wait for the other threads
        pthread_attr_destroy(&attr);
        
        for(int p = 0; p < NUM_THREADS; p++ ) {
          rc = pthread_join(threads[p], &status);
          if (rc) {
             exit(-1);
          }
          Logger::instance().log(MSG+" Block "+to_string(block.number)+" thread " + to_string(td[thID].threadID) +" ends", Logger::kLogLevelInfo);
        }
                                                                                                                                                                                         
      }

      Logger::instance().log("Block " + to_string(block.number) + " Uncles Execution starts", Logger::kLogLevelInfo);
        
      //cout << data[i_str]
      //cout << "UnclesList"
      for (auto& uncle: block.unclesList) {
        int ubn = uncle.number;
        //cout << ubn << endl;
        DataItemsMap[uncle.miner] = DataItemsMap[uncle.miner] + ((ubn+8-block.number) * base_reward)/8;
        uncle_reward += (base_reward/32);
        //cout << block.number << "\t" << base_reward << endl;
        //cout << "uncle: " << ubn << " reward:" << uncle_reward << "," << (block.number-ubn) << " ," << ((ubn+8-block.number) * base_reward)/8 << endl;
      }
      Logger::instance().log("Block " + to_string(block.number) + " Uncles Execution ends", Logger::kLogLevelInfo);

      //cout << "Adding block reward" << endl;

      DataItemsMap[block.miner] = DataItemsMap[block.miner] + (base_reward + uncle_reward);

      auto end2 = chrono::steady_clock::now();
      outfile << chrono::duration_cast<chrono::microseconds>(end2 - end1).count() << "\t";

      /*
      // Mining starts

      Logger::instance().log(MSG+" Block " + to_string(block.number) + " Block Mining starts", Logger::kLogLevelInfo);

      minerStatus = false;
      for (auto const& worker : WorkerList) {
        std::shared_ptr<TTransport> socket(new TSocket(worker.workerIP, worker.workerPort));
        std::shared_ptr<TTransport> transport(new TBufferedTransport(socket));
        std::shared_ptr<TProtocol> protocol(new TBinaryProtocol(transport));
        WorkerServiceClient workerClient(protocol);

        Logger::instance().log(MSG+" Block "+to_string(block.number) +" connection to worker "+to_string(worker.workerID)+" starts", Logger::kLogLevelInfo);
        transport->open();

        Logger::instance().log(MSG+" Block "+to_string(block.number) +" WorkerID "+to_string(worker.workerID)+" mineBlock() starts", Logger::kLogLevelInfo);
        //printf("Sending transactionsList and LocalDataItemsMap to worker nodes\n");


        workerClient.mineBlock(block,block.nonce,NUM_WORKERS); // returns local worker response

        Logger::instance().log(MSG+" Block "+to_string(block.number) +" WorkerID "+to_string(worker.workerID)+" mineBlock() ends", Logger::kLogLevelInfo);
        
        transport->close();
        Logger::instance().log(MSG+" Block "+to_string(block.number) +" connection to worker "+to_string(worker.workerID)+" ends", Logger::kLogLevelInfo);
        //Logger::instance().log(MSG+" Block "+to_string(newBlock.number) +" thread "+ to_string(worker.workerID) +" ends", Logger::kLogLevelInfo);

      }

      //cout << "waiting starts" <<  endl;

      cout << endl;
      while (!minerStatus) {
        cout << minerStatus;
        //bool status = minerStatus;
        //sleep(1);
        std::this_thread::sleep_for(std::chrono::milliseconds(1));
      }
      cout << endl;

      //cout << endl << "waiting ends" << endl;

      
      Logger::instance().log(MSG+" Block " + to_string(block.number) + " Block Mining ends", Logger::kLogLevelInfo);

      auto end3 = chrono::steady_clock::now();
      outfile << chrono::duration_cast<chrono::microseconds>(end3 - end2).count() << "\t";

      */
      Logger::instance().log(MSG+" Block "+to_string(block.number)+" clear_memory starts", Logger::kLogLevelInfo);
      clear_memory(); 
      Logger::instance().log(MSG+" Block "+to_string(block.number)+" clear_memory ends", Logger::kLogLevelInfo);

      auto end4 = chrono::steady_clock::now();
      outfile << chrono::duration_cast<chrono::microseconds>(end4 - start).count() << "\n";
      outfile.close();  
    }
    
      
  }


  void recvMiningStatus(const int64_t nonce, const int32_t number) {
    // Your implementation goes here

    Logger::instance().log("Block " + to_string(block.number) + " recvMiningStatus starts ", Logger::kLogLevelInfo);
    //cout << "\n" << newBlock.number << "\t" << number << "\t" << minerStatus << endl;

    if (block.number == number && !minerStatus) {
      block.nonce = nonce;
      
      string block_str = convert_block_to_string(block);
      block_hash = sha256(block_str);
      //cout << nonce << "\t" << block_hash << endl;

      minerStatus = true;
    }
    
    Logger::instance().log("Block " + to_string(block.number) + " recvMiningStatus ends ", Logger::kLogLevelInfo);
    
    //printf("recvMiningStatus\n");
  }


};

int main(int argc, char **argv) {
  //srand(RANDOM_SEED);
  signal(SIGSEGV, signal_handler);
  Logger::instance().log(MSG+" starts", Logger::kLogLevelInfo);
  
  int port = atoi(argv[1]);
  NUM_THREADS = atoi(argv[2]);
  filename = argv[3];
  NUM_WORKERS = NUM_THREADS;

  ::apache::thrift::stdcxx::shared_ptr<MasterServiceHandler> handler(new MasterServiceHandler());
  ::apache::thrift::stdcxx::shared_ptr<TProcessor> processor(new MasterServiceProcessor(handler));
  ::apache::thrift::stdcxx::shared_ptr<TServerTransport> serverTransport(new TServerSocket(port));
  ::apache::thrift::stdcxx::shared_ptr<TTransportFactory> transportFactory(new TBufferedTransportFactory());
  ::apache::thrift::stdcxx::shared_ptr<TProtocolFactory> protocolFactory(new TBinaryProtocolFactory());
  

  //shared_ptr<UserStorageHandler> handler(new UserStorageHandler());
  //shared_ptr<MasterServiceHandler> handler(new MasterServiceHandler());
  //shared_ptr<TProcessor> processor(new MasterServiceProcessor(handler));
  //::apache::thrift::stdcxx::shared_ptr<TServerTransport> serverTransport(new TServerSocket(port));
  //shared_ptr<TTransportFactory> transportFactory(new TBufferedTransportFactory());
  //shared_ptr<TNonblockingServerSocket> nbServerTransport(new TNonblockingServerSocket(port));
  //shared_ptr<TProtocolFactory> protocolFactory(new TBinaryProtocolFactory());
  
  // using thread pool with maximum 15 threads to handle incoming requests
  shared_ptr<ThreadManager> threadManager = ThreadManager::newSimpleThreadManager(5);
  shared_ptr<PosixThreadFactory> threadFactory = shared_ptr<PosixThreadFactory>(new PosixThreadFactory());
  threadManager->threadFactory(threadFactory);
  threadManager->start();
  
  Logger::instance().log(MSG+" creating TThreadPoolServer connection", Logger::kLogLevelInfo);
  //TNonblockingServer server(processor, protocolFactory, nbServerTransport, threadManager);
  
  //TSimpleServer 
  TThreadPoolServer server(processor, serverTransport, transportFactory, protocolFactory, threadManager);
  Logger::instance().log(MSG+" connection established", Logger::kLogLevelInfo);
  
  Logger::instance().log(MSG+" service starts", Logger::kLogLevelInfo);
  server.serve();
  Logger::instance().log(MSG+" service ends", Logger::kLogLevelInfo);
  return 0;
}
