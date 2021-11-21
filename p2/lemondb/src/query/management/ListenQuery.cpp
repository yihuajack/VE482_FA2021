//
// Created by alaallen on 11/19/21.
//

#include "ListenQuery.h"
#include <iostream>
#include <fstream>
#include "../../db/Database.h"
#include "../../utils/io.h"
constexpr const char *ListenQuery::qname;

QueryResult::Ptr ListenQuery::execute() {
  using std::exception;
  using std::fstream;
  using std::istream;
  using std::string;
  using std::make_unique;
  try {
    auto* newfin = new fstream(this->fileName);
    if (!newfin->is_open()){
      return make_unique<ErrorMsgResult>("Error: could not open "+this->fileName);
    }
    auto* pt = new istream(newfin->rdbuf());
    auto &isPool = IO::getISPool();
    isPool.push_back(pt);
    std::string filename;
    int it;
    for(it = (int)this->fileName.length()-1; it > -1; it--){
      if(this->fileName[static_cast<unsigned long>(it)] == '/') break;
    }
    filename.assign(this->fileName, static_cast<unsigned long>(it + 1), this->fileName.length() -
                                                                        static_cast<unsigned long>(it));
    return make_unique<SuccessMsgResult>("ANSWER = ( listening from "+ filename +" )");
  } catch (const exception &e) {
    return make_unique<ErrorMsgResult>(qname, e.what());
  }
}

std::string ListenQuery::toString() {
    return "QUERY = LISTEN, FileName = \"" + this->fileName + "\"";
}