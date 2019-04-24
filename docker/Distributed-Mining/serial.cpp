#include <iostream>
#include <string>
#include "nlohmann/json.hpp"
#include <fstream>
#include <cstdlib>
#include <vector>
#include <functional> //for std::function
#include <algorithm>  //for std::generate_n
#include "Logger.h"


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


struct Uncle {
	string miner;
	int64_t number;
};

struct Transaction
{
	int16_t transactionID;
	string toAddress;
	string fromAddress;
	double value;
	double gas;
	double gasPrice;
};

struct Block
{
	string timestamp;
	int64_t nonce;
	string prevHash;
	//string hash;
	int64_t number;
	string miner;
	//int16_t transaction_count;
	std::vector<Transaction> transactionsList;
	std::vector<Uncle> unclesList;	
};



std::map<string, double> DataItemsMap;

std::vector<Block> blockchain;

/*
bool verifyECDSA(string pubkey, string hash,string r,string s,int8_t v) {
	cout << hash << endl;
	cout << r << endl;
	cout << s << endl;
	cout << v << endl;
	return true

}*/

// string = 34333131353030636165623163363738613765326135633832326335343235623964633533323864653364303365393864656663363732643932323237656132;

//string difficulty = "00011111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111";
string difficulty = "000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000b01d242b7c6575ef7c593ec1204d6282000000";


// string difficulty = "00111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111";


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
		str.append(to_string(tx.gas));
		str.append(to_string(tx.gasPrice));
	}

	for (auto& u : block.unclesList) {
		str.append(u.miner);
		str.append(to_string(u.number));
	}

	return str;
	//str.append(block.transaction_count);
}

string mine(Block block) {

	block.nonce = 0;
	
	//cout << (char*)&block;

	
	//cout << block_str << endl;

	// serial search for nonce
	//const string ibuf = "compute sha1";
    //unsigned char obuf[20];
	string hash;
	while (true) {
		string block_str = convert_block_to_string(block);
		hash = sha256(block_str);
		if (hash.compare(difficulty) < 0) {
			cout << block.nonce << "\n";
			break;
		}

	    block.nonce++;
	}
    // hash = sha256(block_str); //, strlen(ibuf), obuf);

    //int i;
    //cout << block.nonce << "\n";
    //printf("Ox");
    //cout << hash << endl;
    /*for (i = 0; i < hash.size(); i++) {
        printf("%02x", hash[i]);
    }*/
    //printf("\n");
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

	//std::vector<Transaction> TransactionList;
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
	    //cout << block.timestamp << "\t";
	    block.number = stoi(data.key());//atoi(d.key().c_str());
	    //cout << block.number << "\t";
	    block.miner = data.value()["miner"];

	    // running for originial ethrereum difficulty
	    if (block.number > 4370005) break;

		//cout << block.miner << "\t";
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
	    
	    int16_t txid=0;
		for (auto& tx : data.value()["transactions"]) {
        	double fee;
        	//cout << "from" << tx["from"] << endl;
        	//cout << "to" << tx["to"] << endl;
        	
        	// verify transaction

        	//cout << tx["hash"];
	    	//cout << tx["r"];
	    	//cout << tx["s"];
	    	//cout << tx["v"];

        	Transaction transaction;
      		transaction.transactionID = txid++;
      		transaction.fromAddress = tx["from"];
      		transaction.toAddress = tx["to"];
      		transaction.value = (double)tx["value"];
      		transaction.gas = (double)tx["gas"];
      		transaction.gasPrice = (double)tx["gasPrice"]; 
      		block.transactionsList.push_back(transaction);

	    	//verifyECDSA(tx["publickey"], tx["hash"],tx["r"],tx["s"],stoi(tx["v"]));

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
        	
        	Uncle u;
      		u.miner = uncle["miner"];
      		u.number = uncle["number"];
      		block.unclesList.push_back(u);


        	int ubn = uncle["number"];
        	//cout << ubn << endl;

        	DataItemsMap[uncle["miner"]] = DataItemsMap[uncle["miner"]] + ((ubn+8-block.number) * base_reward)/8;
        	uncle_reward += (base_reward/32);
        	//cout << i_str << "\t" << base_reward << endl;
        	//cout << "uncle: " << ubn << " reward:" << uncle_reward << "," << (i-ubn) << " ," << ((ubn+8-i) * base_reward)/8 << endl;
        }
        Logger::instance().log("Block " + data.key() + " Uncles Execution ends", Logger::kLogLevelInfo);
	    

    	DataItemsMap[block.miner] = DataItemsMap[block.miner] + (base_reward + tx_fees + uncle_reward);
    	
    	//cout << i_str << "\t" << block.miner << "\t" << DataItemsMap[block.miner] << endl << endl;
    	//Logger::instance().log("Block " + data.key() + " saving DataItemsMap starts", Logger::kLogLevelInfo);
		//saveDataItemsMap("logs/accounts/block_"+data.key()+".csv");
		//Logger::instance().log("Block " + data.key() + " saving DataItemsMap ends", Logger::kLogLevelInfo);
    	

    	auto end1 = chrono::steady_clock::now();
    	//elapsed_time = (double) (clock() - t);
    	//cout << elapsed_time/CLOCKS_PER_SEC << endl;
    	cout << chrono::duration_cast<chrono::microseconds>(end1 - start).count() << "\t";

    	Logger::instance().log("Block " + data.key() + " Block Mining starts", Logger::kLogLevelInfo);

    	// Adding Block to Blockchain
    	//cout << "\nAdded block " << block.number << "\n";
    	string block_hash = "0000000000000000000000000000000000000000000000000000000000000000";
    	block.prevHash = block_hash;
    	block_hash = mine(block);
    	blockchain.push_back(block);

    	Logger::instance().log("Block " + data.key() + " Block Mining ends", Logger::kLogLevelInfo);
    	auto end2 = chrono::steady_clock::now();
    	//elapsed_time = (double) (clock() - t);
    	//cout << elapsed_time/CLOCKS_PER_SEC << endl;
    	cout << chrono::duration_cast<chrono::microseconds>(end2 - end1).count() << "\t" << chrono::duration_cast<chrono::microseconds>(end2 - start).count() << endl;
    	Logger::instance().log("Block " + data.key() + " Block Execution ends", Logger::kLogLevelInfo);

	}



	//cout << "\nTotal: " << total_transactions << endl;
	//cout << "Success: " << successful_transactions << endl;
	//cout << "Failed: " << failed_transactions << endl;
	//cout << "Base Reward: " << base_reward << endl;
	Logger::instance().log("Serial Execution ends", Logger::kLogLevelInfo);
	
	return 0;
}
