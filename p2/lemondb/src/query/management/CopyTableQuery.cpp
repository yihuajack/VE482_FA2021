//
// Created by alaallen on 10/26/21.
//

#include "CopyTableQuery.h"


#include "../../db/Database.h"

constexpr const char *CopyTableQuery::qname;

QueryResult::Ptr CopyTableQuery::execute() {
  using std::exception;
  using std::make_unique;
  using std::move;
  Database &db = Database::getInstance();
  try {
    auto table = make_unique<Table>(this->newTableName,db[this->targetTable]);
    db.registerTable(move(table));
    return make_unique<NullQueryResult>();
  } catch (const exception &e) {
    return make_unique<ErrorMsgResult>(qname, e.what());
  }
}

std::string CopyTableQuery::toString() {
    return "QUERY = Copy TABLE, TABLE = \"" + this->newTableName + "\"";
}
