// This autogenerated skeleton file illustrates how to build a server.
// You should copy it to another filename to avoid overwriting it.

#include "gen-cpp/MasterService.h"
#include "gen-cpp/WorkerService.h"
#include "gen-cpp/SharedService.h"

#include "nlohmann/json.hpp"

#include <thrift/server/TNonblockingServer.h>


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
#include <thrift/transport/TNonblockingServerTransport.h>
#include <thrift/transport/TNonblockingServerSocket.h>
#include <thrift/TToString.h>

#include <iostream>
#include <string>
#include <pthread.h>
#include <cstdlib>
#include <vector>
#include <random>
#include <functional>
#include <algorithm>  
#include "Logger.h"


#define NUM_WORKERS 1
#define NUM_THREADS 1
#define NUM_ACCOUNTS 50
#define NUM_TRANSACTIONS 10
#define RANDOM_SEED 10



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

vector<WorkerNode> WorkerList;

//vector<Transaction> TransactionList;
//vector<Account> AccountList;



map<string,double> DataItemsMap;
map<int16_t, map<set<string>,set<int16_t>>> GlobalConflictsMap; // map<list<addresses>, list<txid>>
map<string,list<int16_t>> LocalConflictsMap; // map<address,list<txid>>


map<int16_t,std::vector<WorkerResponse>> GlobalWorkerResponsesList;


map<string,list<string>> AdjacencyMap;

const string MSG="MasterServer";
const double base_reward = 3000000000000000000;

struct thread_data {
   int16_t  threadID;
   int16_t workerID;
   string workerIP;
   int32_t workerPort;
   string miner;
};


map< string,double> LocalDataItemsMap;
//map< int16_t,map<string,map<double,vector<int16_t>>>> GlobalDataItemsMap;
map<int16_t,vector<Transaction>> sendTransactionMap;


//map<int16_t, set<string>> ccAddressMap;
map<int16_t, set<int16_t>> ccTransactionMap;


void clear_memory() {
  //cout << "Before clear size " <<LocalDataItemsMap.size() << endl;
  LocalDataItemsMap.clear();
  //cout << "After clear size " <<LocalDataItemsMap.size() << endl;
  sendTransactionMap.clear();
  ccTransactionMap.clear();
  GlobalConflictsMap.clear();
  LocalConflictsMap.clear();
  //LocalWorkerResponse.accountList.clear();
  //LocalWorkerResponse.transactionIDList.clear();
  GlobalWorkerResponsesList.clear();
  //AddressList.clear();
  AdjacencyMap.clear();
}

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

// use list<Transaction>
void analyze(vector<Transaction> TransactionList) {
  std::set<string> AddressList;

  Logger::instance().log(MSG+" AdjacencyMap starts", Logger::kLogLevelInfo);
  for (auto const& tx: TransactionList) {
    LocalConflictsMap[tx.fromAddress].push_back(tx.transactionID);
    LocalConflictsMap[tx.toAddress].push_back(tx.transactionID);
    AdjacencyMap[tx.fromAddress].push_back(tx.toAddress);
    AdjacencyMap[tx.toAddress].push_back(tx.fromAddress);
    
    AddressList.insert(tx.fromAddress);
    AddressList.insert(tx.toAddress);
  }
  Logger::instance().log(MSG+" AdjacencyMap ends", Logger::kLogLevelInfo);

  map<string,bool> visited;

  // Mark all vertices as not visited
  for (auto const& address: AddressList) {
    visited[address] = false;
  }

  int ccID = 1;
  int id = 0;

  for (auto const& address: AddressList) {
    if (visited[address] == false) {
      // print all reachable vertices
      //cout << address << endl;
      Logger::instance().log(MSG+" DFSUtil starts", Logger::kLogLevelInfo);  
      DFSUtil(ccID,address,visited);
      Logger::instance().log(MSG+" DFSUtil ends", Logger::kLogLevelInfo);
      //cout << ccID << "\t";
      
      if (ccTransactionMap[ccID].size() > 0) {
        for (auto const& txid: ccTransactionMap[ccID]) {
          //cout << TransactionList[txid].transactionID << "\t";
          sendTransactionMap[WorkerList[id].workerID].push_back(TransactionList[txid]);
        }
        ccID++;
        id = (id+1)%NUM_WORKERS;
        //cout << endl;
      }

    }
  }
}


void *connectWorker (void *threadarg) {
  struct thread_data *worker;
  worker = (struct thread_data *) threadarg;

  cout << "Thread ID : " << worker->threadID << endl ;
  //cout << " Message : " << my_data->message << endl;
  Logger::instance().log(MSG+" thread "+ to_string(worker->workerID) +" starts", Logger::kLogLevelInfo);
      

  std::shared_ptr<TTransport> socket(new TSocket(worker->workerIP, worker->workerPort));
  std::shared_ptr<TTransport> transport(new TBufferedTransport(socket));
  std::shared_ptr<TProtocol> protocol(new TBinaryProtocol(transport));
  WorkerServiceClient workerClient(protocol);

  Logger::instance().log(MSG+" connection to worker "+to_string(worker->workerID)+" starts", Logger::kLogLevelInfo);
  transport->open();

  Logger::instance().log(MSG+" LocalDataItemsMap generation for worker "+to_string(worker->workerID)+" starts", Logger::kLogLevelInfo);
  cout << "Size: " << sendTransactionMap[worker->workerID].size() << endl;
  for (auto& tx: sendTransactionMap[worker->workerID]) {
    LocalDataItemsMap[tx.fromAddress] = DataItemsMap[tx.fromAddress];
    LocalDataItemsMap[tx.toAddress] = DataItemsMap[tx.toAddress];
  }
  Logger::instance().log(MSG+" LocalDataItemsMap generation for worker "+to_string(worker->workerID)+" ends", Logger::kLogLevelInfo);
  

  //LocalDataItemsMap.clear();
  Logger::instance().log(MSG+" "+to_string(worker->workerID)+" recvTransactions() starts", Logger::kLogLevelInfo);
  printf("Sending transactionsList and LocalDataItemsMap to worker nodes\n");
  WorkerResponse LocalWorkerResponse;    
  workerClient.recvTransactions(LocalWorkerResponse,sendTransactionMap[worker->workerID],LocalDataItemsMap); // returns local worker response
  Logger::instance().log(MSG+" "+to_string(worker->workerID)+" recvTransactions() ends", Logger::kLogLevelInfo);
  
  map<string,double>::iterator it;
  for (it = LocalWorkerResponse.accountList.begin(); it != LocalWorkerResponse.accountList.end(); ++it)
  {
    //cout << it->first << "\t" << it->second << endl;
    DataItemsMap[it->first] = it->second;
  }
  //cout << endl;
  for (auto const& i: LocalWorkerResponse.transactionIDList)
  {
    //cout << i << "\t";
  }
  //cout << endl;
  DataItemsMap[worker->miner] += LocalWorkerResponse.transactionFees; 

  //GlobalWorkerResponsesList[worker->workerID] = LocalWorkerResponse;
  //GlobalDataItemsMap[worker->workerID] = WorkerResponse;
  //cout <<  "GlobalDataItemsMap size: " << GlobalDataItemsMap[worker->workerID].size() << endl;
  printf("\nclosing transport at Master\n\n");
  //sleep(5);
  transport->close();
  Logger::instance().log(MSG+" connection to worker "+to_string(worker->workerID)+" ends", Logger::kLogLevelInfo);
  Logger::instance().log(MSG+" thread "+ to_string(worker->workerID) +" ends", Logger::kLogLevelInfo);
      
  //sleep(5);
  pthread_exit(&worker->threadID);
}

class MasterServiceHandler : virtual public MasterServiceIf {
 public:
  MasterServiceHandler() {
    // Your initialization goes here
    
    int port = 9091;
    string ip = "localhost";

    for (int id=1; id<=NUM_WORKERS; id++) {
      WorkerNode workerNode;
      workerNode.workerID = id;
      workerNode.workerIP = ip;
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
      //cout << it.key() << " " << it.value() << endl;
    }
    Logger::instance().log(MSG+" Initializing accounts with 100000000000000000000000 wei to execute transactions ends", Logger::kLogLevelInfo);
    
  }

  void processBlock(const Block& block) {
    // Your implementation goes here
    cout << endl;
    cout << "Block: " << block.number << "\t" << block.miner << endl;
    
    double uncle_reward = 0;

    //cout << block.miner << "\t" << DataItemsMap[block.miner] << endl;
    //cout << "Size: " << block.transactionsList.size() << endl;

    if (block.transactionsList.size() > 0) {
      cout << block.transactionsList.size() << endl;
      Logger::instance().log(MSG+" Block "+block.miner+" analyze starts", Logger::kLogLevelInfo);
      analyze(block.transactionsList);
      Logger::instance().log(MSG+" Block "+block.miner+" analyze ends", Logger::kLogLevelInfo);
      cout << endl;
      
      /*
      cout << "Printing sendTransactionMap" << endl;
      //cout << sendTransactionMap.size() << endl;
    
      std::map<int16_t, std::vector<Transaction>>::iterator itTxn;
      for (itTxn = sendTransactionMap.begin(); itTxn != sendTransactionMap.end(); ++itTxn) {
        cout << itTxn->first << " ";
        for (auto const& txn: itTxn->second) {
          cout << txn.transactionID << " ";
        }
        cout << endl;
      }
      */

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
        cout <<"sendTransactions() : creating thread, " << thID << endl;
        td[thID].threadID = thID;
        td[thID].workerID = worker.workerID;
        td[thID].workerIP = worker.workerIP;
        td[thID].workerPort = worker.workerPort;
        td[thID].miner = block.miner;
        Logger::instance().log(MSG+" Block "+to_string(block.number)+" thread " + to_string(thID) +" starts", Logger::kLogLevelInfo); 
        rc = pthread_create(&threads[thID], &attr, connectWorker, (void *)&td[thID]);
        
        if (rc) {
           cout << "Error:unable to create thread," << rc << endl;
           exit(-1);
        }
        thID = (thID+1) % NUM_THREADS;
      }
      
      // free attribute and wait for the other threads
      pthread_attr_destroy(&attr);
      
      for(int p = 0; p < NUM_THREADS; p++ ) {
        rc = pthread_join(threads[p], &status);
        if (rc) {
           cout << "Error:unable to join," << rc << endl;
           exit(-1);
        }
        
        cout << "Main: completed thread id :" << p ;
        cout << "  exiting with status :" << &status << endl;
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
      cout << "uncle: " << ubn << " reward:" << uncle_reward << "," << (block.number-ubn) << " ," << ((ubn+8-block.number) * base_reward)/8 << endl;
    }
    Logger::instance().log("Block " + to_string(block.number) + " Uncles Execution ends", Logger::kLogLevelInfo);

    cout << "Adding block reward" << endl;

    DataItemsMap[block.miner] = DataItemsMap[block.miner] + (base_reward + uncle_reward);

    printf("Block Processed\n");
    cout << "Clearing all global memories for next block creation" << endl;
    Logger::instance().log(MSG+" Block "+to_string(block.number)+" clear_memory starts", Logger::kLogLevelInfo);
    clear_memory(); 
    Logger::instance().log(MSG+" Block "+to_string(block.number)+" clear_memory ends", Logger::kLogLevelInfo);
  }
};

int main(int argc, char **argv) {
  //srand(RANDOM_SEED);
  Logger::instance().log(MSG+" starts", Logger::kLogLevelInfo);
  
  int port = atoi(argv[1]);
  
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
  //shared_ptr<ThreadManager> threadManager = ThreadManager::newSimpleThreadManager(15);
  //shared_ptr<PosixThreadFactory> threadFactory = shared_ptr<PosixThreadFactory>(new PosixThreadFactory());
  //threadManager->threadFactory(threadFactory);
  //threadManager->start();
  

  //Logger::instance().log(MSG+" Preprocessing starts", Logger::kLogLevelInfo);
  //Logger::instance().log(MSG+" accounts creation starts", Logger::kLogLevelInfo);
  //Logger::instance().log(MSG+" accounts creation ends", Logger::kLogLevelInfo);
  //Logger::instance().log(MSG+" transactions creation starts", Logger::kLogLevelInfo);
  //Logger::instance().log(MSG+" Transactions creation ends", Logger::kLogLevelInfo);

  Logger::instance().log(MSG+" creating simpleServer connection", Logger::kLogLevelInfo);
  //TNonblockingServer server(processor, protocolFactory, nbServerTransport, threadManager);
  
  TSimpleServer server(processor, serverTransport, transportFactory, protocolFactory);
  Logger::instance().log(MSG+" connection established", Logger::kLogLevelInfo);
  
  Logger::instance().log(MSG+" service starts", Logger::kLogLevelInfo);
  server.serve();
  Logger::instance().log(MSG+" service ends", Logger::kLogLevelInfo);
  return 0;
}