#include <iostream>
#include <string>
#include "nlohmann/json.hpp"
#include <fstream>
#include <cstdlib>
#include <vector>
#include <functional> //for std::function
#include <algorithm>  //for std::generate_n
#include "Logger.h"

using json = nlohmann::json;

using namespace std;


struct Uncle {
	string address;
	int64_t number;
};

struct Transaction
{
	string to_address;
	string from_address;
	double value;
};

struct Block
{
	string timestamp;
	int64_t number;
	string miner;
	int16_t transaction_count;
	std::vector<Transaction> transaction_list;	
};



std::map<string, double> DataItemsMap;


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


using json = nlohmann::json;

int main(int argc, char const *argv[])
{
	//double t;

	std::vector<Transaction> TransactionList;
	//std::map<string, double> DataItemsMap;

	//int start = stoi(argv[2]);
	//int end = stoi(argv[3]);

	//cout << "Creating genesis state of data items.\n";
	genesis();
	//cout << "Created genesis state of data items.\n";

	//cout << "Saving genesis state starts.\n";
	saveDataItemsMap("logs/accounts/genesis.csv");
	//cout << "Saving genesis state ends.\n";

	//read data from file and store in memory
	json ethereum_data;
	//"data/ethereum_data_" +  + ".json"
	//cout << "reading ethereum_data_file starts.\n";
  	std::ifstream ethereum_data_file(argv[1]); 

  	ethereum_data_file >> ethereum_data;
  	//cout << "reading ethereum_data_file ends.\n";
	
  	Logger::instance().log("Serial Execution starts", Logger::kLogLevelInfo);
  	int64_t total_transactions = 0;
  	int64_t successful_transactions = 0;
  	int64_t failed_transactions = 0;



  	for (auto& data: ethereum_data.items()) {
  		auto start = chrono::steady_clock::now();
  		clock_t t = clock();
  		double elapsed_time;
	    //string i_str = to_string(i);
	    cout << data.key() << "\t" << data.value()["transactions"].size() << "\t" << data.value()["unclesList"].size() << "\t";//<< "\t" << data[i_str]["transactions"].size();
	    Logger::instance().log("Block " + data.key() + " Block Execution starts", Logger::kLogLevelInfo);
	    //cout << i;
	    //std::cout << i_str << "\t" <<data[i_str] << '\n';
	    Block block;
	    //cout << data["0"]["timestamp"];
	    block.timestamp = data.value()["timestamp"];
	    block.number = stoi(data.key());//atoi(d.key().c_str());
	    block.miner = data.value()["miner"];
	    //block.gasLimit = data[i_str]["gasLimit"]
	    //block.gasUsed = data[i_str]["gasUsed"]
	    //block.transaction_count = data[i_str]["transaction_count"];
	    
	    total_transactions += data.value()["transactions"].size(); //block.transaction_count;

	    double base_reward = 3000000000000000000;
	    double tx_fees = 0;
	    double uncle_reward = 0;
	    double uncle_miner_reward = 0;

	    //std::vector<Transaction> txnList;
	    Logger::instance().log("Block " + data.key() + " Transactions Execution starts", Logger::kLogLevelInfo);
	    
		for (auto& tx : data.value()["transactions"]) {
        	double fee;
        	//cout << "from" << tx["from"] << endl;
        	//cout << "to" << tx["to"] << endl;
        	
        	// verify transaction


        	if (DataItemsMap[tx["from"]] >= double(tx["value"]))
        	{
        		fee = (double)tx["gasPrice"] * (double)tx["gas"];
        		DataItemsMap[tx["from"]] = DataItemsMap[tx["from"]] - double(tx["value"]) - fee;
        		//cout << "hello";
        		if (tx["to"].is_null()) {
        			//cout << "to" <<endl;
        			DataItemsMap[tx["to"]] = double(tx["value"]);
        		} else {
        			DataItemsMap[tx["to"]] = DataItemsMap[tx["to"]] + double(tx["value"]);
        		}
        		
        		//cout << "successful_transactions" << endl;
        		//cout << "from_address: " << tx["from_address"] << DataItemsMap[tx["from_address"]] << endl; 
        		//cout << "to_address: " << tx["to_address"] << DataItemsMap[tx["to_address"]] << endl; 
        		successful_transactions++;
        	} else {
        		//cout << "Block:"+i_str << endl;
        		//cout << "Transaction: " <<DataItemsMap[tx["from"]] << "\t" << ((double)tx["value"]) << endl;
        		//cout << "Transaction Failed!!! Insufficient Balance" << endl;
        		failed_transactions++;
        	}
        	tx_fees += fee;
        }
        Logger::instance().log("Block " + data.key() + " Transactions Execution ends", Logger::kLogLevelInfo);
	    
        /*
	    if (data[i_str]["transactions"].size() > 0) {
			// send transaction list
			//sleep(2);
			
    	}*/
    	Logger::instance().log("Block " + data.key() + " Uncles Execution starts", Logger::kLogLevelInfo);
	    
    	//cout << data[i_str]
    	//cout << "UnclesList"
        for (auto& uncle: data.value()["unclesList"]) {
        	int ubn = uncle["number"];
        	//cout << ubn << endl;
        	DataItemsMap[uncle["miner"]] = DataItemsMap[uncle["miner"]] + ((ubn+8-block.number) * base_reward)/8;
        	uncle_reward += (base_reward/32);
        	//cout << i_str << "\t" << base_reward << endl;
        	//cout << "uncle: " << ubn << " reward:" << uncle_reward << "," << (i-ubn) << " ," << ((ubn+8-i) * base_reward)/8 << endl;
        }
        Logger::instance().log("Block " + data.key() + " Uncles Execution ends", Logger::kLogLevelInfo);
	    

    	DataItemsMap[block.miner] = DataItemsMap[block.miner] + (base_reward + tx_fees + uncle_reward);
    	auto end = chrono::steady_clock::now();
    	//elapsed_time = (double) (clock() - t);
    	//cout << elapsed_time/CLOCKS_PER_SEC << endl;
    	cout << chrono::duration_cast<chrono::microseconds>(end - start).count() << endl;
    	//cout << i_str << "\t" << block.miner << "\t" << DataItemsMap[block.miner] << endl << endl;
    	//Logger::instance().log("Block " + data.key() + " saving DataItemsMap starts", Logger::kLogLevelInfo);
		//saveDataItemsMap("logs/accounts/block_"+data.key()+".csv");
		//Logger::instance().log("Block " + data.key() + " saving DataItemsMap ends", Logger::kLogLevelInfo);
    	Logger::instance().log("Block " + data.key() + " Block Execution ends", Logger::kLogLevelInfo);


	}



	//cout << "\nTotal: " << total_transactions << endl;
	//cout << "Success: " << successful_transactions << endl;
	//cout << "Failed: " << failed_transactions << endl;
	//cout << "Base Reward: " << base_reward << endl;
	Logger::instance().log("Serial Execution ends", Logger::kLogLevelInfo);
	
	return 0;
}
