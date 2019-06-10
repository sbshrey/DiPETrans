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

#include "contract.h"
#include "utils.h"

//#include <CkCrypt2.h>
//#include <CkPrivateKey.h>
//#include <CkPrng.h>
//#include <CkEcc.h>
//#include <CkPublicKey.h>

#include "gen-cpp/MasterService.h"
#include "gen-cpp/WorkerService.h"
#include "gen-cpp/SharedService.h"

using json = nlohmann::json;

using namespace std;


using namespace  ::MasterService;
using namespace  ::WorkerService;
using namespace  ::SharedService;


/*struct Uncle {
	string miner;
	int64_t number;
};
*/

/*
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

*/



map<string,DataItem> dataItemMap;

std::vector<string> contract_addresses;


string difficulty = "00011111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111";

string MSG="Serial";

Block block;

string prevBlockHash = "0000000000000000000000000000000000000000000000000000000000000000";

bool mining = false;


// Clears memory of global data structures after every block creation
void clear_memory() {
  block.transactionsList.clear();
}


// create initial state of data items map
void genesis() {
	string line;
	//ifstream accounts_file ("data/block16/accounts.json");
	ifstream accounts_file ("data/bigquery/addresses.json");
	json accounts;
	accounts_file >> accounts;
	cout << "accounts size " << accounts.size() << endl;
	// iterate the array
	for (json::iterator it = accounts.begin(); it != accounts.end(); ++it) {
		if (it.value().get<double>() != 0)
			dataItemMap[it.key()].value = it.value().get<double>();
	}
	cout << "optimized accounts size " << dataItemMap.size() << endl;
    
}


string convert_block_to_string(Block block) {
	/*string str = "";
	//str.append(block.timestamp);
	str.append(to_string(block.nonce));
	cout << str << endl;
	str.append(block.prevHash);
	cout << str << endl;
	str.append(to_string(block.number));
	cout << str << endl;
	for (auto& tx : block.transactionsList) {
		str.append(to_string(tx.transactionID));
		str.append(tx.toAddress);
		str.append(tx.fromAddress);
		str.append(to_string(int64_t(tx.value)));
		str.append(tx.input);
		str.append(tx.creates);
		//str.append(to_string(tx.gas));
		//str.append(to_string(tx.gasPrice));
	}
	cout << str << endl;
	*/

	stringstream ss;
	ss << to_string(block.number) << block.prevHash << to_string(block.nonce);
	for (auto& tx : block.transactionsList) {
		ss << to_string(tx.transactionID);
		ss << tx.toAddress;
		ss << tx.fromAddress;
		ss << tx.value;
		ss << tx.input;
		ss << tx.creates;
		//str.append(to_string(tx.gas));
		//str.append(to_string(tx.gasPrice));
	}
	return ss.str();
	//str.append(block.transaction_count);
}


string mine(Block block) {
	block.nonce = 0; 
	string hash;
	//cout << "mining " << endl;
	while (true) {
		string block_str = convert_block_to_string(block);
		//cout << "block string " << block_str << endl;
		hash = sha256(block_str); //sha256(sha256(block_str));
		if (hash.compare(difficulty) < 0) {
			//cout << block.nonce << "\t";
			break;
		}
	    block.nonce++;
	}
	//cout << block.nonce << "\t";
	return hash;
}


void createBlock(json::iterator data) {
  //Logger::instance().log(MSG+" Block "+to_string(block.number)+" creation starts", Logger::kLogLevelInfo);
  //block.number = index++;stoi(data.key());//atoi(d.key().c_str());
  
  //Logger::instance().log(MSG+" Block "+to_string(block.number)+" transactionsList starts", Logger::kLogLevelInfo);
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
  //Logger::instance().log(MSG+" Block "+to_string(block.number)+" transactionsList ends", Logger::kLogLevelInfo);

  //Logger::instance().log(MSG+" Block "+to_string(block.number)+" creation ends", Logger::kLogLevelInfo);
}



int main(int argc, char const *argv[])
{
	// initializing accounts 
	genesis();
	
	json ethereum_data;
	
	std::ifstream ethereum_data_file(argv[1]); 
  	ethereum_data_file >> ethereum_data;
  	
  	//Logger::instance().log("Serial Execution starts", Logger::kLogLevelInfo);
  	

  	string dir_path = argv[2];

  	int64_t total_transactions = 0;
  	int64_t successful_transactions = 0;
  	int64_t failed_transactions = 0;

  	ofstream nttfile;
    nttfile.open(dir_path+"be_ntt.csv",std::ofstream::out | std::ofstream::trunc);
    ofstream cttfile;
    cttfile.open(dir_path+"be_ctt.csv",std::ofstream::out | std::ofstream::trunc);

    ofstream e2efile;
    e2efile.open(dir_path+"be_e2e.csv",std::ofstream::out | std::ofstream::trunc);

    ofstream txnfile;
    txnfile.open(dir_path+"be_txn.csv",std::ofstream::out | std::ofstream::trunc);

    ofstream minefile;
    minefile.open(dir_path+"be_mine.csv",std::ofstream::out | std::ofstream::trunc);

    ofstream createfile;
    createfile.open(dir_path+"be_create.csv",std::ofstream::out | std::ofstream::trunc); 
    
    ofstream analyzefile;
    analyzefile.open(dir_path+"be_analyze.csv",std::ofstream::out | std::ofstream::trunc);

    //ofstream scfile;
    //scfile.open(dir_path+"sc_call.csv",std::ofstream::out | std::ofstream::app);

    int64_t index = 0;

    mining = stoi(argv[3]);
    cout << "Mining " << mining << endl;

  	for (json::iterator data = ethereum_data.begin(); data != ethereum_data.end(); ++data){
  		
  		//block.number = index++;

	    /*
	    ofstream prevState;
      	prevState.open("state/serial/block_"+to_string(block.number)+"_prev_state.csv",std::ofstream::out | std::ofstream::trunc);
      	for (auto it : dataItemMap)
		{
			for (auto b: it.second.balances) {
	          cout << b.first << ":" << b.second << endl;
	        }
			prevState << it.first << "," << it.second << endl; 
		}
      	prevState.close();
		*/

  		auto start = chrono::steady_clock::now();

  		block.number = index++;
  		block.prevHash = prevBlockHash;
  		block.nonce = 0;
  		//cout << mining << endl;
  		if (block.number > 10 and mining) break;
  		//cout << typeid(data).name() << endl;
  		//cout << (*data)["transactions"].size() << "\t";// << data.value()["transactions"].size() << endl;
  		//break;
	    createBlock(data);

	    auto end1 = chrono::steady_clock::now();
      	createfile << chrono::duration_cast<chrono::microseconds>(end1 - start).count() << "\n";
	    
	    //if (block.number >= 5) break;
	    
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
				if (tx.toAddress == "creates") {
					contract_addresses.push_back(tx.creates);
					DataItem localDataItem;
        			call_contract(&localDataItem, tx.creates, tx.fromAddress, tx.toAddress, tx.value);
        			dataItemMap[tx.creates] =  localDataItem;
        			txn_end = chrono::steady_clock::now();
					contract_txn_time += chrono::duration_cast<chrono::microseconds>(txn_end - txn_start).count();
					sc_cnt++;
				} else {
					bool flag = false;
			        std::vector<string>::iterator it = std::find (contract_addresses.begin(), contract_addresses.end(), tx.toAddress); 
			        if (it != contract_addresses.end()) {  
			          DataItem localDataItem = dataItemMap[tx.toAddress];
			          call_contract(&localDataItem, tx.toAddress, tx.fromAddress, tx.input, tx.value);
			          dataItemMap[tx.toAddress] =  localDataItem;
			          txn_end = chrono::steady_clock::now();
			          contract_txn_time += chrono::duration_cast<chrono::microseconds>(txn_end - txn_start).count();
			          sc_cnt++;
			          flag = true;
			        }

			        it = std::find (contract_addresses.begin(), contract_addresses.end(), tx.fromAddress); 
			        if (it != contract_addresses.end() and !flag) {  
			          DataItem localDataItem = dataItemMap[tx.fromAddress];
			          call_contract(&localDataItem, tx.fromAddress, tx.toAddress, tx.input, tx.value);
			          dataItemMap[tx.fromAddress] = localDataItem;
			          txn_end = chrono::steady_clock::now();
			          contract_txn_time += chrono::duration_cast<chrono::microseconds>(txn_end - txn_start).count();
			          sc_cnt++;
			          flag = true;
			        }

			        if (!flag) {
			          if (dataItemMap[tx.fromAddress].value >= double(tx.value)) {
			            dataItemMap[tx.fromAddress].value -=  (double(tx.value));
			            dataItemMap[tx.toAddress].value += double(tx.value);
			            status = true;  
			          } else {
			          	//cout << "nsc txn " << dataItemMap[tx.fromAddress].value << "\t" << tx.value << endl;
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

		auto end2 = chrono::steady_clock::now();
      	txnfile << chrono::duration_cast<chrono::microseconds>(end2 - start).count() << "\n";

		//cout << "success: " << successful_transactions << endl;
		//cout << "failed " << failed_transactions << endl;
    	//Logger::instance().log("Block " + data.key() + " Block Execution ends", Logger::kLogLevelInfo);

      	/*
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
    	DataItemsMap[block.miner] = DataItemsMap[block.miner] + (base_reward + tx_fees + uncle_reward);    	
		auto end2 = chrono::steady_clock::now();
    	cout << chrono::duration_cast<chrono::microseconds>(end2 - end1).count() << "\t";    
		*/

    	
    	if (mining) {
    		//Logger::instance().log("Block " + to_string(block.number) + " Block Mining starts", Logger::kLogLevelInfo);
	    	prevBlockHash = mine(block);
	    	//blockchain.push_back(block);
	    	//Logger::instance().log("Block " + to_string(block.number) + " Block Mining ends", Logger::kLogLevelInfo);
	    	auto end3 = chrono::steady_clock::now();
      		minefile << chrono::duration_cast<chrono::microseconds>(end3 - end2).count() << "\n";
    	}
    	

    	//Logger::instance().log(MSG+" Block "+to_string(block.number)+" clear_memory starts", Logger::kLogLevelInfo);
		clear_memory(); 
		//Logger::instance().log(MSG+" Block "+to_string(block.number)+" clear_memory ends", Logger::kLogLevelInfo);

		auto end4 = chrono::steady_clock::now();
		e2efile << chrono::duration_cast<chrono::microseconds>(end4 - start).count() << "\n";

		/*if (block.number % 100 == 0) {
			ofstream nextState;
			nextState.open("state/serial/block_"+to_string(block.number)+"_next_state.csv",std::ofstream::out | std::ofstream::trunc);
			for (auto it : dataItemMap)
			{
				nextState << it.first << "," << it.second.value << "," << it.second.owner << ","; 
				for (auto b: it.second.balances) {
				  nextState << b.first << "," << b.second << ",";
				}
				for (auto a: it.second.allowed) {
				  nextState << a.first << ","; //<< b.second << ",";
				  for (auto c: a.second) {
				    nextState << c.first << "," << c.second << ",";
				  }
				}
				for (auto v: it.second.votes) {
				  nextState << v.first << "," << v.second << ",";
				}
				for (auto tx: it.second.transactions) {
				  nextState << tx << ","; //<< v.second << ",";
				}
				for (auto p: it.second.playerRolls) {
				  nextState << p.first << "," << p.second << ",";
				}
				nextState <<endl;
			}
			nextState.close();
		}*/
		


	}
	nttfile.close();
	cttfile.close();
	e2efile.close();
	createfile.close();
	analyzefile.close();
	//scfile.close();

	//Logger::instance().log("Serial Execution ends", Logger::kLogLevelInfo);
	
	return 0;
}
