#include <iostream>
#include <cstdlib>
#include <string>
#include <chrono>
#include <thread>
#include <vector>
#include <map>
#include <bits/stdc++.h> 
#include "nlohmann/json.hpp"

using namespace std;
using json = nlohmann::json;



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
	~ERC20();
	
};

string sender = "";
string owner = "";
map<string, double> balances;
map<string, map<string, double>> allowed;
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



double hexTodouble(const std::string& hexstr)
{
	long l;
	//double d;
    stringstream ss;
	ss << hex << hexstr;
	ss >> l;
	return (double)l;
}


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
			//cout << "Contract creates" << endl;
			//std::this_thread::sleep_for(std::chrono::seconds(2));
			//cout << sender << endl;
			//cout << owner << endl;
			if (sender.compare(owner) == 0) {
				//cout << "Distributed ERC20" << endl;
				tc->distributeERC20(addresses);
			}
		} else {
			cout << input << endl;
			//cout << "Reading owner" << endl;
			readOwner(contract_data_path+"/owner.dat");
			//cout << "Reading accounts (balances map)" << endl;
			readBalancesMap(contract_data_path+"/balances.dat");

			//cout << "Reading approvals (allowed map)" << endl;
			readAllowedMap(contract_data_path+"/allowed.dat");

			string fxHash = input.substr(0,10);
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
				string str = input.substr(10,64);
				str.erase(0, min(str.find_first_not_of("0"), str.size()-1));
				string address = "0x" + str;
				//cout << "to: " << address << endl;
				double value = hexTodouble("0x" + input.substr(74,input.size()));
				//cout << "amount: " << value << endl;
				status = tc->transfer(address,value);
				//cout << "status: " << status << endl;
			} else if (fxHash.compare("0x095ea7b3") == 0) {
				//cout << "approve executes" << endl;
				string str = input.substr(10,64);
				str.erase(0, min(str.find_first_not_of("0"), str.size()-1));
				string spender = "0x" + str;
				//cout << "to: " << spender << endl;
				double amount = hexTodouble("0x" + input.substr(74,input.size()));
				//cout << "amount: " << amount << endl;
				status = tc->approve(spender,amount);
				//cout << "status: " << status << endl;
			} else {
				cout << "wrong input" << endl;
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
	return 0;
}