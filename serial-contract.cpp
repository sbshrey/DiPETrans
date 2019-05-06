#include <iostream>
#include <string>
#include "nlohmann/json.hpp"
#include <fstream>
#include <cstdlib>
#include <vector>
#include <functional> //for std::function
#include <algorithm>  //for std::generate_n
#include "Logger.h"
#include <random>
#include <typeinfo>


#include <openssl/sha.h>
#include <sstream>
#include <iomanip>

//#include <CkCrypt2.h>
//#include <CkPrivateKey.h>
//#include <CkPrng.h>
//#include <CkEcc.h>
//#include <CkPublicKey.h>


using json = nlohmann::json;

using namespace std;


/*struct Uncle {
	string miner;
	int64_t number;
};
*/

struct Transaction
{
	int64_t transactionID;
	string toAddress;
	string fromAddress;
	double value;
	string input;
	string creates;
	//double gas;
	//double gasPrice;
};

struct Block
{
	//string timestamp;
	//uint64_t nonce;
	//string prevHash;
	//string hash;
	int64_t number;
	//string miner;
	//int16_t transaction_count;
	std::vector<Transaction> transactionsList;
	//std::vector<Uncle> unclesList;	
};



std::map<string, double> DataItemsMap;

std::vector<string> contract_addresses;


string MSG="Serial";

Block block;


// Clears memory of global data structures after every block creation
void clear_memory() {
  block.transactionsList.clear();
}


// create initial state of data items map
void genesis() {
	string line;
	ifstream accounts_file ("data/block16/accounts.json");
	json accounts;
	accounts_file >> accounts;

	// iterate the array
	for (json::iterator it = accounts.begin(); it != accounts.end(); ++it) {
		DataItemsMap[it.key()] = it.value();
	}
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


void createBlock(json::iterator data) {


  Logger::instance().log(MSG+" Block "+to_string(block.number)+" creation starts", Logger::kLogLevelInfo);
  //block.number = index++; //stoi(data.key());

  Logger::instance().log(MSG+" Block "+to_string(block.number)+" transactionsList starts", Logger::kLogLevelInfo);
  int16_t txid=0;
  for (auto& tx: (*data)["transactions"]) {
    Transaction transaction;
    transaction.transactionID = txid++; //tx["txID"];
    transaction.fromAddress = tx["from"];
    transaction.toAddress = tx["to"];
    transaction.value = tx["value"].get<double>();
    transaction.input = tx["input"];
    transaction.creates = tx["creates"];

    block.transactionsList.push_back(transaction);
  }
  Logger::instance().log(MSG+" Block "+to_string(block.number)+" transactionsList ends", Logger::kLogLevelInfo);
  
  Logger::instance().log(MSG+" Block "+to_string(block.number)+" creation ends", Logger::kLogLevelInfo);
}



int main(int argc, char const *argv[])
{
	
	genesis();
	
	json ethereum_data;
	
	std::ifstream ethereum_data_file(argv[1]); 
  	ethereum_data_file >> ethereum_data;
  	
  	Logger::instance().log("Serial Execution starts", Logger::kLogLevelInfo);
  	

  	string dir_path = argv[2];

  	int64_t total_transactions = 0;
  	int64_t successful_transactions = 0;
  	int64_t failed_transactions = 0;

  	ofstream nttfile;
    nttfile.open(dir_path+"be_ntt.csv",std::ofstream::out | std::ofstream::app);
    ofstream cttfile;
    cttfile.open(dir_path+"be_ctt.csv",std::ofstream::out | std::ofstream::app);

    ofstream e2efile;
    e2efile.open(dir_path+"be_e2e.csv",std::ofstream::out | std::ofstream::app);

    int64_t index = 0;

  	for (json::iterator data = ethereum_data.begin(); data != ethereum_data.end(); ++data){
  		auto start = chrono::steady_clock::now();
  		//cout << typeid(data).name() << endl;
  		//cout << (*data)["transactions"].size() << "\t";// << data.value()["transactions"].size() << endl;
  		//break;
	    createBlock(data);
	    block.number = index++;

	    cout << block.number << "\t" << block.transactionsList.size() << endl;

	    double normal_txn_time = 0;
    	double contract_txn_time = 0;

    	int sc_cnt = 0;
    	int nsc_cnt = 0;

    	//cout << endl;
    	if (block.transactionsList.size() > 0) {
		    for (auto const& tx: block.transactionsList) {
		      //cout << tx.transactionID << "\t";
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
		          if (DataItemsMap[tx.fromAddress] >= double(tx.value))
		          {
		            DataItemsMap[tx.fromAddress] = DataItemsMap[tx.fromAddress] - double(tx.value) - fee;
		            DataItemsMap[tx.toAddress] = DataItemsMap[tx.toAddress] + double(tx.value);
		            status = true;
		          } else {
		            status = false;
		          }
		          txn_end = chrono::steady_clock::now();
		          normal_txn_time += chrono::duration_cast<chrono::microseconds>(txn_end - txn_start).count();
		          nsc_cnt++;
		        }
		      }
		      
		      if (status) {
		        successful_transactions++;
		      } else {
		        failed_transactions++;
		      }

		      total_transactions++;
		    }

		cout << "sc " << sc_cnt << "\tnsc " << nsc_cnt << endl;
		if (contract_txn_time > 0)
			cttfile << contract_txn_time << endl;
		if (normal_txn_time > 0)
	    	nttfile << normal_txn_time << endl;
		}	

    	//Logger::instance().log("Block " + data.key() + " Block Execution ends", Logger::kLogLevelInfo);

    	Logger::instance().log(MSG+" Block "+to_string(block.number)+" clear_memory starts", Logger::kLogLevelInfo);
		clear_memory(); 
		Logger::instance().log(MSG+" Block "+to_string(block.number)+" clear_memory ends", Logger::kLogLevelInfo);

		auto end4 = chrono::steady_clock::now();
		e2efile << chrono::duration_cast<chrono::microseconds>(end4 - start).count() << "\n";

	}

	Logger::instance().log("Serial Execution ends", Logger::kLogLevelInfo);
	
	return 0;
}
