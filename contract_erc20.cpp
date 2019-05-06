#include <iostream>
#include <cstdlib>
#include <string>
#include <chrono>
#include <thread>
#include <vector>
#include <map>
#include <bits/stdc++.h> 
#include "nlohmann/json.hpp"

#include <openssl/sha.h>
#include <sstream>
#include <iomanip>

using namespace std;
using json = nlohmann::json;





string sender = "";
string owner = "";
double value = 0;
double ether = pow(10,18);
map<string, double> balances;
map<string, map<string, double>> allowed;


map<string, map<string,double>> tokens; //mapping of token addresses to mapping of account balances (token=0 means Ether)
map<string, map<string,bool>> orders; //mapping of user accounts to mapping of order hashes to booleans (true = submitted by user, equivalent to offchain signature)
map<string, map<string,int64_t>> orderFills; //mapping of user accounts to mapping of order hashes to uints (amount of order that has been filled)


double _totalSupply = 88 * pow(10,14);

const string symbol = "ERC20";
const string name = "ERC20";
const uint8_t decimals = 8;

string contract_data_path = "data/contract/";

//double totalSupply();
//double balanceOf(string _owner);
//bool transfer(string _to, double _value);
//bool transferFrom(string _from, string _to, double _value);
//bool approve(string _spender, double _value);
//double allowance(string _owner, string _spender);

//event Transfer(address indexed _from, address indexed _to, uint256 _value);
//event Approval(address indexed _owner, address indexed _spender, uint256 _value);

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

double hexTodouble(const std::string& hexstr)
{
	long l;
	//double d;
    stringstream ss;
	ss << hex << hexstr;
	ss >> l;
	return (double)l;
}


int8_t hexToint8(const std::string& hexstr)
{
	long l;
	//double d;
    stringstream ss;
	ss << hex << hexstr;
	ss >> l;
	return (int8_t)l;
}

int32_t hexToint32(const std::string& hexstr)
{
	long l;
	//double d;
    stringstream ss;
	ss << hex << hexstr;
	ss >> l;
	return (int32_t)l;
}

int64_t hexToint64(const std::string& hexstr)
{
	long l;
	//double d;
    stringstream ss;
	ss << hex << hexstr;
	ss >> l;
	return (int64_t)l;
}



class ERC20
{
public:
	ERC20(vector<string> addresses);
	void distributeERC20(vector<string> addresses);
	double totalSupply();
	double balanceOf(string _owner);
	bool transfer(string _to, double _value);
	bool transferFrom(string _from, string _to, double _value);
	bool approve(string _spender, double _value);
	double allowance(string _owner, string _spender);
	void deposit();
	void depositToken(string token, double amount);
	void withdraw(double amount);
	void withdrawToken(string token, double amount);
	void order(string tokenGetAddress, double amountGet, string tokenGiveAddress, double amountGive, int64_t expires, int64_t nonce);
	void trade(string tokenGetAddress, double amountGet, string tokenGiveAddress, double amountGive, int64_t expires, int64_t nonce, string userAddress, int8_t v, string r, string s, double amount);
	void tradeBalances(string tokenGetAddress, double amountGet, string tokenGiveAddress, double amountGive, string userAddress, double amount);
	void cancelOrder(string tokenGetAddress, double amountGet, string tokenGiveAddress, double amountGive, int64_t expires, int64_t nonce, int8_t v, string r, string s );
	void makeWallet(string wallet);
	void sweep(string address, double amount);
	void split(string address1, string address2);
	~ERC20();
	
};

ERC20::ERC20(vector<string> addresses) {
	//owner = sender;
	//cout << "Initializing Accounts" << endl;
	balances[owner] = 88 * pow(10,14);
	//cout << owner << "\n" << balances[owner] << endl;
	approve(owner,balances[owner]);
	for (uint i = 0; i < addresses.size(); i++) {
		if (owner.compare(addresses[i]))
			balances[addresses[i]] = 0;
	}
}

/*
modifier onlyOwner() {
require(msg.sender == owner);
_;
}
*/

void ERC20::distributeERC20(vector<string> addresses) {
	//cout << "Distributing ERC20 to accounts" << endl;
	for (uint i = 0; i < addresses.size(); i++) {
		balances[owner] -= 245719916000;
		balances[addresses[i]] += 245719916000;
		approve(addresses[i],balances[addresses[i]]);
	}
}


double ERC20::totalSupply() {
	return _totalSupply;
}


double ERC20::balanceOf(string _owner) {
	return balances[_owner];
}

bool ERC20::transfer(string _to, double _amount) {
	if (balances[sender] >= _amount && _amount > 0 
		&& balances[_to] + _amount > balances[_to]) {
    	balances[sender] -= _amount;
    	balances[_to] += _amount;
    	//transferFrom(sender, _to, _amount);
    	//cout << sender << "\t" << _to << "\t" << _amount << endl;
    	return true;
 	} else {
 		//cout << "failed" << endl;
    	return false;
 	}
}


bool ERC20::transferFrom(string _from, string _to, double _amount) {
	if (balances[_from] >= _amount
    	&& allowed[_from][sender] >= _amount
    	&& _amount > 0
    	&& balances[_to] + _amount > balances[_to]) {
    	balances[_from] -= _amount;
    	allowed[_from][sender] -= _amount;
    	balances[_to] += _amount;
    	//Transfer(_from, _to, _amount);
    	//cout << _from << "\t" << _to << "\t" << _amount << endl;
    	return true;
	} else {
		//cout << "failed" << endl;
    	return false;
 	}
}

bool ERC20::approve(string _spender, double _amount) {
	allowed[sender][_spender] = _amount;
	//Approval(sender, _spender, _amount);
	return true;
}

double ERC20::allowance(string _owner, string _spender) {
 return allowed[_owner][_spender];
}


void ERC20::deposit() {
	//if (!(transferFrom(sender,owner,amount))) {
	//	cout << "deposit transfer failed" << endl;
	//	exit(-2);
	//}
	//tokens["0"][sender] += amount;
	//return true; 
}

void ERC20::depositToken(string token, double amount) {
	if (token == "0") {
		cout << token << endl;
		exit(-1);
	}
	if (!(transferFrom(sender,owner,amount))) {
		cout << "depositToken transfer failed" << endl;
		exit(-2);
	}
	tokens[token][sender] += amount; 
	//return true;
}

void ERC20::withdraw(double amount) {
	if (tokens["0"][sender] < amount) {
		cout << "insufficient balance" << endl;
		exit(-2);
	}
	if (!(transfer(sender,amount))) {
		cout << "deposit transfer failed" << endl;
		exit(-2);
	}
	tokens["0"][sender] -= amount;
	//return true;
}

void ERC20::withdrawToken (string token, double amount) {
	if (token == "0") {
		cout << token << endl;
		exit(-1);
	}
	if (tokens[token][sender] < amount) {
		cout << "withdrawal failed" << endl;
		exit(-2);
	}
	tokens[token][sender] -= amount; 
	if (!(transfer(sender,amount))) {
		cout << "transfer failed in withdrawal" << endl;
		exit(-3);
	}
	//return true;	
}

void ERC20::order(string tokenGetAddress, double amountGet, string tokenGiveAddress, double amountGive, int64_t expires, int64_t nonce) {
	string str = owner + tokenGetAddress + to_string(amountGet) + tokenGiveAddress + to_string(amountGive) + to_string(expires) + to_string(nonce);
	string hash = sha256(str);
	orders[sender][hash] = true;
}

void ERC20::trade(string tokenGetAddress, double amountGet, string tokenGiveAddress, double amountGive, int64_t expires, int64_t nonce, string userAddress, int8_t v, string r, string s, double amount) {
	string str = owner + tokenGetAddress + to_string(amountGet) + tokenGiveAddress + to_string(amountGive) + to_string(expires) + to_string(nonce);
	string hash = sha256(str);
	if (!orders[userAddress][hash]) {
		cout << "trade failed" << endl;
		exit(-3);
	} 
	tradeBalances(tokenGetAddress, amountGet, tokenGiveAddress, amountGive, userAddress, amount);
	orderFills[userAddress][hash] += amount;
}

void ERC20::tradeBalances(string tokenGetAddress, double amountGet, string tokenGiveAddress, double amountGive, string userAddress, double amount) {
	const double feeMake = 0;
	const double feeTake = 3000000000000000;

	double feeMakeXfer = (amount * feeMake) / ether;
	double feeTakeXfer = (amount * feeTake) / ether;
	double feeRebateXfer = 0;

	double feeAccount = feeRebateXfer + feeTakeXfer - feeRebateXfer;

	tokens[tokenGetAddress][sender] -= (amount + feeTakeXfer);
	tokens[tokenGetAddress][userAddress] += ((amount + feeRebateXfer) - feeMakeXfer);
    //tokens[tokenGetAddress][feeAccount] += ((feeMakeXfer + feeTakeXfer) - feeRebateXfer);
    tokens[tokenGiveAddress][userAddress] -= ((amountGive * amount) / amountGet);
    tokens[tokenGiveAddress][sender] += ((amountGive * amount) / amountGet);
}


void ERC20::cancelOrder(string tokenGetAddress, double amountGet, string tokenGiveAddress, double amountGive, int64_t expires, int64_t nonce, int8_t v, string r, string s) {
	string str = owner + tokenGetAddress + to_string(amountGet) + tokenGiveAddress + to_string(amountGive) + to_string(expires) + to_string(nonce);
	string hash = sha256(str);
	if (!(orders[sender][hash])) {
		cout << "order not found" << endl;
		exit(-4);
	}
	orderFills[sender][hash] = amountGet;
}


void readOwner(string filename) {
	ifstream ownerFile(filename, ios::in);
	ownerFile >> owner;
	ownerFile.close();
}

void saveOwner(string filename) {
	ofstream ownerFile(filename, ios::out | ios::trunc);
	ownerFile << owner;
	ownerFile.close();
}



void readBalancesMap(string filename) {
	balances.clear();
	ifstream balancesFile(filename, ios::in);
	string line;
	while(getline(balancesFile,line))
	{
	    stringstream   linestream(line);
	    string 		address;
	    string         value;
	    bool flag = false;
	    while(getline(linestream,value,','))
	    {
	    	if (flag) {
	    		balances[address] = stod(value);
	    		//cout << address << "," << value << endl;
	    	} else {
	    		address = value;
	    		flag = true;
	    	}
	        //std::cout << "Value(" << value << ")\n";

	    }
	    //std::cout << "Line Finished" << std::endl;
	}
	balancesFile.close();
}



void readAllowedMap(string filename) {
	ifstream allowedFile(filename, ios::in);
	string line;
	while(getline(allowedFile,line))
	{
	    stringstream   linestream(line);
	    string 		address1;
	    string 		address2;
	    string         value;
	    bool flag1 = false;
	    bool flag2 = false;
	    while(getline(linestream,value,','))
	    {
	    	if (flag1 && flag2) {
	    		allowed[address1][address2] = stod(value);
	    		//cout << address1 << "," << address2 << "," << value << endl;
	    	} else if(!flag1){
	    		address1 = value;
	    		flag1 = true;
	    	} else if (!flag2) {
	    		address2 = value;
	    		flag2 = true;
	    	}
	        //std::cout << "Value(" << value << ")\n";

	    }
	    //std::cout << "Line Finished" << std::endl;
	}
	allowedFile.close();
}


void readTokensMap(string filename) {
	ifstream tokensFile(filename, ios::in);
	string line;
	while(getline(tokensFile,line))
	{
	    stringstream   linestream(line);
	    string 		address1;
	    string 		address2;
	    string         value;
	    bool flag1 = false;
	    bool flag2 = false;
	    while(getline(linestream,value,','))
	    {
	    	if (flag1 && flag2) {
	    		tokens[address1][address2] = stod(value);
	    		//cout << address1 << "," << address2 << "," << value << endl;
	    	} else if(!flag1){
	    		address1 = value;
	    		flag1 = true;
	    	} else if (!flag2) {
	    		address2 = value;
	    		flag2 = true;
	    	}
	        //std::cout << "Value(" << value << ")\n";

	    }
	    //std::cout << "Line Finished" << std::endl;
	}
	tokensFile.close();
}

void readOrdersMap(string filename) {
	ifstream ordersFile(filename, ios::in);
	string line;
	while(getline(ordersFile,line))
	{
	    stringstream   linestream(line);
	    string 		address1;
	    string 		address2;
	    string         value;
	    bool flag1 = false;
	    bool flag2 = false;
	    while(getline(linestream,value,','))
	    {
	    	if (flag1 && flag2) {
	    		orders[address1][address2] = stoi(value);
	    		//cout << address1 << "," << address2 << "," << value << endl;
	    	} else if(!flag1){
	    		address1 = value;
	    		flag1 = true;
	    	} else if (!flag2) {
	    		address2 = value;
	    		flag2 = true;
	    	}
	        //std::cout << "Value(" << value << ")\n";

	    }
	    //std::cout << "Line Finished" << std::endl;
	}
	ordersFile.close();
}


void readOrderFillsMap(string filename) {
	ifstream orderFillsFile(filename, ios::in);
	string line;
	while(getline(orderFillsFile,line))
	{
	    stringstream   linestream(line);
	    string 		address1;
	    string 		address2;
	    string         value;
	    bool flag1 = false;
	    bool flag2 = false;
	    while(getline(linestream,value,','))
	    {
	    	if (flag1 && flag2) {
	    		orderFills[address1][address2] = stoi(value);
	    		//cout << address1 << "," << address2 << "," << value << endl;
	    	} else if(!flag1){
	    		address1 = value;
	    		flag1 = true;
	    	} else if (!flag2) {
	    		address2 = value;
	    		flag2 = true;
	    	}
	        //std::cout << "Value(" << value << ")\n";

	    }
	    //std::cout << "Line Finished" << std::endl;
	}
	orderFillsFile.close();
}

void saveBalancesMap(string filename) {
	ofstream balancesFile(filename, ios::out | ios::trunc );
	for (std::map<string,double>::iterator i = balances.begin(); i != balances.end(); ++i)
	{
		balancesFile << i->first << "," << i->second << endl;
		//balancesFile.write((i->first).c_str(),i->first.size());
		//balancesFile.write("\t",1);
		//balancesFile.write(to_string(i->second).c_str(), sizeof(to_string(i->second)));
		//balancesFile.write("\n",1);
	}
	balancesFile.close();
}



void saveAllowedMap(string filename) {
	ofstream allowedFile(filename, ios::out | ios::trunc );
	for (std::map<string,map<string,double>>::iterator i = allowed.begin(); i != allowed.end(); ++i)
	{
		for (map<string,double>::iterator j = i->second.begin(); j != i->second.end(); ++j) {
			allowedFile << i->first << "," << j->first << "," << j->second << endl;
		}
		//allowedFile.write((i->first).c_str(),i->first.size());
		//allowedFile.write("\t",1);
		//allowedFile.write(to_string(i->second).c_str(), sizeof(to_string(i->second)));
		//allowedFile.write("\n",1);
	}
	allowedFile.close();
}


void saveTokensMap(string filename) {
	ofstream tokensFile(filename, ios::out | ios::trunc );
	for (std::map<string,map<string,double>>::iterator i = tokens.begin(); i != tokens.end(); ++i)
	{
		for (map<string,double>::iterator j = i->second.begin(); j != i->second.end(); ++j) {
			tokensFile << i->first << "," << j->first << "," << j->second << endl;
		}
		//allowedFile.write((i->first).c_str(),i->first.size());
		//allowedFile.write("\t",1);
		//allowedFile.write(to_string(i->second).c_str(), sizeof(to_string(i->second)));
		//allowedFile.write("\n",1);
	}
	tokensFile.close();
}

void saveOrdersMap(string filename) {
	ofstream ordersFile(filename, ios::out | ios::trunc );
	for (std::map<string,map<string,bool>>::iterator i = orders.begin(); i != orders.end(); ++i)
	{
		for (map<string,bool>::iterator j = i->second.begin(); j != i->second.end(); ++j) {
			ordersFile << i->first << "," << j->first << "," << j->second << endl;
		}
		//allowedFile.write((i->first).c_str(),i->first.size());
		//allowedFile.write("\t",1);
		//allowedFile.write(to_string(i->second).c_str(), sizeof(to_string(i->second)));
		//allowedFile.write("\n",1);
	}
	ordersFile.close();
}

void saveOrderFillsMap(string filename) {
	ofstream orderFillsFile(filename, ios::out | ios::trunc );
	for (std::map<string,map<string,int64_t>>::iterator i = orderFills.begin(); i != orderFills.end(); ++i)
	{
		for (map<string,int64_t>::iterator j = i->second.begin(); j != i->second.end(); ++j) {
			orderFillsFile << i->first << "," << j->first << "," << j->second << endl;
		}
		//allowedFile.write((i->first).c_str(),i->first.size());
		//allowedFile.write("\t",1);
		//allowedFile.write(to_string(i->second).c_str(), sizeof(to_string(i->second)));
		//allowedFile.write("\n",1);
	}
	orderFillsFile.close();
}

int main(int argc, char const *argv[])
{
	//cout << argc << endl;
	string contract_address = argv[1];
	// updating contract_data_path
	contract_data_path += contract_address;
	string dir = "mkdir -p " + contract_data_path;
	//cout << dir << endl;
	const char *command = dir.c_str();
	bool output = system(command);

	std::vector<string> addresses;
	ifstream file("data/bigquery/addresses.txt");
	std::string str; 
	while (std::getline(file, str))
	{
	    addresses.push_back(str);
	    //cout << str << endl;
	}

	ERC20 *tc = new ERC20(addresses);


	sender = argv[2];
	string input = "";
	if (argc > 3)
	{
		/* code */
		input = argv[3];	
		//cout << input << endl;
	
		//cout << input << endl;

	
		if (input.compare("creates") == 0) {
			owner = sender;
			//cout << "creates" << endl; // << input.size() << endl;
			//cout << "Contract creates" << endl;
			//std::this_thread::sleep_for(std::chrono::seconds(2));
			//cout << sender << endl;
			//cout << owner << endl;
			if (sender.compare(owner) == 0) {
				//cout << "Distributed ERC20" << endl;
				tc->distributeERC20(addresses);
			}
		} else {
			//cout << input << endl;
			//cout << "Reading owner" << endl;
			readOwner(contract_data_path+"/owner.dat");
			//cout << "Reading accounts (balances map)" << endl;
			readBalancesMap(contract_data_path+"/balances.dat");

			//cout << "Reading approvals (allowed map)" << endl;
			readAllowedMap(contract_data_path+"/allowed.dat");


			readTokensMap(contract_data_path+"/tokens.dat");

			readOrdersMap(contract_data_path+"/orders.dat");

			readOrderFillsMap(contract_data_path+"/orderFills.dat");

			
			if (input.size() < 10) {
				//cout << input.size() << endl << input << endl;
				//cout << "no input" << endl;
				exit(-1);
			}

			string fxHash = input.substr(0,10);
			//cout << input.size() << endl << input << endl;


			//cout << input.size() << endl;
			//cout << input << endl;
			//cout << fxHash << endl;

			//cout << "fxHash: " << fxHash << endl;
			

			//json contract_data;
			
			//std::ifstream ethereum_data_file(argv[1]); 
		  	//contract_data_file >> contract_data;

			/*function_sighashes = {  
				"0x06fdde03":"name", 
				"0x095ea7b3":"approve", 
				"0x18160ddd":"totalSupply", 
				"0x1b8de48c":"distributeERC20",
				"0x23b872dd":"transferFrom", 
				"0x313ce567":"decimals", 
				"0x70a08231":"balanceOf", 
				"0x8da5cb5b":"owner",	
				"0x95d89b41":"symbol", 
				"0xa9059cbb":"transfer", 
				"0xdd62ed3e":"allowance", 
				"0xffffffff":"throw"
			}*/
			
			bool status = false;
			
			if (fxHash.compare("0xa9059cbb") == 0) {
				//cout << "transfer executes" << endl;
				//cout << input.size() << endl << input << endl << fxHash << endl;
				string str = input.substr(10,64);
				str.erase(0, min(str.find_first_not_of("0"), str.size()-1));
				string address = "0x" + str;
				//cout << "to: " << address << endl;
				double value = hexTodouble("0x" + input.substr(74,64));
				//cout << "amount: " << value << endl;
				status = tc->transfer(address,value);
				//cout << "transfer executed" << endl;
			} else if (fxHash.compare("0x095ea7b3") == 0) {
				//cout << "approve executes" << endl;
				//cout << input.size() << endl << input << endl << fxHash << endl;
				string str = input.substr(10,64);
				str.erase(0, min(str.find_first_not_of("0"), str.size()-1));
				string spender = "0x" + str;
				//cout << "to: " << spender << endl;
				double amount = hexTodouble("0x" + input.substr(74,64));
				//cout << "amount: " << amount << endl;
				status = tc->approve(spender,amount);
				//cout << "approve executed" << endl;
			} else if (fxHash.compare("0x0a19b14a") == 0) {
				cout << "trade executes" << endl;
				//cout << input.size() << endl << input << endl << fxHash << endl;
				string tokenGetAddress = "0x" + input.substr(10,64);
				tokenGetAddress.erase(2, min(tokenGetAddress.find_first_not_of("0"), tokenGetAddress.size()-1));
				double amountGet = hexTodouble("0x" + input.substr(74,64));
				string tokenGiveAddress = "0x" + input.substr(138,64);
				tokenGiveAddress.erase(2, min(tokenGiveAddress.find_first_not_of("0"), tokenGiveAddress.size()-1));
				double amountGive = hexTodouble("0x" + input.substr(202,64));
				double expires = hexTodouble("0x" + input.substr(266,64));
				int64_t nonce = hexToint64("0x" + input.substr(330,64));
				string userAddress = "0x" + input.substr(394,64);
				userAddress.erase(2, min(userAddress.find_first_not_of("0"), userAddress.size()-1));
				int8_t v = hexToint8("0x" + input.substr(458,16));
				string r = "0x" + input.substr(474,64);
				string s = "0x" + input.substr(538,64);
				double amount = hexTodouble("0x" + input.substr(602,64));
				tc->trade(tokenGetAddress, amountGet, tokenGiveAddress, amountGive, expires, nonce, userAddress, v, r, s, amount);
				cout << "trade executed" << endl;
			} else if (fxHash.compare("0x0b927666") == 0) {
				cout << "order executes" << endl;
				//cout << input.size() << endl << input << endl << fxHash << endl;
				string tokenGetAddress = "0x" + input.substr(10,64);
				tokenGetAddress.erase(2, min(tokenGetAddress.find_first_not_of("0"), tokenGetAddress.size()-1));
				double amountGet = hexTodouble("0x" + input.substr(74,64));
				string tokenGiveAddress = "0x" + input.substr(138,64);
				tokenGiveAddress.erase(2, min(tokenGiveAddress.find_first_not_of("0"), tokenGiveAddress.size()-1));
				double amountGive = hexTodouble("0x" + input.substr(202,64));
				double expires = hexTodouble("0x" + input.substr(266,64));
				int64_t nonce = hexToint64("0x" + input.substr(330,64));
				cout << "order executed" << endl;
			}
			else if (fxHash.compare("0x278b8c0e") == 0) {
				cout << "cancelOrder executes" << endl;
				//cout << input.size() << endl << input << endl << fxHash << endl;
				string tokenGetAddress = "0x" + input.substr(10,64);
				tokenGetAddress.erase(2, min(tokenGetAddress.find_first_not_of("0"), tokenGetAddress.size()-1));
				double amountGet = hexTodouble("0x" + input.substr(74,64));
				string tokenGiveAddress = "0x" + input.substr(138,64);
				tokenGiveAddress.erase(2, min(tokenGiveAddress.find_first_not_of("0"), tokenGiveAddress.size()-1));
				double amountGive = hexTodouble("0x" + input.substr(202,64));
				double expires = hexTodouble("0x" + input.substr(266,64));
				int64_t nonce = hexToint64("0x" + input.substr(330,64));
				int8_t v = hexToint8("0x" + input.substr(394,16));
				string r = "0x" + input.substr(410,64);
				string s = "0x" + input.substr(474,64);
				tc->cancelOrder(tokenGetAddress, amountGet, tokenGiveAddress, amountGive, expires, nonce, v, r, s);
				cout << "cancelOrder executed" << endl;
			} else if (fxHash.compare("0x9e281a98") == 0) {
				cout << "wthdrawToken executes" << endl;
				//cout << input.size() << endl << input << endl;
				string address = "0x" + input.substr(10,64);
				address.erase(2, min(address.find_first_not_of("0"), address.size()-1));
				double amount = hexTodouble("0x" + input.substr(74,64));
				tc->withdrawToken(address, amount);
				cout << "withdrawToken executed" << endl;
			} else if (fxHash.compare("0x2e1a7d4d") == 0) {
				cout << "wthdraw executes" << endl;
				//cout << input.size() << endl << input << endl;
				double amount = hexTodouble("0x" + input.substr(10,64));
				tc->withdraw(amount);
				cout << "withdraw executed" << endl;
			} else if (fxHash.compare("0xd0e30db0") == 0) {
				cout << "deposit executes" << endl;
				//cout << input.size() << endl << input << endl;
				//string address = input.substr(10,64);
				//double amount = input.substr(74,64);
				tc->deposit();
				cout << "deposit executed" << endl;
			} else if (fxHash.compare("0x338b5dea") == 0) {
				cout << "depositToken executes" << endl;
				//cout << input.size() << endl << input << endl;
				string address = "0x" + input.substr(10,64);
				address.erase(2, min(address.find_first_not_of("0"), address.size()-1));
				double amount = hexTodouble("0x" + input.substr(74,64));
				tc->depositToken(address, amount);
				cout << "depositToken executed" << endl;
			} else if (fxHash.compare("0xa9b1d507") == 0) {
				cout << "makeWallet executes" << endl;
				//cout << input.size() << endl << input << endl;
				string wallet = "0x" + input.substr(10,64);
				wallet.erase(2, min(wallet.find_first_not_of("0"), wallet.size()-1));
				//tc->makeWallet(wallet);
				cout << "makeWallet executed" << endl;
			} else if (fxHash.compare("0x6ea056a9") == 0) {
				cout << "sweep executes" << endl;
				//cout << input.size() << endl << input << endl;
				string address = "0x" + input.substr(10,64);
				address.erase(2, min(address.find_first_not_of("0"), address.size()-1));
				double amount = hexTodouble("0x" + input.substr(74,64));
				//tc->sweep(address, amount);
				cout << "sweep executed" << endl;
			} else if (fxHash.compare("0x0f2c9329") == 0) {
				cout << "split executes" << endl;
				//cout << input.size() << endl << input << endl;
				string address1 = "0x" + input.substr(10,64);
				address1.erase(2, min(address1.find_first_not_of("0"), address1.size()-1));
				string address2 = "0x" + input.substr(74,64);
				address2.erase(2, min(address2.find_first_not_of("0"), address2.size()-1));
				//tc->split(address1, address2);
				cout << "split executed" << endl;
			}
			else {
				//cout << input.size() << endl << input << endl;	
				//cout << "wrong input" << endl;
				exit(-1);
			}
		} 
	} else {
		//cout << "no input" << endl;
	}

	//cout << "Saving owner" << endl;
	saveOwner(contract_data_path+"/owner.dat");
	//cout << "Saving accounts (balances map)" << endl;
	saveBalancesMap(contract_data_path+"/balances.dat");
	//cout << "Saving approvals (allowed map)" << endl;
	saveAllowedMap(contract_data_path+"/allowed.dat");

	saveTokensMap(contract_data_path+"/tokens.dat");

	saveOrdersMap(contract_data_path+"/orders.dat");

	saveOrderFillsMap(contract_data_path+"/orderFills.dat");

	return 0;
}