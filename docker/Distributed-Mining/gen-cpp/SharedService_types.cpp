/**
 * Autogenerated by Thrift Compiler (0.12.0)
 *
 * DO NOT EDIT UNLESS YOU ARE SURE THAT YOU KNOW WHAT YOU ARE DOING
 *  @generated
 */
#include "SharedService_types.h"

#include <algorithm>
#include <ostream>

#include <thrift/TToString.h>

namespace SharedService {


Transaction::~Transaction() throw() {
}


void Transaction::__set_transactionID(const int16_t val) {
  this->transactionID = val;
}

void Transaction::__set_fromAddress(const std::string& val) {
  this->fromAddress = val;
}

void Transaction::__set_toAddress(const std::string& val) {
  this->toAddress = val;
}

void Transaction::__set_value(const double val) {
  this->value = val;
}

void Transaction::__set_gas(const double val) {
  this->gas = val;
}

void Transaction::__set_gasPrice(const double val) {
  this->gasPrice = val;
}
std::ostream& operator<<(std::ostream& out, const Transaction& obj)
{
  obj.printTo(out);
  return out;
}


uint32_t Transaction::read(::apache::thrift::protocol::TProtocol* iprot) {

  ::apache::thrift::protocol::TInputRecursionTracker tracker(*iprot);
  uint32_t xfer = 0;
  std::string fname;
  ::apache::thrift::protocol::TType ftype;
  int16_t fid;

  xfer += iprot->readStructBegin(fname);

  using ::apache::thrift::protocol::TProtocolException;

  bool isset_transactionID = false;
  bool isset_fromAddress = false;
  bool isset_toAddress = false;
  bool isset_value = false;
  bool isset_gas = false;
  bool isset_gasPrice = false;

  while (true)
  {
    xfer += iprot->readFieldBegin(fname, ftype, fid);
    if (ftype == ::apache::thrift::protocol::T_STOP) {
      break;
    }
    switch (fid)
    {
      case 1:
        if (ftype == ::apache::thrift::protocol::T_I16) {
          xfer += iprot->readI16(this->transactionID);
          isset_transactionID = true;
        } else {
          xfer += iprot->skip(ftype);
        }
        break;
      case 2:
        if (ftype == ::apache::thrift::protocol::T_STRING) {
          xfer += iprot->readString(this->fromAddress);
          isset_fromAddress = true;
        } else {
          xfer += iprot->skip(ftype);
        }
        break;
      case 3:
        if (ftype == ::apache::thrift::protocol::T_STRING) {
          xfer += iprot->readString(this->toAddress);
          isset_toAddress = true;
        } else {
          xfer += iprot->skip(ftype);
        }
        break;
      case 4:
        if (ftype == ::apache::thrift::protocol::T_DOUBLE) {
          xfer += iprot->readDouble(this->value);
          isset_value = true;
        } else {
          xfer += iprot->skip(ftype);
        }
        break;
      case 5:
        if (ftype == ::apache::thrift::protocol::T_DOUBLE) {
          xfer += iprot->readDouble(this->gas);
          isset_gas = true;
        } else {
          xfer += iprot->skip(ftype);
        }
        break;
      case 6:
        if (ftype == ::apache::thrift::protocol::T_DOUBLE) {
          xfer += iprot->readDouble(this->gasPrice);
          isset_gasPrice = true;
        } else {
          xfer += iprot->skip(ftype);
        }
        break;
      default:
        xfer += iprot->skip(ftype);
        break;
    }
    xfer += iprot->readFieldEnd();
  }

  xfer += iprot->readStructEnd();

  if (!isset_transactionID)
    throw TProtocolException(TProtocolException::INVALID_DATA);
  if (!isset_fromAddress)
    throw TProtocolException(TProtocolException::INVALID_DATA);
  if (!isset_toAddress)
    throw TProtocolException(TProtocolException::INVALID_DATA);
  if (!isset_value)
    throw TProtocolException(TProtocolException::INVALID_DATA);
  if (!isset_gas)
    throw TProtocolException(TProtocolException::INVALID_DATA);
  if (!isset_gasPrice)
    throw TProtocolException(TProtocolException::INVALID_DATA);
  return xfer;
}

uint32_t Transaction::write(::apache::thrift::protocol::TProtocol* oprot) const {
  uint32_t xfer = 0;
  ::apache::thrift::protocol::TOutputRecursionTracker tracker(*oprot);
  xfer += oprot->writeStructBegin("Transaction");

  xfer += oprot->writeFieldBegin("transactionID", ::apache::thrift::protocol::T_I16, 1);
  xfer += oprot->writeI16(this->transactionID);
  xfer += oprot->writeFieldEnd();

  xfer += oprot->writeFieldBegin("fromAddress", ::apache::thrift::protocol::T_STRING, 2);
  xfer += oprot->writeString(this->fromAddress);
  xfer += oprot->writeFieldEnd();

  xfer += oprot->writeFieldBegin("toAddress", ::apache::thrift::protocol::T_STRING, 3);
  xfer += oprot->writeString(this->toAddress);
  xfer += oprot->writeFieldEnd();

  xfer += oprot->writeFieldBegin("value", ::apache::thrift::protocol::T_DOUBLE, 4);
  xfer += oprot->writeDouble(this->value);
  xfer += oprot->writeFieldEnd();

  xfer += oprot->writeFieldBegin("gas", ::apache::thrift::protocol::T_DOUBLE, 5);
  xfer += oprot->writeDouble(this->gas);
  xfer += oprot->writeFieldEnd();

  xfer += oprot->writeFieldBegin("gasPrice", ::apache::thrift::protocol::T_DOUBLE, 6);
  xfer += oprot->writeDouble(this->gasPrice);
  xfer += oprot->writeFieldEnd();

  xfer += oprot->writeFieldStop();
  xfer += oprot->writeStructEnd();
  return xfer;
}

void swap(Transaction &a, Transaction &b) {
  using ::std::swap;
  swap(a.transactionID, b.transactionID);
  swap(a.fromAddress, b.fromAddress);
  swap(a.toAddress, b.toAddress);
  swap(a.value, b.value);
  swap(a.gas, b.gas);
  swap(a.gasPrice, b.gasPrice);
}

Transaction::Transaction(const Transaction& other0) {
  transactionID = other0.transactionID;
  fromAddress = other0.fromAddress;
  toAddress = other0.toAddress;
  value = other0.value;
  gas = other0.gas;
  gasPrice = other0.gasPrice;
}
Transaction& Transaction::operator=(const Transaction& other1) {
  transactionID = other1.transactionID;
  fromAddress = other1.fromAddress;
  toAddress = other1.toAddress;
  value = other1.value;
  gas = other1.gas;
  gasPrice = other1.gasPrice;
  return *this;
}
void Transaction::printTo(std::ostream& out) const {
  using ::apache::thrift::to_string;
  out << "Transaction(";
  out << "transactionID=" << to_string(transactionID);
  out << ", " << "fromAddress=" << to_string(fromAddress);
  out << ", " << "toAddress=" << to_string(toAddress);
  out << ", " << "value=" << to_string(value);
  out << ", " << "gas=" << to_string(gas);
  out << ", " << "gasPrice=" << to_string(gasPrice);
  out << ")";
}


Account::~Account() throw() {
}


void Account::__set_address(const std::string& val) {
  this->address = val;
}

void Account::__set_value(const double val) {
  this->value = val;
}

void Account::__set_privateKey(const std::string& val) {
  this->privateKey = val;
__isset.privateKey = true;
}

void Account::__set_publicKey(const std::string& val) {
  this->publicKey = val;
__isset.publicKey = true;
}
std::ostream& operator<<(std::ostream& out, const Account& obj)
{
  obj.printTo(out);
  return out;
}


uint32_t Account::read(::apache::thrift::protocol::TProtocol* iprot) {

  ::apache::thrift::protocol::TInputRecursionTracker tracker(*iprot);
  uint32_t xfer = 0;
  std::string fname;
  ::apache::thrift::protocol::TType ftype;
  int16_t fid;

  xfer += iprot->readStructBegin(fname);

  using ::apache::thrift::protocol::TProtocolException;

  bool isset_address = false;
  bool isset_value = false;

  while (true)
  {
    xfer += iprot->readFieldBegin(fname, ftype, fid);
    if (ftype == ::apache::thrift::protocol::T_STOP) {
      break;
    }
    switch (fid)
    {
      case 1:
        if (ftype == ::apache::thrift::protocol::T_STRING) {
          xfer += iprot->readString(this->address);
          isset_address = true;
        } else {
          xfer += iprot->skip(ftype);
        }
        break;
      case 2:
        if (ftype == ::apache::thrift::protocol::T_DOUBLE) {
          xfer += iprot->readDouble(this->value);
          isset_value = true;
        } else {
          xfer += iprot->skip(ftype);
        }
        break;
      case 3:
        if (ftype == ::apache::thrift::protocol::T_STRING) {
          xfer += iprot->readString(this->privateKey);
          this->__isset.privateKey = true;
        } else {
          xfer += iprot->skip(ftype);
        }
        break;
      case 4:
        if (ftype == ::apache::thrift::protocol::T_STRING) {
          xfer += iprot->readString(this->publicKey);
          this->__isset.publicKey = true;
        } else {
          xfer += iprot->skip(ftype);
        }
        break;
      default:
        xfer += iprot->skip(ftype);
        break;
    }
    xfer += iprot->readFieldEnd();
  }

  xfer += iprot->readStructEnd();

  if (!isset_address)
    throw TProtocolException(TProtocolException::INVALID_DATA);
  if (!isset_value)
    throw TProtocolException(TProtocolException::INVALID_DATA);
  return xfer;
}

uint32_t Account::write(::apache::thrift::protocol::TProtocol* oprot) const {
  uint32_t xfer = 0;
  ::apache::thrift::protocol::TOutputRecursionTracker tracker(*oprot);
  xfer += oprot->writeStructBegin("Account");

  xfer += oprot->writeFieldBegin("address", ::apache::thrift::protocol::T_STRING, 1);
  xfer += oprot->writeString(this->address);
  xfer += oprot->writeFieldEnd();

  xfer += oprot->writeFieldBegin("value", ::apache::thrift::protocol::T_DOUBLE, 2);
  xfer += oprot->writeDouble(this->value);
  xfer += oprot->writeFieldEnd();

  if (this->__isset.privateKey) {
    xfer += oprot->writeFieldBegin("privateKey", ::apache::thrift::protocol::T_STRING, 3);
    xfer += oprot->writeString(this->privateKey);
    xfer += oprot->writeFieldEnd();
  }
  if (this->__isset.publicKey) {
    xfer += oprot->writeFieldBegin("publicKey", ::apache::thrift::protocol::T_STRING, 4);
    xfer += oprot->writeString(this->publicKey);
    xfer += oprot->writeFieldEnd();
  }
  xfer += oprot->writeFieldStop();
  xfer += oprot->writeStructEnd();
  return xfer;
}

void swap(Account &a, Account &b) {
  using ::std::swap;
  swap(a.address, b.address);
  swap(a.value, b.value);
  swap(a.privateKey, b.privateKey);
  swap(a.publicKey, b.publicKey);
  swap(a.__isset, b.__isset);
}

Account::Account(const Account& other2) {
  address = other2.address;
  value = other2.value;
  privateKey = other2.privateKey;
  publicKey = other2.publicKey;
  __isset = other2.__isset;
}
Account& Account::operator=(const Account& other3) {
  address = other3.address;
  value = other3.value;
  privateKey = other3.privateKey;
  publicKey = other3.publicKey;
  __isset = other3.__isset;
  return *this;
}
void Account::printTo(std::ostream& out) const {
  using ::apache::thrift::to_string;
  out << "Account(";
  out << "address=" << to_string(address);
  out << ", " << "value=" << to_string(value);
  out << ", " << "privateKey="; (__isset.privateKey ? (out << to_string(privateKey)) : (out << "<null>"));
  out << ", " << "publicKey="; (__isset.publicKey ? (out << to_string(publicKey)) : (out << "<null>"));
  out << ")";
}


WorkerNode::~WorkerNode() throw() {
}


void WorkerNode::__set_workerID(const int16_t val) {
  this->workerID = val;
}

void WorkerNode::__set_workerIP(const std::string& val) {
  this->workerIP = val;
}

void WorkerNode::__set_workerPort(const int32_t val) {
  this->workerPort = val;
}
std::ostream& operator<<(std::ostream& out, const WorkerNode& obj)
{
  obj.printTo(out);
  return out;
}


uint32_t WorkerNode::read(::apache::thrift::protocol::TProtocol* iprot) {

  ::apache::thrift::protocol::TInputRecursionTracker tracker(*iprot);
  uint32_t xfer = 0;
  std::string fname;
  ::apache::thrift::protocol::TType ftype;
  int16_t fid;

  xfer += iprot->readStructBegin(fname);

  using ::apache::thrift::protocol::TProtocolException;

  bool isset_workerID = false;
  bool isset_workerIP = false;
  bool isset_workerPort = false;

  while (true)
  {
    xfer += iprot->readFieldBegin(fname, ftype, fid);
    if (ftype == ::apache::thrift::protocol::T_STOP) {
      break;
    }
    switch (fid)
    {
      case 1:
        if (ftype == ::apache::thrift::protocol::T_I16) {
          xfer += iprot->readI16(this->workerID);
          isset_workerID = true;
        } else {
          xfer += iprot->skip(ftype);
        }
        break;
      case 2:
        if (ftype == ::apache::thrift::protocol::T_STRING) {
          xfer += iprot->readString(this->workerIP);
          isset_workerIP = true;
        } else {
          xfer += iprot->skip(ftype);
        }
        break;
      case 3:
        if (ftype == ::apache::thrift::protocol::T_I32) {
          xfer += iprot->readI32(this->workerPort);
          isset_workerPort = true;
        } else {
          xfer += iprot->skip(ftype);
        }
        break;
      default:
        xfer += iprot->skip(ftype);
        break;
    }
    xfer += iprot->readFieldEnd();
  }

  xfer += iprot->readStructEnd();

  if (!isset_workerID)
    throw TProtocolException(TProtocolException::INVALID_DATA);
  if (!isset_workerIP)
    throw TProtocolException(TProtocolException::INVALID_DATA);
  if (!isset_workerPort)
    throw TProtocolException(TProtocolException::INVALID_DATA);
  return xfer;
}

uint32_t WorkerNode::write(::apache::thrift::protocol::TProtocol* oprot) const {
  uint32_t xfer = 0;
  ::apache::thrift::protocol::TOutputRecursionTracker tracker(*oprot);
  xfer += oprot->writeStructBegin("WorkerNode");

  xfer += oprot->writeFieldBegin("workerID", ::apache::thrift::protocol::T_I16, 1);
  xfer += oprot->writeI16(this->workerID);
  xfer += oprot->writeFieldEnd();

  xfer += oprot->writeFieldBegin("workerIP", ::apache::thrift::protocol::T_STRING, 2);
  xfer += oprot->writeString(this->workerIP);
  xfer += oprot->writeFieldEnd();

  xfer += oprot->writeFieldBegin("workerPort", ::apache::thrift::protocol::T_I32, 3);
  xfer += oprot->writeI32(this->workerPort);
  xfer += oprot->writeFieldEnd();

  xfer += oprot->writeFieldStop();
  xfer += oprot->writeStructEnd();
  return xfer;
}

void swap(WorkerNode &a, WorkerNode &b) {
  using ::std::swap;
  swap(a.workerID, b.workerID);
  swap(a.workerIP, b.workerIP);
  swap(a.workerPort, b.workerPort);
}

WorkerNode::WorkerNode(const WorkerNode& other4) {
  workerID = other4.workerID;
  workerIP = other4.workerIP;
  workerPort = other4.workerPort;
}
WorkerNode& WorkerNode::operator=(const WorkerNode& other5) {
  workerID = other5.workerID;
  workerIP = other5.workerIP;
  workerPort = other5.workerPort;
  return *this;
}
void WorkerNode::printTo(std::ostream& out) const {
  using ::apache::thrift::to_string;
  out << "WorkerNode(";
  out << "workerID=" << to_string(workerID);
  out << ", " << "workerIP=" << to_string(workerIP);
  out << ", " << "workerPort=" << to_string(workerPort);
  out << ")";
}


Uncle::~Uncle() throw() {
}


void Uncle::__set_miner(const std::string& val) {
  this->miner = val;
}

void Uncle::__set_number(const int32_t val) {
  this->number = val;
}
std::ostream& operator<<(std::ostream& out, const Uncle& obj)
{
  obj.printTo(out);
  return out;
}


uint32_t Uncle::read(::apache::thrift::protocol::TProtocol* iprot) {

  ::apache::thrift::protocol::TInputRecursionTracker tracker(*iprot);
  uint32_t xfer = 0;
  std::string fname;
  ::apache::thrift::protocol::TType ftype;
  int16_t fid;

  xfer += iprot->readStructBegin(fname);

  using ::apache::thrift::protocol::TProtocolException;

  bool isset_miner = false;
  bool isset_number = false;

  while (true)
  {
    xfer += iprot->readFieldBegin(fname, ftype, fid);
    if (ftype == ::apache::thrift::protocol::T_STOP) {
      break;
    }
    switch (fid)
    {
      case 1:
        if (ftype == ::apache::thrift::protocol::T_STRING) {
          xfer += iprot->readString(this->miner);
          isset_miner = true;
        } else {
          xfer += iprot->skip(ftype);
        }
        break;
      case 2:
        if (ftype == ::apache::thrift::protocol::T_I32) {
          xfer += iprot->readI32(this->number);
          isset_number = true;
        } else {
          xfer += iprot->skip(ftype);
        }
        break;
      default:
        xfer += iprot->skip(ftype);
        break;
    }
    xfer += iprot->readFieldEnd();
  }

  xfer += iprot->readStructEnd();

  if (!isset_miner)
    throw TProtocolException(TProtocolException::INVALID_DATA);
  if (!isset_number)
    throw TProtocolException(TProtocolException::INVALID_DATA);
  return xfer;
}

uint32_t Uncle::write(::apache::thrift::protocol::TProtocol* oprot) const {
  uint32_t xfer = 0;
  ::apache::thrift::protocol::TOutputRecursionTracker tracker(*oprot);
  xfer += oprot->writeStructBegin("Uncle");

  xfer += oprot->writeFieldBegin("miner", ::apache::thrift::protocol::T_STRING, 1);
  xfer += oprot->writeString(this->miner);
  xfer += oprot->writeFieldEnd();

  xfer += oprot->writeFieldBegin("number", ::apache::thrift::protocol::T_I32, 2);
  xfer += oprot->writeI32(this->number);
  xfer += oprot->writeFieldEnd();

  xfer += oprot->writeFieldStop();
  xfer += oprot->writeStructEnd();
  return xfer;
}

void swap(Uncle &a, Uncle &b) {
  using ::std::swap;
  swap(a.miner, b.miner);
  swap(a.number, b.number);
}

Uncle::Uncle(const Uncle& other6) {
  miner = other6.miner;
  number = other6.number;
}
Uncle& Uncle::operator=(const Uncle& other7) {
  miner = other7.miner;
  number = other7.number;
  return *this;
}
void Uncle::printTo(std::ostream& out) const {
  using ::apache::thrift::to_string;
  out << "Uncle(";
  out << "miner=" << to_string(miner);
  out << ", " << "number=" << to_string(number);
  out << ")";
}


WorkerResponse::~WorkerResponse() throw() {
}


void WorkerResponse::__set_accountList(const std::map<std::string, double> & val) {
  this->accountList = val;
}

void WorkerResponse::__set_transactionIDList(const std::vector<int16_t> & val) {
  this->transactionIDList = val;
}

void WorkerResponse::__set_transactionFees(const double val) {
  this->transactionFees = val;
}
std::ostream& operator<<(std::ostream& out, const WorkerResponse& obj)
{
  obj.printTo(out);
  return out;
}


uint32_t WorkerResponse::read(::apache::thrift::protocol::TProtocol* iprot) {

  ::apache::thrift::protocol::TInputRecursionTracker tracker(*iprot);
  uint32_t xfer = 0;
  std::string fname;
  ::apache::thrift::protocol::TType ftype;
  int16_t fid;

  xfer += iprot->readStructBegin(fname);

  using ::apache::thrift::protocol::TProtocolException;

  bool isset_accountList = false;
  bool isset_transactionIDList = false;
  bool isset_transactionFees = false;

  while (true)
  {
    xfer += iprot->readFieldBegin(fname, ftype, fid);
    if (ftype == ::apache::thrift::protocol::T_STOP) {
      break;
    }
    switch (fid)
    {
      case 1:
        if (ftype == ::apache::thrift::protocol::T_MAP) {
          {
            this->accountList.clear();
            uint32_t _size8;
            ::apache::thrift::protocol::TType _ktype9;
            ::apache::thrift::protocol::TType _vtype10;
            xfer += iprot->readMapBegin(_ktype9, _vtype10, _size8);
            uint32_t _i12;
            for (_i12 = 0; _i12 < _size8; ++_i12)
            {
              std::string _key13;
              xfer += iprot->readString(_key13);
              double& _val14 = this->accountList[_key13];
              xfer += iprot->readDouble(_val14);
            }
            xfer += iprot->readMapEnd();
          }
          isset_accountList = true;
        } else {
          xfer += iprot->skip(ftype);
        }
        break;
      case 2:
        if (ftype == ::apache::thrift::protocol::T_LIST) {
          {
            this->transactionIDList.clear();
            uint32_t _size15;
            ::apache::thrift::protocol::TType _etype18;
            xfer += iprot->readListBegin(_etype18, _size15);
            this->transactionIDList.resize(_size15);
            uint32_t _i19;
            for (_i19 = 0; _i19 < _size15; ++_i19)
            {
              xfer += iprot->readI16(this->transactionIDList[_i19]);
            }
            xfer += iprot->readListEnd();
          }
          isset_transactionIDList = true;
        } else {
          xfer += iprot->skip(ftype);
        }
        break;
      case 3:
        if (ftype == ::apache::thrift::protocol::T_DOUBLE) {
          xfer += iprot->readDouble(this->transactionFees);
          isset_transactionFees = true;
        } else {
          xfer += iprot->skip(ftype);
        }
        break;
      default:
        xfer += iprot->skip(ftype);
        break;
    }
    xfer += iprot->readFieldEnd();
  }

  xfer += iprot->readStructEnd();

  if (!isset_accountList)
    throw TProtocolException(TProtocolException::INVALID_DATA);
  if (!isset_transactionIDList)
    throw TProtocolException(TProtocolException::INVALID_DATA);
  if (!isset_transactionFees)
    throw TProtocolException(TProtocolException::INVALID_DATA);
  return xfer;
}

uint32_t WorkerResponse::write(::apache::thrift::protocol::TProtocol* oprot) const {
  uint32_t xfer = 0;
  ::apache::thrift::protocol::TOutputRecursionTracker tracker(*oprot);
  xfer += oprot->writeStructBegin("WorkerResponse");

  xfer += oprot->writeFieldBegin("accountList", ::apache::thrift::protocol::T_MAP, 1);
  {
    xfer += oprot->writeMapBegin(::apache::thrift::protocol::T_STRING, ::apache::thrift::protocol::T_DOUBLE, static_cast<uint32_t>(this->accountList.size()));
    std::map<std::string, double> ::const_iterator _iter20;
    for (_iter20 = this->accountList.begin(); _iter20 != this->accountList.end(); ++_iter20)
    {
      xfer += oprot->writeString(_iter20->first);
      xfer += oprot->writeDouble(_iter20->second);
    }
    xfer += oprot->writeMapEnd();
  }
  xfer += oprot->writeFieldEnd();

  xfer += oprot->writeFieldBegin("transactionIDList", ::apache::thrift::protocol::T_LIST, 2);
  {
    xfer += oprot->writeListBegin(::apache::thrift::protocol::T_I16, static_cast<uint32_t>(this->transactionIDList.size()));
    std::vector<int16_t> ::const_iterator _iter21;
    for (_iter21 = this->transactionIDList.begin(); _iter21 != this->transactionIDList.end(); ++_iter21)
    {
      xfer += oprot->writeI16((*_iter21));
    }
    xfer += oprot->writeListEnd();
  }
  xfer += oprot->writeFieldEnd();

  xfer += oprot->writeFieldBegin("transactionFees", ::apache::thrift::protocol::T_DOUBLE, 3);
  xfer += oprot->writeDouble(this->transactionFees);
  xfer += oprot->writeFieldEnd();

  xfer += oprot->writeFieldStop();
  xfer += oprot->writeStructEnd();
  return xfer;
}

void swap(WorkerResponse &a, WorkerResponse &b) {
  using ::std::swap;
  swap(a.accountList, b.accountList);
  swap(a.transactionIDList, b.transactionIDList);
  swap(a.transactionFees, b.transactionFees);
}

WorkerResponse::WorkerResponse(const WorkerResponse& other22) {
  accountList = other22.accountList;
  transactionIDList = other22.transactionIDList;
  transactionFees = other22.transactionFees;
}
WorkerResponse& WorkerResponse::operator=(const WorkerResponse& other23) {
  accountList = other23.accountList;
  transactionIDList = other23.transactionIDList;
  transactionFees = other23.transactionFees;
  return *this;
}
void WorkerResponse::printTo(std::ostream& out) const {
  using ::apache::thrift::to_string;
  out << "WorkerResponse(";
  out << "accountList=" << to_string(accountList);
  out << ", " << "transactionIDList=" << to_string(transactionIDList);
  out << ", " << "transactionFees=" << to_string(transactionFees);
  out << ")";
}

} // namespace
