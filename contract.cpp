#include <iostream>
#include "contract.h"
#include "utils.h"
#include <cstdlib>
#include <string>
#include <chrono>

#include <thread>
#include <vector>
#include <map>
#include <set>
#include <bits/stdc++.h> 
#include "nlohmann/json.hpp"

#include <openssl/sha.h>
#include <sstream>
#include <iomanip>

#include "gen-cpp/SharedService.h"


using namespace std;

using namespace  ::SharedService;

using json = nlohmann::json;

double totalSupply = 0;

double houseEdge = 1;
double houseEdgeDivisor = 1;

class ERC20
{
public:
  ERC20(DataItem  *dataItem, string _sender, std::set<string> addresses);
  void distributeERC20(DataItem* dataItem, string _sender, std::set<string> addresses);
  //double totalSupply(DataItem* dataItem);
  //double balanceOf(DataItem* dataItem, string _owner);
  bool transfer(DataItem* dataItem, string _sender, string _to, double _value);
  bool transferFrom(DataItem* dataItem, string _from, string _to, double _value);
  bool approve(DataItem* dataItem, string _sender, string _spender, double _value);
  double allowance(DataItem* dataItem, string _owner, string _spender);
  void deposit(DataItem* dataItem);
  void depositToken(DataItem* dataItem, string token, double amount);
  void withdraw(DataItem* dataItem, double amount);
  void withdrawToken(DataItem* dataItem, string token, double amount);
  
  void vote(DataItem* dataItem, int64_t _tokenIndex);
  void transferOwnership(DataItem* dataItem, string address);
  void submitTransaction(DataItem* dataItem, string _sender, string _address, double _amount, string _data);
  void issue(DataItem* dataItem, string _to, double _amount);
  void playerRollDice(DataItem* dataItem, string _sender, int64_t _rollUnder);
  void multisend(DataItem* dataItem, string _tokenAddr, std::vector<string> dests, std::vector<int64_t> values);

  ~ERC20();
  
};


ERC20::ERC20(DataItem* dataItem, string _sender, set<string> addresses) {
	//owner = sender;
	//cout << "Initializing Accounts" << endl;
	totalSupply += 88 * pow(10,14);
	dataItem->balances[dataItem->owner] = pow(10,25);
	//cout << owner << "\n" << balances[owner] << endl;
	approve(dataItem, _sender, dataItem->owner,dataItem->balances[dataItem->owner]);
	for (auto adr : addresses) {
		if (dataItem->owner.compare(adr))
			dataItem->balances[adr] = pow(10,23);
	}
}

void ERC20::distributeERC20(DataItem* dataItem, string _sender, std::set<string> addresses) {
	//cout << "Distributing ERC20 to accounts" << endl;
	for (auto adr: addresses) {
		//dataItem->balances[dataItem->owner] -= 8* pow(10,15);
		//dataItem->balances[addresses[i]] += 8 * pow(10,15);
		approve(dataItem, _sender, adr, dataItem->balances[adr]);
	}
}

bool ERC20::transfer(DataItem* dataItem, string _sender, string _to, double _amount) {
	//cout << _sender << endl;
	//cout << dataItem->balances[_sender] << endl;
	//cout << dataItem->balances[_to] << endl;
	//cout << _amount << endl;
	if (dataItem->balances[_sender] >= _amount and _amount >= 0) {
		dataItem->balances[_sender] -= _amount;
		dataItem->balances[_to] += _amount;
		//transferFrom(sender, _to, _amount);
		//cout << sender << "\t" << _to << "\t" << _amount << endl;
		return true;
	} else {
		//cout << "failed" << endl;
		return false;
	}
}

bool ERC20::approve(DataItem* dataItem, string _sender, string _spender, double _amount) {
	dataItem->allowed[_sender][_spender] = _amount;
	//Approval(sender, _spender, _amount);
	return true;
}

void ERC20::vote(DataItem* dataItem, int64_t _tokenIndex) {
	auto it = dataItem->votes.find(_tokenIndex);
	if (it == dataItem->votes.end()) {
		dataItem->votes[_tokenIndex] = 0; 
	} else {
		dataItem->votes[_tokenIndex] += 1;
		//cout << "votes " << dataItem->votes[_tokenIndex] << endl;
	}
}


void ERC20::transferOwnership(DataItem* dataItem, string newOwner) {
	if (newOwner != dataItem->owner) {
		dataItem->owner = newOwner;

	}
}

void ERC20::submitTransaction(DataItem* dataItem, string _sender, string _address, double _amount, string _data) {
	Transaction transaction;
	transaction.fromAddress = _sender;
	transaction.toAddress = _address;
	transaction.value = _amount;
	transaction.input = _data;
	transaction.creates = "";
	dataItem->transactions.push_back(transaction);
}

void ERC20::issue(DataItem* dataItem, string _to, double _amount) {
	totalSupply += _amount;
	dataItem->balances[_to] += _amount;
}


void ERC20::playerRollDice(DataItem* dataItem, string _sender, int64_t _rollUnder) {
	PlayerRoll playerRoll;
	playerRoll.playerNumber = _rollUnder;
	playerRoll.playerBetValue = dataItem->value;
	playerRoll.playerAddress = _sender;
	playerRoll.playerProfit = ((((dataItem->value * (100-(_rollUnder - 1))) / (_rollUnder - 1)+dataItem->value))*houseEdge/houseEdgeDivisor)-dataItem->value;

	string playerBetId = sha256(to_string(playerRoll.playerNumber) + to_string(playerRoll.playerBetValue) + playerRoll.playerAddress + to_string(playerRoll.playerProfit)); 
	dataItem->playerRolls[playerBetId] = playerRoll;
}


void call_contract(DataItem *dataItem, string contractAddress, string senderAddress, string input, double value) {
  /*

	struct DataItem {
	//1: required string address;
	double value;
	string owner;
	map<string,double> balances;
	map<string,map<string,double>> allowed;
	map<int64_t, int64_t> votes;
	//map<string,allowedMap> file1; // allowed map file <filename, allowedMap struct>
	//map<string,balanceMap> file2; // balance map file <filename, balanceMap struct>
	//map<string,string> file3; // owner file <filename, owner name>
};

  */

	ERC20 *tc;
	//cout << input << endl;
	if (input == "creates") {
		//cout << "creating contract" << endl;
		dataItem->owner = senderAddress;
		std::set<string> addresses;
		ifstream file("data/bigquery/contract_addresses/"+contractAddress+".json");
		//ifstream file("data/bigquery/addresses.json");
		json addresses_data;
		file >> addresses_data;

		//cout << contractAddress << "\t" << addresses_data.size() << endl;
		for (json::iterator it = addresses_data.begin(); it != addresses_data.end(); ++it) {
			for (auto adr : it.value())
				//string str = adr.get<string>();
	    		addresses.insert(adr.get<string>());
	    }
		
		//cout << "address size " << addresses.size() << endl;
		tc = new ERC20(dataItem, senderAddress, addresses);
		//cout << "constructor called" << endl;
		tc->distributeERC20(dataItem, senderAddress, addresses);
		//cout << "called distributeERC20" << endl;
		

	} else if ((input.size() < 10)) {
		//cout << input << endl;
		//cout << "No function to call" << endl;
	} else {
		string fxHash =  input.substr(0,10);
		//cout << fxHash << endl;
		bool status = false;
		// https://bloxy.info/ (search everything)
		if (fxHash.compare("0xa9059cbb") == 0) {
		  //cout << "transfer executes" << endl;
		  //cout << input.size() << endl << input << endl << fxHash << endl;
		  string str = input.substr(10,64);
		  str.erase(0, min(str.find_first_not_of("0"), str.size()-1));
		  string address = "0x" + str;
		  //cout << "to: " << address << endl;
		  double amount = hexTodouble("0x" + input.substr(74,64));
		  //cout << "amount: " << value << endl;
		  status = tc->transfer(dataItem, senderAddress, address, amount);
		  if (dataItem->balances.find(senderAddress) == dataItem->balances.end())  {
		  	cout << "address not found\t" <<  contractAddress << "\t"  <<  senderAddress << dataItem->balances[senderAddress] << "\t" << amount << endl;
		  	exit(-1);
		  }
		  	
		  if (!status) {
		  	//cout << senderAddress << endl;
		  	//if (dataItem->balances.find(address) == dataItem->balances.end()) 
		  	cout <<  contractAddress << "\t"  <<  senderAddress << "\t" << dataItem->balances[senderAddress] << "\t" << amount << endl;
		  	//exit(-1);
		  }
		  cout << "transfer executed" << endl;
		} else if (fxHash.compare("0x095ea7b3") == 0) {
		  //cout << "approve executes" << endl;
		  //cout << input.size() << endl << input << endl << fxHash << endl;
		  string str = input.substr(10,64);
		  str.erase(0, min(str.find_first_not_of("0"), str.size()-1));
		  string spender = "0x" + str;
		  //cout << "to: " << spender << endl;
		  double amount = hexTodouble("0x" + input.substr(74,64));
		  //cout << "amount: " << amount << endl;
		  status = tc->approve(dataItem, senderAddress, spender,amount);
		  //cout << "status " << status << endl;
		   
		  cout << "approve executed" << endl;
		} else if (fxHash.compare("0x0121b93f") == 0)
		{
		  // https://etherscan.io/address/0x2b2d7d874bbfb73f85b2f8a9ee0d9f3e93722622#code

		  /*

		  struct TokenProposal {
		    address[15] consideredTokens;
		    uint startBlock;
		    uint startTime;
		    uint duration;
		    address votingToken;
		    uint[15] yesVotes;
		  }

		  TokenProposal[] tokenBatches;

		  /// @notice Vote for specific token with yes
		  /// @param _tokenIndex is the position from 0-9 in the token array of the chosen token
		  function vote(uint _tokenIndex) public {
		    // voting only on the most recent set of proposed tokens
		    require(tokenBatches.length > 0);
		    uint _proposalId = tokenBatches.length - 1;
		    require(_tokenIndex < 15);

		    TokenProposal memory p = tokenBatches[_proposalId];

		    require(now < p.startTime + p.duration);

		    uint amount = DestructibleMiniMeToken(p.votingToken).balanceOf(msg.sender);
		    require(amount > 0);

		    require(DestructibleMiniMeToken(p.votingToken).transferFrom(msg.sender, address(this), amount));

		    tokenBatches[_proposalId].yesVotes[_tokenIndex] += amount;

		    emit Vote(_proposalId, msg.sender, tokenBatches[_proposalId].consideredTokens[_tokenIndex], amount);
		  }

		  */

		  //cout << "vote executes" << endl;

		  int64_t tokenIndex = hexToint64("0x" + input.substr(10,64));
		  //cout << "input " << tokenIndex << endl;
		  tc->vote(dataItem, tokenIndex);
		  /*
		  if (tokenBatches.size() > 0) {
		    int64_t _proposalId = tokenBatches.size() - 1;
		    if (_tokenIndex < 15) {

		    } else {
		      cout << "_tokenIndex is out of bounds (more than 15)" << endl;
		    }
		  } else {
		    cout << "tokenBatches is empty" << endl;
		  }
		  */
		  cout << "vote executed" << endl;
		} else if (fxHash.compare("0xf2fde38b") == 0)
		{
		  // https://etherscan.io/address/0x2b2d7d874bbfb73f85b2f8a9ee0d9f3e93722622#code

		  /*

		  function transferOwnership(address newOwner) onlyOwner {
		      if (newOwner != address(0)) {
		        owner = newOwner;
		      }
		    }

		  */
		  //cout << "transferOwnership executes" << endl;
		  string address = "0x" + input.substr(10,64);
		  address.erase(2, min(address.find_first_not_of("0"), address.size()-1));
		  tc->transferOwnership(dataItem, address);
		  cout << "transferOwnership executed" << endl;
		} else if (fxHash.compare("0xc6427474") == 0)
		{
		  // https://medium.com/hellogold/ethereum-multi-signature-wallets-77ab926ab63b
		  // function submitTransaction(address destination, uint value, bytes data)
		  // https://etherscan.io/address/0x05cf67329a262818e67c080e9d511a34d36152c0#code

			/*
			/// @dev Allows an owner to submit and confirm a transaction.
		    /// @param destination Transaction target address.
		    /// @param value Transaction ether value.
		    /// @param data Transaction data payload.
		    /// @return Returns transaction ID.
		    function submitTransaction(address destination, uint value, bytes data)
		        public
		        returns (uint transactionId)
		    {
		        transactionId = addTransaction(destination, value, data);
		        confirmTransaction(transactionId);
		    }

		    /// @dev Allows an owner to confirm a transaction.
		    /// @param transactionId Transaction ID.
		    function confirmTransaction(uint transactionId)
		        public
		        ownerExists(msg.sender)
		        transactionExists(transactionId)
		        notConfirmed(transactionId, msg.sender)
		    {
		        confirmations[transactionId][msg.sender] = true;
		        Confirmation(msg.sender, transactionId);
		        executeTransaction(transactionId);
		    }

		    /// @dev Adds a new transaction to the transaction mapping, if transaction does not exist yet.
		    /// @param destination Transaction target address.
		    /// @param value Transaction ether value.
		    /// @param data Transaction data payload.
		    /// @return Returns transaction ID.
		    function addTransaction(address destination, uint value, bytes data)
		        internal
		        notNull(destination)
		        returns (uint transactionId)
		    {
		        transactionId = transactionCount;
		        transactions[transactionId] = Transaction({
		            destination: destination,
		            value: value,
		            data: data,
		            executed: false
		        });
		        transactionCount += 1;
		        Submission(transactionId);
		    }
			
			/// @dev Allows anyone to execute a confirmed transaction.
		    /// @param transactionId Transaction ID.
		    function executeTransaction(uint transactionId)
		        public
		        notExecuted(transactionId)
		    {
		        if (isConfirmed(transactionId)) {
		            Transaction tx = transactions[transactionId];
		            tx.executed = true;
		            if (tx.destination.call.value(tx.value)(tx.data))
		                Execution(transactionId);
		            else {
		                ExecutionFailure(transactionId);
		                tx.executed = false;
		            }
		        }
		    }
			

			*/

		  //cout << "submitTransaction executes" << endl;
		  string address = "0x" + input.substr(10,64);
		  address.erase(2, min(address.find_first_not_of("0"), address.size()-1));
		  double amount = hexTodouble("0x" + input.substr(74,64));
		  string data = "0x" + input.substr(138,64);
		  data.erase(2, min(data.find_first_not_of("0"), data.size()-1));
		  tc->submitTransaction(dataItem, senderAddress,address,amount,data);
		  cout << "submitTransaction executed" << endl;
		} else if (fxHash.compare("0x867904b4") == 0)
		{
		  // https://etherscan.io/address/0x1f573d6fb3f13d689ff844b4ce37794d79a7ff1c#code

			/*
	
	        @dev increases the token supply and sends the new tokens to an account
	        can only be called by the contract owner

	        @param _to         account to receive the new amount
	        @param _amount     amount to increase the supply by
		    
		    function issue(address _to, uint256 _amount)
		        public
		        ownerOnly
		        validAddress(_to)
		        notThis(_to)
		    {
		        totalSupply = safeAdd(totalSupply, _amount);
		        balanceOf[_to] = safeAdd(balanceOf[_to], _amount);

		        Issuance(_amount);
		        Transfer(this, _to, _amount);
		    }

			*/

		  //cout << "issue executes" << endl;
		  string address = "0x" + input.substr(10,64);
		  address.erase(2, min(address.find_first_not_of("0"), address.size()-1));
		  double amount = hexTodouble("0x" + input.substr(74,64));
		  tc->issue(dataItem, address, amount);
		  cout << "issue executed" << endl;
		} else if (fxHash.compare("0x38bbfa50") == 0)
		{
		  // https://medium.com/coinmonks/using-apis-in-your-ethereum-smart-contract-with-oraclize-95656434292e
		  // https://etherscan.io/address/0xa52e014b3f5cc48287c2d483a3e026c32cc76e6d#code
		  //cout << "__callback executes" << endl;
		  cout << "nested contract calls (assumption does not handle these calls)" << endl;
		  cout << "__callback executed" << endl;
		} else if (fxHash.compare("0xdc6dd152") == 0)
		{
		  // https://etherscan.io/address/0xa52e014b3f5cc48287c2d483a3e026c32cc76e6d#code
		  /*

		  //
		    // public function
		    // player submit bet
		    // only if game is active & bet is valid can query oraclize and set player vars     
		    //

		    function playerRollDice(uint rollUnder) public 
		        payable
		        gameIsActive
		        betIsValid(msg.value, rollUnder)
		  {       

		        //
		        // assign partially encrypted query to oraclize
		        // only the apiKey is encrypted 
		        // integer query is in plain text
		        //       
		        randomQueryID += 1;
		        string memory queryString1 = "[URL] ['json(https://api.random.org/json-rpc/1/invoke).result.random[\"serialNumber\",\"data\"]', '\\n{\"jsonrpc\":\"2.0\",\"method\":\"generateSignedIntegers\",\"params\":{\"apiKey\":${[decrypt] BBMyVwxtiTy5oKVkRGwW2Yc094lpQyT74AdenJ1jywmN4rNyxXqidtDsDBPlASVWPJ0t8SwjSYjJvHAGS83Si8sYCxNH0y2kl/Vw5CizdcgUax1NtTdFs1MXXdvLYgkFq3h8b2qV2oEvxVFqL7v28lcGzOuy5Ms=},\"n\":1,\"min\":1,\"max\":100,\"replacement\":true,\"base\":10${[identity] \"}\"},\"id\":";
		        string memory queryString2 = uint2str(randomQueryID);
		        string memory queryString3 = "${[identity] \"}\"}']";

		        string memory queryString1_2 = queryString1.toSlice().concat(queryString2.toSlice());

		        string memory queryString1_2_3 = queryString1_2.toSlice().concat(queryString3.toSlice());

		        bytes32 rngId = oraclize_query("nested", queryString1_2_3, gasForOraclize);   
		                 
		        // map bet id to this oraclize query //
		    playerBetId[rngId] = rngId;
		        // map player lucky number to this oraclize query //
		    playerNumber[rngId] = rollUnder;
		        // map value of wager to this oraclize query //
		        playerBetValue[rngId] = msg.value;
		        // map player address to this oraclize query //
		        playerAddress[rngId] = msg.sender;
		        // safely map player profit to this oraclize query //                     
		        playerProfit[rngId] = ((((msg.value * (100-(safeSub(rollUnder,1)))) / (safeSub(rollUnder,1))+msg.value))*houseEdge/houseEdgeDivisor)-msg.value;        
		        // safely increase maxPendingPayouts liability - calc all pending payouts under assumption they win //
		        maxPendingPayouts = safeAdd(maxPendingPayouts, playerProfit[rngId]);
		        // check contract can payout on win //
		        if(maxPendingPayouts >= contractBalance) throw;
		        // provides accurate numbers for web3 and allows for manual refunds in case of no oraclize __callback //
		        LogBet(playerBetId[rngId], playerAddress[rngId], safeAdd(playerBetValue[rngId], playerProfit[rngId]), playerProfit[rngId], playerBetValue[rngId], playerNumber[rngId], randomQueryID);          

		    }   
		             
		  
		  */

		  //cout << "playerRollDice executes" << endl;
		  int64_t rollUnder = hexToint64("0x" + input.substr(10,64));
		  tc->playerRollDice(dataItem, senderAddress, rollUnder);
		  cout << "playerRollDice executed" << endl;
		} else if (fxHash.compare("0xad8733ca") == 0)
		{
		  // https://etherscan.io/address/0xb59fdff77a6175dfa4fe7af4281a52f61611eaa2#code

		  /*

		   function multisend(address _tokenAddr, address[] dests, uint256[] values)
		      onlyOwner
		      returns (uint256) {
		          uint256 i = 0;
		          while (i < dests.length) {
		             ERC20(_tokenAddr).transfer(dests[i], values[i]);
		             i += 1;
		          }
		          return(i);
		      }

		  */

		  //cout << "multisend executes" << endl;
		  //cout << input << endl;
		  string _tokenAddr = "0x" + input.substr(10,64);
		  std::vector<string> dests;
		  std::vector<int64_t> values;
		  //int64_t i = tc->multisend(_tokenAddr, dests, values);
		  //return i;
		  cout << "multisend executed" << endl;
		} else if (fxHash.compare("0xa8faf6f0") == 0)
		{
		  // https://etherscan.io/address/0x8bbf4dd0f11b3a535660fd7fcb7158daebd3a17e#code
		  //cout << "SmartAirdrop executes" << endl;

		  cout << "SmartAirdrop executed" << endl;
		} else if (fxHash.compare("0x87ccccb3") == 0)
		{
		  // https://etherscan.io/address/0x5f6e7fb7fe92ea7822472bb0e8f1be60d6a4ea50#code
		  //cout << "PublicMine executes" << endl;

		  cout << "PublicMine executed" << endl;
		} else if (fxHash.compare("0x0d571742") == 0)
		{
		  // https://etherscan.io/address/0x5f6e7fb7fe92ea7822472bb0e8f1be60d6a4ea50#code
		  //cout << "setGenesisAddress executes" << endl;

		  cout << "setGenesisAddress executed" << endl;
		}
		else {
		  //cout << input.size() << endl << input << endl;  
		  //cout << "wrong input" << endl;
		  //exit(-1);
		}
	}
}

