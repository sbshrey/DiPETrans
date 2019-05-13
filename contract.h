#include <string>
#include <map>
#include "gen-cpp/SharedService.h"
using namespace std;
using namespace  ::SharedService;
/*
struct DataItem {
	//1: required string address;
	double value;
	string owner;
	map<string,double> balances;
	map<string,map<string,double>> allowed;
	
	//map<string,allowedMap> file1; // allowed map file <filename, allowedMap struct>
	//map<string,balanceMap> file2; // balance map file <filename, balanceMap struct>
	//map<string,string> file3; // owner file <filename, owner name>
};
*/


/* Your function statement here */
void call_contract(DataItem* dataItem, string contractAddress, string senderAddress, string input, double value);
