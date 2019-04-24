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
	int16_t transactionID;
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

//std::vector<Block> blockchain;
std::vector<string> contract_addresses;
/*
bool verifyECDSA(string pubkey, string hash,string r,string s,int8_t v) {
	cout << hash << endl;
	cout << r << endl;
	cout << s << endl;
	cout << v << endl;
	return true

}*/

string MSG="Serial";
// string = 34333131353030636165623163363738613765326135633832326335343235623964633533323864653364303365393864656663363732643932323237656132;

//string difficulty = "00011111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111";
//string difficulty = "000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000b01d242b7c6575ef7c593ec1204d6282000000";
//string difficulty = "0000000000000000000000000000000000000000000000000000000000000000000000000000265B3B61FA6B9E08CF1F72BB03D926D291E05380000000000000"

//string difficulty = "00111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111";

Block block;

// Clears memory of global data structures after every block creation
void clear_memory() {
  block.transactionsList.clear();
  //block.unclesList.clear();
}

/*

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
	str.append(block.timestamp);
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
		//str.append(to_string(tx.gas));
		//str.append(to_string(tx.gasPrice));
	}

	for (auto& u : block.unclesList) {
		str.append(u.miner);
		str.append(to_string(u.number));
	}

	return str;
	//str.append(block.transaction_count);
}

string mine(Block block) {

	//std::random_device rd;     //Get a random seed from the OS entropy device, or whatever
  	//std::mt19937_64 eng(rd()); //Use the 64-bit Mersenne Twister 19937 generator
                             //and seed it with entropy.

  	//Define the distribution, by default it goes from 0 to MAX(unsigned long long)
  	//or what have you.
  	//std::uniform_int_distribution<unsigned long long> distr;

	block.nonce = 0; //distr(eng);
	string hash;
	while (true) {
		string block_str = convert_block_to_string(block);
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


void saveDataItemsMap(string filename) {
	ofstream accountsFile(filename);
	for (std::map<string,double>::iterator i = DataItemsMap.begin(); i != DataItemsMap.end(); ++i)
	{
		accountsFile.write((i->first).c_str(),i->first.size());
		accountsFile.write("\t",1);
		accountsFile.write(to_string(i->second).c_str(), sizeof(to_string(i->second)));
		accountsFile.write("\n",1);
	}
}
*/

// create initial state of data items map
void genesis() {
	string line;
	ifstream accounts_file ("data/block16/accounts.json");
	json accounts;
	accounts_file >> accounts;

	// iterate the array
	for (json::iterator it = accounts.begin(); it != accounts.end(); ++it) {
		DataItemsMap[it.key()] = it.value();
		//cout << it.key() << " " << it.value() << endl;
	}
}

bool execute (Transaction transaction) {
	//int create_txs = 0;
	//int execute_txs = 0;

	bool status = false;
	string cmd = "";
	if (transaction.toAddress == "creates") {
		cmd = "./contract_erc20 " + transaction.creates + " " + transaction.fromAddress + " " + transaction.toAddress;
		//const char *command = cmd.c_str();
		//cout << command << endl;
		//status = system(command);
	} else {
		cmd = "./contract_erc20 " + transaction.toAddress + " " + transaction.fromAddress + " " + transaction.input;
		//const char *command = cmd.c_str();
		//cout << command << endl;
		//status = system(command);
		//cout << "execute transaction" << endl;
		//execute_txs++;
		//system()
	}
	const char *command = cmd.c_str();
	//cout << command << endl;
	status = system(command);
	
	return true;
	//cout << create_txs << "\t" << execute_txs << endl;
}

void createBlock(json::iterator data) {
  //for (auto& data: ethereum_data.items()) {
  
  //Block block;
  Logger::instance().log(MSG+" Block "+to_string(block.number)+" creation starts", Logger::kLogLevelInfo);
  block.number = stoi(data.key());//atoi(d.key().c_str());
  //block.timestamp = data.value()["timestamp"];
  //block.nonce = 0;
  //block.prevHash = "0000000000000000000000000000000000000000000000000000000000000000";
  //block.miner = data.value()["miner"];

  Logger::instance().log(MSG+" Block "+to_string(block.number)+" transactionsList starts", Logger::kLogLevelInfo);
  int16_t txid=0;
  for (auto& tx: data.value()["transactions"]) {
    Transaction transaction;
    transaction.transactionID = txid++;
    //cout << endl;
    //cout << tx["from"] << " ";
    //cout << tx["to"] << " ";
    //cout << tx["value"] << " ";
    //cout << tx["creates"] << " ";

    transaction.fromAddress = tx["from"];
    if (tx["to"].is_null()) {
      transaction.toAddress = "creates";
    } else {
      transaction.toAddress = tx["to"];
    }

    if (tx["value"].is_number()) {
      transaction.value = tx["value"].get<double>();
      //cout << transaction.value;
    } else {
      transaction.value = stod(tx["value"].get<string>());
    }
    //transaction.toAddress = tx["to"];
    //transaction.value = (double)tx["value"];
    if (tx["input"].is_null()) {
      transaction.input = ""; 	
    } else {
      cout << transaction.input << "\t";
      transaction.input = tx["input"];
    }
    if (tx["creates"].is_null()) {
      transaction.creates = "";
      //contract_addresses.push_back(transaction.creates);
    } else {
      transaction.creates = tx["creates"];
    }
    //transaction.creates = tx["creates"]; 
    block.transactionsList.push_back(transaction);
  }
  Logger::instance().log(MSG+" Block "+to_string(block.number)+" transactionsList ends", Logger::kLogLevelInfo);
  /*
  Logger::instance().log(MSG+" Block "+to_string(block.number)+" unclesList starts", Logger::kLogLevelInfo);
  for (auto& u: data.value()["unclesList"]) {
    Uncle uncle;
    uncle.miner = u["miner"];
    uncle.number = u["number"];
    block.unclesList.push_back(uncle);
  }
  Logger::instance().log(MSG+" Block "+to_string(block.number)+" unclesList ends", Logger::kLogLevelInfo);
  */
  Logger::instance().log(MSG+" Block "+to_string(block.number)+" creation ends", Logger::kLogLevelInfo);
}



int main(int argc, char const *argv[])
{
	
	genesis();
	
	//saveDataItemsMap("logs/accounts/genesis.csv");
	json ethereum_data;
	
	std::ifstream ethereum_data_file(argv[1]); 
  	ethereum_data_file >> ethereum_data;
  	
  	Logger::instance().log("Serial Execution starts", Logger::kLogLevelInfo);
  	
  	int64_t total_transactions = 0;
  	int64_t successful_transactions = 0;
  	int64_t failed_transactions = 0;

  	ofstream nttfile;
    nttfile.open("data/serial/be_ntt.csv",std::ofstream::out | std::ofstream::app);
    ofstream cttfile;
    cttfile.open("data/serial/be_ctt.csv",std::ofstream::out | std::ofstream::app);

    cout << ethereum_data.size() << endl;

  	for (json::iterator data = ethereum_data.begin(); data != ethereum_data.end(); ++data){

  		auto start = chrono::steady_clock::now();
  		// running for originial ethrereum difficulty
	    //if (stoi(data.key()) >= 4370100) break;

	    cout << data.key() << "\t";
	    //cout << data.value()["transactions"].size() << "\t"; //<< data.value()["unclesList"].size() << "\t";//<< "\t" << data[i_str]["transactions"].size();

	    createBlock(data);
	    
	    auto end1 = chrono::steady_clock::now();
    	cout << chrono::duration_cast<chrono::microseconds>(end1 - start).count() << "\t";
    	
    	//double base_reward = 3000000000000000000;
	    //double tx_fees = 0;
	    //double uncle_reward = 0;
	    //double uncle_miner_reward = 0;

	    double normal_txn_time = 0;
    	double contract_txn_time = 0;

    	if (block.transactionsList.size() > 0) {
		    for (auto const& tx: block.transactionsList) {
		      //cout << tx.transactionID << "\t";
		      auto txn_start = chrono::steady_clock::now();
	  		  auto txn_end = chrono::steady_clock::now();
		      double fee;


		      bool status= false;
		      if (tx.creates != "") {
		      	//cout << "creates " << tx.creates << endl;
		        //transaction.creates = tx["creates"];
		        contract_addresses.push_back(tx.creates);
		        status = execute(tx);
		        txn_end = chrono::steady_clock::now();
	        	contract_txn_time += chrono::duration_cast<chrono::milliseconds>(txn_end - txn_start).count();
		      } else {
		        //cout << "checking toAddress wiht contract_addresses created so far" << endl;
		        //cout << tx.toAddress << endl;
		        std::vector<string>::iterator it = std::find (contract_addresses.begin(), contract_addresses.end(), tx.toAddress); 
		        //cout << contract_addresses.size() << endl;
		        if (it != contract_addresses.end()) { 
		            //cout << tx.toAddress << endl;
		            //transaction.creates = "";
		            //std::cout << "Element " << ser <<" found at position : " ; 
		            //std:: cout << it - vec.begin() + 1 << "\n" ; 
		            status = execute(tx);
		            txn_end = chrono::steady_clock::now();
	            	contract_txn_time += chrono::duration_cast<chrono::milliseconds>(txn_end - txn_start).count();
		        } 
		        else {
		        	//cout << "reached end" << endl;
		            //std::cout << "Element not found.\n\n"; 
		            //cout << "financial transaction" << endl;
		          if (DataItemsMap[tx.fromAddress] >= double(tx.value))
		          {
		            //fee = (double)tx["gasPrice"] * (double)tx["gas"];
		            DataItemsMap[tx.fromAddress] = DataItemsMap[tx.fromAddress] - double(tx.value) - fee;
		            //cout << "hello";
		            //if (tx.toAddress == "") {
		              //cout << "to" <<endl;
		              //DataItemsMap[tx["to"]] = double(tx["value"]);
		            //} else {
		            DataItemsMap[tx.toAddress] = DataItemsMap[tx.toAddress] + double(tx.value);
		            //}
		            status = true;
		            //successful_transactions++;
		            txn_end = chrono::steady_clock::now();
	            	normal_txn_time += chrono::duration_cast<chrono::microseconds>(txn_end - txn_start).count();
		          } else {
		            //failed_transactions++;
		            status = false;
		          }
		        }
		      }
		      
		      if (status) {
		        successful_transactions++;
		      } else {
		        failed_transactions++;
		      }
		      //tx_fees += fee;
		      //_return.transactionIDList.push_back(tx.transactionID);
		      total_transactions++;
		    }
		if (contract_txn_time > 0)
			cttfile << contract_txn_time << endl;

	    nttfile << normal_txn_time << endl;
	    //cttfile << contract_txn_time << endl;
		}	

		auto end2 = chrono::steady_clock::now();
    	cout << chrono::duration_cast<chrono::microseconds>(end2 - end1).count() << "\t";
	    /*
	    Logger::instance().log("Block " + data.key() + " Block Execution starts", Logger::kLogLevelInfo);
	    Block block;
	    block.timestamp = data.value()["timestamp"];
	    block.number = stoi(data.key());//atoi(d.key().c_str());
	    block.miner = data.value()["miner"];

	    

	    total_transactions += data.value()["transactions"].size(); //block.transaction_count;

	    double base_reward = 3000000000000000000;
	    double tx_fees = 0;
	    double uncle_reward = 0;
	    double uncle_miner_reward = 0;

	    Logger::instance().log("Block " + data.key() + " Transactions Execution starts", Logger::kLogLevelInfo);
	    
	    int16_t txid=0;
		for (auto& tx : data.value()["transactions"]) {

        	double fee = 0;

        	//cout << endl;
        	//cout << txid << "\t";
        	//cout << tx["from"] << "\t";
        	//cout << tx["to"] << "\t";
        	//cout << tx["value"]<< "\t" ;
        	//cout << tx["input"] << "\t";
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
      		transaction.input = tx["input"];
      		//transaction.gas = (double)tx["gas"];
      		//transaction.gasPrice = (double)tx["gasPrice"]; 
      		block.transactionsList.push_back(transaction);
      		bool status= false;
      		if (!tx["creates"].is_null()) {
      			transaction.creates = tx["creates"];
      			contract_addresses.push_back(transaction.creates);
      			status = execute(transaction);
      			if (status) {
      				successful_transactions++;
      			} else {
      				failed_transactions++;
      			}
      		} else {
      			//cout << "checking toAddress wiht contract_addresses created so far" << endl;

      			std::vector<string>::iterator it = std::find (contract_addresses.begin(), contract_addresses.end(), transaction.creates); 
			    if (it != contract_addresses.end()) { 
			    	transaction.creates = "call";
			        //std::cout << "Element " << ser <<" found at position : " ; 
			        //std:: cout << it - vec.begin() + 1 << "\n" ; 
			        status = execute(transaction);
	      			if (status) {
	      				successful_transactions++;
	      			} else {
	      				failed_transactions++;
	      			}
			    } 
			    else {
			        //std::cout << "Element not found.\n\n"; 
	      			//cout << "financial transaction" << endl;
	      			if (DataItemsMap[tx["from"]] >= double(tx["value"]))
		        	{
		        		//fee = (double)tx["gasPrice"] * (double)tx["gas"];
		        		DataItemsMap[tx["from"]] = DataItemsMap[tx["from"]] - double(tx["value"]) - fee;
		        		//cout << "hello";
		        		if (tx["to"].is_null()) {
		        			//cout << "to" <<endl;
		        			DataItemsMap[tx["to"]] = double(tx["value"]);
		        		} else {
		        			DataItemsMap[tx["to"]] = DataItemsMap[tx["to"]] + double(tx["value"]);
		        		}
		        		status = true;
		        		successful_transactions++;
		        	} else {
		        		failed_transactions++;
		        	}
		        }
      		}
      		
      		//cout << status << "\t";
      		
        	tx_fees += fee;
        }
        Logger::instance().log("Block " + data.key() + " Transactions Execution ends", Logger::kLogLevelInfo);

    	Logger::instance().log("Block " + data.key() + " Uncles Execution starts", Logger::kLogLevelInfo);
	    
        for (auto& uncle: data.value()["unclesList"]) {
        	
        	Uncle u;
      		u.miner = uncle["miner"];
      		u.number = uncle["number"];
      		block.unclesList.push_back(u);

        	int ubn = uncle["number"];

        	DataItemsMap[uncle["miner"]] = DataItemsMap[uncle["miner"]] + ((ubn+8-block.number) * base_reward)/8;
        	uncle_reward += (base_reward/32);
        }
        Logger::instance().log("Block " + data.key() + " Uncles Execution ends", Logger::kLogLevelInfo);
	    */

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

    	/*
    	Logger::instance().log("Block " + data.key() + " Block Mining starts", Logger::kLogLevelInfo);

    	string block_hash = "0000000000000000000000000000000000000000000000000000000000000000";
    	block.prevHash = block_hash;
    	block_hash = mine(block);
    	//blockchain.push_back(block);

    	Logger::instance().log("Block " + data.key() + " Block Mining ends", Logger::kLogLevelInfo);
    	auto end3 = chrono::steady_clock::now();
    	cout << chrono::duration_cast<chrono::microseconds>(end3 - end2).count() << "\t";
		*/
    	Logger::instance().log("Block " + data.key() + " Block Execution ends", Logger::kLogLevelInfo);

    	Logger::instance().log(MSG+" Block "+to_string(block.number)+" clear_memory starts", Logger::kLogLevelInfo);
		clear_memory(); 
		Logger::instance().log(MSG+" Block "+to_string(block.number)+" clear_memory ends", Logger::kLogLevelInfo);

		auto end4 = chrono::steady_clock::now();
		cout << chrono::duration_cast<chrono::microseconds>(end4 - start).count() << "\n";

	}

	cout << endl << total_transactions << endl;
	Logger::instance().log("Serial Execution ends", Logger::kLogLevelInfo);
	
	return 0;
}
