/**
 * Autogenerated by Thrift Compiler (0.12.0)
 *
 * DO NOT EDIT UNLESS YOU ARE SURE THAT YOU KNOW WHAT YOU ARE DOING
 *  @generated
 */
#ifndef MasterService_H
#define MasterService_H

#include <thrift/TDispatchProcessor.h>
#include <thrift/async/TConcurrentClientSyncInfo.h>
#include "MasterService_types.h"
#include "SharedService.h"

namespace MasterService {

#ifdef _MSC_VER
  #pragma warning( push )
  #pragma warning (disable : 4250 ) //inheriting methods via dominance 
#endif

class MasterServiceIf : virtual public  ::SharedService::SharedServiceIf {
 public:
  virtual ~MasterServiceIf() {}
  virtual void processBlocks() = 0;
  virtual void recvMiningStatus(const int64_t nonce, const int32_t number) = 0;
};

class MasterServiceIfFactory : virtual public  ::SharedService::SharedServiceIfFactory {
 public:
  typedef MasterServiceIf Handler;

  virtual ~MasterServiceIfFactory() {}

  virtual MasterServiceIf* getHandler(const ::apache::thrift::TConnectionInfo& connInfo) = 0;
  virtual void releaseHandler( ::SharedService::SharedServiceIf* /* handler */) = 0;
};

class MasterServiceIfSingletonFactory : virtual public MasterServiceIfFactory {
 public:
  MasterServiceIfSingletonFactory(const ::apache::thrift::stdcxx::shared_ptr<MasterServiceIf>& iface) : iface_(iface) {}
  virtual ~MasterServiceIfSingletonFactory() {}

  virtual MasterServiceIf* getHandler(const ::apache::thrift::TConnectionInfo&) {
    return iface_.get();
  }
  virtual void releaseHandler( ::SharedService::SharedServiceIf* /* handler */) {}

 protected:
  ::apache::thrift::stdcxx::shared_ptr<MasterServiceIf> iface_;
};

class MasterServiceNull : virtual public MasterServiceIf , virtual public  ::SharedService::SharedServiceNull {
 public:
  virtual ~MasterServiceNull() {}
  void processBlocks() {
    return;
  }
  void recvMiningStatus(const int64_t /* nonce */, const int32_t /* number */) {
    return;
  }
};


class MasterService_processBlocks_args {
 public:

  MasterService_processBlocks_args(const MasterService_processBlocks_args&);
  MasterService_processBlocks_args& operator=(const MasterService_processBlocks_args&);
  MasterService_processBlocks_args() {
  }

  virtual ~MasterService_processBlocks_args() throw();

  bool operator == (const MasterService_processBlocks_args & /* rhs */) const
  {
    return true;
  }
  bool operator != (const MasterService_processBlocks_args &rhs) const {
    return !(*this == rhs);
  }

  bool operator < (const MasterService_processBlocks_args & ) const;

  uint32_t read(::apache::thrift::protocol::TProtocol* iprot);
  uint32_t write(::apache::thrift::protocol::TProtocol* oprot) const;

};


class MasterService_processBlocks_pargs {
 public:


  virtual ~MasterService_processBlocks_pargs() throw();

  uint32_t write(::apache::thrift::protocol::TProtocol* oprot) const;

};


class MasterService_processBlocks_result {
 public:

  MasterService_processBlocks_result(const MasterService_processBlocks_result&);
  MasterService_processBlocks_result& operator=(const MasterService_processBlocks_result&);
  MasterService_processBlocks_result() {
  }

  virtual ~MasterService_processBlocks_result() throw();

  bool operator == (const MasterService_processBlocks_result & /* rhs */) const
  {
    return true;
  }
  bool operator != (const MasterService_processBlocks_result &rhs) const {
    return !(*this == rhs);
  }

  bool operator < (const MasterService_processBlocks_result & ) const;

  uint32_t read(::apache::thrift::protocol::TProtocol* iprot);
  uint32_t write(::apache::thrift::protocol::TProtocol* oprot) const;

};


class MasterService_processBlocks_presult {
 public:


  virtual ~MasterService_processBlocks_presult() throw();

  uint32_t read(::apache::thrift::protocol::TProtocol* iprot);

};

typedef struct _MasterService_recvMiningStatus_args__isset {
  _MasterService_recvMiningStatus_args__isset() : nonce(false), number(false) {}
  bool nonce :1;
  bool number :1;
} _MasterService_recvMiningStatus_args__isset;

class MasterService_recvMiningStatus_args {
 public:

  MasterService_recvMiningStatus_args(const MasterService_recvMiningStatus_args&);
  MasterService_recvMiningStatus_args& operator=(const MasterService_recvMiningStatus_args&);
  MasterService_recvMiningStatus_args() : nonce(0), number(0) {
  }

  virtual ~MasterService_recvMiningStatus_args() throw();
  int64_t nonce;
  int32_t number;

  _MasterService_recvMiningStatus_args__isset __isset;

  void __set_nonce(const int64_t val);

  void __set_number(const int32_t val);

  bool operator == (const MasterService_recvMiningStatus_args & rhs) const
  {
    if (!(nonce == rhs.nonce))
      return false;
    if (!(number == rhs.number))
      return false;
    return true;
  }
  bool operator != (const MasterService_recvMiningStatus_args &rhs) const {
    return !(*this == rhs);
  }

  bool operator < (const MasterService_recvMiningStatus_args & ) const;

  uint32_t read(::apache::thrift::protocol::TProtocol* iprot);
  uint32_t write(::apache::thrift::protocol::TProtocol* oprot) const;

};


class MasterService_recvMiningStatus_pargs {
 public:


  virtual ~MasterService_recvMiningStatus_pargs() throw();
  const int64_t* nonce;
  const int32_t* number;

  uint32_t write(::apache::thrift::protocol::TProtocol* oprot) const;

};


class MasterService_recvMiningStatus_result {
 public:

  MasterService_recvMiningStatus_result(const MasterService_recvMiningStatus_result&);
  MasterService_recvMiningStatus_result& operator=(const MasterService_recvMiningStatus_result&);
  MasterService_recvMiningStatus_result() {
  }

  virtual ~MasterService_recvMiningStatus_result() throw();

  bool operator == (const MasterService_recvMiningStatus_result & /* rhs */) const
  {
    return true;
  }
  bool operator != (const MasterService_recvMiningStatus_result &rhs) const {
    return !(*this == rhs);
  }

  bool operator < (const MasterService_recvMiningStatus_result & ) const;

  uint32_t read(::apache::thrift::protocol::TProtocol* iprot);
  uint32_t write(::apache::thrift::protocol::TProtocol* oprot) const;

};


class MasterService_recvMiningStatus_presult {
 public:


  virtual ~MasterService_recvMiningStatus_presult() throw();

  uint32_t read(::apache::thrift::protocol::TProtocol* iprot);

};

class MasterServiceClient : virtual public MasterServiceIf, public  ::SharedService::SharedServiceClient {
 public:
  MasterServiceClient(apache::thrift::stdcxx::shared_ptr< ::apache::thrift::protocol::TProtocol> prot) :
     ::SharedService::SharedServiceClient(prot, prot) {}
  MasterServiceClient(apache::thrift::stdcxx::shared_ptr< ::apache::thrift::protocol::TProtocol> iprot, apache::thrift::stdcxx::shared_ptr< ::apache::thrift::protocol::TProtocol> oprot) :     ::SharedService::SharedServiceClient(iprot, oprot) {}
  apache::thrift::stdcxx::shared_ptr< ::apache::thrift::protocol::TProtocol> getInputProtocol() {
    return piprot_;
  }
  apache::thrift::stdcxx::shared_ptr< ::apache::thrift::protocol::TProtocol> getOutputProtocol() {
    return poprot_;
  }
  void processBlocks();
  void send_processBlocks();
  void recv_processBlocks();
  void recvMiningStatus(const int64_t nonce, const int32_t number);
  void send_recvMiningStatus(const int64_t nonce, const int32_t number);
  void recv_recvMiningStatus();
};

class MasterServiceProcessor : public  ::SharedService::SharedServiceProcessor {
 protected:
  ::apache::thrift::stdcxx::shared_ptr<MasterServiceIf> iface_;
  virtual bool dispatchCall(::apache::thrift::protocol::TProtocol* iprot, ::apache::thrift::protocol::TProtocol* oprot, const std::string& fname, int32_t seqid, void* callContext);
 private:
  typedef  void (MasterServiceProcessor::*ProcessFunction)(int32_t, ::apache::thrift::protocol::TProtocol*, ::apache::thrift::protocol::TProtocol*, void*);
  typedef std::map<std::string, ProcessFunction> ProcessMap;
  ProcessMap processMap_;
  void process_processBlocks(int32_t seqid, ::apache::thrift::protocol::TProtocol* iprot, ::apache::thrift::protocol::TProtocol* oprot, void* callContext);
  void process_recvMiningStatus(int32_t seqid, ::apache::thrift::protocol::TProtocol* iprot, ::apache::thrift::protocol::TProtocol* oprot, void* callContext);
 public:
  MasterServiceProcessor(::apache::thrift::stdcxx::shared_ptr<MasterServiceIf> iface) :
     ::SharedService::SharedServiceProcessor(iface),
    iface_(iface) {
    processMap_["processBlocks"] = &MasterServiceProcessor::process_processBlocks;
    processMap_["recvMiningStatus"] = &MasterServiceProcessor::process_recvMiningStatus;
  }

  virtual ~MasterServiceProcessor() {}
};

class MasterServiceProcessorFactory : public ::apache::thrift::TProcessorFactory {
 public:
  MasterServiceProcessorFactory(const ::apache::thrift::stdcxx::shared_ptr< MasterServiceIfFactory >& handlerFactory) :
      handlerFactory_(handlerFactory) {}

  ::apache::thrift::stdcxx::shared_ptr< ::apache::thrift::TProcessor > getProcessor(const ::apache::thrift::TConnectionInfo& connInfo);

 protected:
  ::apache::thrift::stdcxx::shared_ptr< MasterServiceIfFactory > handlerFactory_;
};

class MasterServiceMultiface : virtual public MasterServiceIf, public  ::SharedService::SharedServiceMultiface {
 public:
  MasterServiceMultiface(std::vector<apache::thrift::stdcxx::shared_ptr<MasterServiceIf> >& ifaces) : ifaces_(ifaces) {
    std::vector<apache::thrift::stdcxx::shared_ptr<MasterServiceIf> >::iterator iter;
    for (iter = ifaces.begin(); iter != ifaces.end(); ++iter) {
       ::SharedService::SharedServiceMultiface::add(*iter);
    }
  }
  virtual ~MasterServiceMultiface() {}
 protected:
  std::vector<apache::thrift::stdcxx::shared_ptr<MasterServiceIf> > ifaces_;
  MasterServiceMultiface() {}
  void add(::apache::thrift::stdcxx::shared_ptr<MasterServiceIf> iface) {
     ::SharedService::SharedServiceMultiface::add(iface);
    ifaces_.push_back(iface);
  }
 public:
  void processBlocks() {
    size_t sz = ifaces_.size();
    size_t i = 0;
    for (; i < (sz - 1); ++i) {
      ifaces_[i]->processBlocks();
    }
    ifaces_[i]->processBlocks();
  }

  void recvMiningStatus(const int64_t nonce, const int32_t number) {
    size_t sz = ifaces_.size();
    size_t i = 0;
    for (; i < (sz - 1); ++i) {
      ifaces_[i]->recvMiningStatus(nonce, number);
    }
    ifaces_[i]->recvMiningStatus(nonce, number);
  }

};

// The 'concurrent' client is a thread safe client that correctly handles
// out of order responses.  It is slower than the regular client, so should
// only be used when you need to share a connection among multiple threads
class MasterServiceConcurrentClient : virtual public MasterServiceIf, public  ::SharedService::SharedServiceConcurrentClient {
 public:
  MasterServiceConcurrentClient(apache::thrift::stdcxx::shared_ptr< ::apache::thrift::protocol::TProtocol> prot) :
     ::SharedService::SharedServiceConcurrentClient(prot, prot) {}
  MasterServiceConcurrentClient(apache::thrift::stdcxx::shared_ptr< ::apache::thrift::protocol::TProtocol> iprot, apache::thrift::stdcxx::shared_ptr< ::apache::thrift::protocol::TProtocol> oprot) :     ::SharedService::SharedServiceConcurrentClient(iprot, oprot) {}
  apache::thrift::stdcxx::shared_ptr< ::apache::thrift::protocol::TProtocol> getInputProtocol() {
    return piprot_;
  }
  apache::thrift::stdcxx::shared_ptr< ::apache::thrift::protocol::TProtocol> getOutputProtocol() {
    return poprot_;
  }
  void processBlocks();
  int32_t send_processBlocks();
  void recv_processBlocks(const int32_t seqid);
  void recvMiningStatus(const int64_t nonce, const int32_t number);
  int32_t send_recvMiningStatus(const int64_t nonce, const int32_t number);
  void recv_recvMiningStatus(const int32_t seqid);
};

#ifdef _MSC_VER
  #pragma warning( pop )
#endif

} // namespace

#endif