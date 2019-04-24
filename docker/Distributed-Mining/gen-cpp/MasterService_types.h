/**
 * Autogenerated by Thrift Compiler (0.12.0)
 *
 * DO NOT EDIT UNLESS YOU ARE SURE THAT YOU KNOW WHAT YOU ARE DOING
 *  @generated
 */
#ifndef MasterService_TYPES_H
#define MasterService_TYPES_H

#include <iosfwd>

#include <thrift/Thrift.h>
#include <thrift/TApplicationException.h>
#include <thrift/TBase.h>
#include <thrift/protocol/TProtocol.h>
#include <thrift/transport/TTransport.h>

#include <thrift/stdcxx.h>
#include "SharedService_types.h"


namespace MasterService {

class MasterNode;

class Block;


class MasterNode : public virtual ::apache::thrift::TBase {
 public:

  MasterNode(const MasterNode&);
  MasterNode& operator=(const MasterNode&);
  MasterNode() : masterID(0), masterIP(), masterPort(0) {
  }

  virtual ~MasterNode() throw();
  int16_t masterID;
  std::string masterIP;
  int32_t masterPort;

  void __set_masterID(const int16_t val);

  void __set_masterIP(const std::string& val);

  void __set_masterPort(const int32_t val);

  bool operator == (const MasterNode & rhs) const
  {
    if (!(masterID == rhs.masterID))
      return false;
    if (!(masterIP == rhs.masterIP))
      return false;
    if (!(masterPort == rhs.masterPort))
      return false;
    return true;
  }
  bool operator != (const MasterNode &rhs) const {
    return !(*this == rhs);
  }

  bool operator < (const MasterNode & ) const;

  uint32_t read(::apache::thrift::protocol::TProtocol* iprot);
  uint32_t write(::apache::thrift::protocol::TProtocol* oprot) const;

  virtual void printTo(std::ostream& out) const;
};

void swap(MasterNode &a, MasterNode &b);

std::ostream& operator<<(std::ostream& out, const MasterNode& obj);


class Block : public virtual ::apache::thrift::TBase {
 public:

  Block(const Block&);
  Block& operator=(const Block&);
  Block() : timestamp(), nonce(0), prevHash(), number(0), miner() {
  }

  virtual ~Block() throw();
  std::string timestamp;
  int64_t nonce;
  std::string prevHash;
  int32_t number;
  std::string miner;
  std::vector< ::SharedService::Transaction>  transactionsList;
  std::vector< ::SharedService::Uncle>  unclesList;

  void __set_timestamp(const std::string& val);

  void __set_nonce(const int64_t val);

  void __set_prevHash(const std::string& val);

  void __set_number(const int32_t val);

  void __set_miner(const std::string& val);

  void __set_transactionsList(const std::vector< ::SharedService::Transaction> & val);

  void __set_unclesList(const std::vector< ::SharedService::Uncle> & val);

  bool operator == (const Block & rhs) const
  {
    if (!(timestamp == rhs.timestamp))
      return false;
    if (!(nonce == rhs.nonce))
      return false;
    if (!(prevHash == rhs.prevHash))
      return false;
    if (!(number == rhs.number))
      return false;
    if (!(miner == rhs.miner))
      return false;
    if (!(transactionsList == rhs.transactionsList))
      return false;
    if (!(unclesList == rhs.unclesList))
      return false;
    return true;
  }
  bool operator != (const Block &rhs) const {
    return !(*this == rhs);
  }

  bool operator < (const Block & ) const;

  uint32_t read(::apache::thrift::protocol::TProtocol* iprot);
  uint32_t write(::apache::thrift::protocol::TProtocol* oprot) const;

  virtual void printTo(std::ostream& out) const;
};

void swap(Block &a, Block &b);

std::ostream& operator<<(std::ostream& out, const Block& obj);

} // namespace

#endif