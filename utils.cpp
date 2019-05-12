#include <iostream>
#include <bits/stdc++.h>
#include <string>
#include "utils.h"
#include <openssl/sha.h>
#include <sstream>
#include <iomanip>

using namespace std;

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
