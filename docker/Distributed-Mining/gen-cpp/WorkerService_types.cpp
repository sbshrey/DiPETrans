/**
 * Autogenerated by Thrift Compiler (0.12.0)
 *
 * DO NOT EDIT UNLESS YOU ARE SURE THAT YOU KNOW WHAT YOU ARE DOING
 *  @generated
 */
#include "WorkerService_types.h"

#include <algorithm>
#include <ostream>

#include <thrift/TToString.h>

namespace WorkerService {


result::~result() throw() {
}


void result::__set_number(const int32_t val) {
  this->number = val;
}

void result::__set_nonce(const int64_t val) {
  this->nonce = val;
}
std::ostream& operator<<(std::ostream& out, const result& obj)
{
  obj.printTo(out);
  return out;
}


uint32_t result::read(::apache::thrift::protocol::TProtocol* iprot) {

  ::apache::thrift::protocol::TInputRecursionTracker tracker(*iprot);
  uint32_t xfer = 0;
  std::string fname;
  ::apache::thrift::protocol::TType ftype;
  int16_t fid;

  xfer += iprot->readStructBegin(fname);

  using ::apache::thrift::protocol::TProtocolException;

  bool isset_number = false;
  bool isset_nonce = false;

  while (true)
  {
    xfer += iprot->readFieldBegin(fname, ftype, fid);
    if (ftype == ::apache::thrift::protocol::T_STOP) {
      break;
    }
    switch (fid)
    {
      case 1:
        if (ftype == ::apache::thrift::protocol::T_I32) {
          xfer += iprot->readI32(this->number);
          isset_number = true;
        } else {
          xfer += iprot->skip(ftype);
        }
        break;
      case 2:
        if (ftype == ::apache::thrift::protocol::T_I64) {
          xfer += iprot->readI64(this->nonce);
          isset_nonce = true;
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

  if (!isset_number)
    throw TProtocolException(TProtocolException::INVALID_DATA);
  if (!isset_nonce)
    throw TProtocolException(TProtocolException::INVALID_DATA);
  return xfer;
}

uint32_t result::write(::apache::thrift::protocol::TProtocol* oprot) const {
  uint32_t xfer = 0;
  ::apache::thrift::protocol::TOutputRecursionTracker tracker(*oprot);
  xfer += oprot->writeStructBegin("result");

  xfer += oprot->writeFieldBegin("number", ::apache::thrift::protocol::T_I32, 1);
  xfer += oprot->writeI32(this->number);
  xfer += oprot->writeFieldEnd();

  xfer += oprot->writeFieldBegin("nonce", ::apache::thrift::protocol::T_I64, 2);
  xfer += oprot->writeI64(this->nonce);
  xfer += oprot->writeFieldEnd();

  xfer += oprot->writeFieldStop();
  xfer += oprot->writeStructEnd();
  return xfer;
}

void swap(result &a, result &b) {
  using ::std::swap;
  swap(a.number, b.number);
  swap(a.nonce, b.nonce);
}

result::result(const result& other0) {
  number = other0.number;
  nonce = other0.nonce;
}
result& result::operator=(const result& other1) {
  number = other1.number;
  nonce = other1.nonce;
  return *this;
}
void result::printTo(std::ostream& out) const {
  using ::apache::thrift::to_string;
  out << "result(";
  out << "number=" << to_string(number);
  out << ", " << "nonce=" << to_string(nonce);
  out << ")";
}

} // namespace
