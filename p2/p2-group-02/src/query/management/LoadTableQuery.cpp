//
// Created by liu on 18-10-25.
//

#include "LoadTableQuery.h"

#include <fstream>

#include "../../db/Database.h"

constexpr const char *LoadTableQuery::qname;

QueryResult::Ptr LoadTableQuery::execute() {
  using std::exception;
  using std::ifstream;
  using std::make_unique;
  Database &db = Database::getInstance();
  try {
    ifstream infile(this->fileName);
    if (!infile.is_open()) {
      return make_unique<ErrorMsgResult>(qname, "Cannot open file '?'"_f %
                                                    this->fileName);
    }
    db.loadTableFromStream(infile, this->fileName);
    infile.close();
    return make_unique<NullQueryResult>();
  } catch (const exception &e) {
    return make_unique<ErrorMsgResult>(qname, e.what());
  }
}

std::string LoadTableQuery::toString() {
  return "QUERY = Load TABLE, FILE = \"" + this->fileName + "\"";
}
