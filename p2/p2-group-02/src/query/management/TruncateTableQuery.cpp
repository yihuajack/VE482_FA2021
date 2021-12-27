//
// Created by alaallen on 10/26/21.
//

#include "TruncateTableQuery.h"

#include "../../db/Database.h"

constexpr const char *TruncateTableQuery::qname;

QueryResult::Ptr TruncateTableQuery::execute() {
  using std::exception;
  using std::make_unique;
  Database &db = Database::getInstance();
  try {
    db[this->targetTable].clear();
    // db.dropTable(this->targetTable);
    return make_unique<NullQueryResult>();
  } catch (const exception &e) {
    return make_unique<ErrorMsgResult>(qname, e.what());
  }
}

std::string TruncateTableQuery::toString() {
  return "QUERY = TRUNCATE, Table = \"" + targetTable + "\"";
}