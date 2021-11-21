//
// Created by liu on 18-10-25.
//

#include "DeleteQuery.h"

#include "../../db/Database.h"
#include "../../multiThread/ThreadPool.h"

constexpr const char *DeleteQuery::qname;

unsigned long DeleteQuery::DeleteQueryHelper(Table *tablePtr, Table::Iterator it,
		size_t range) {
  unsigned long counter = 0;
  for (size_t i = 0; i < range; ++i) {
    if (it == tablePtr->end()) {
      break;
    }
    if (this->evalCondition(*it)) {
      tablePtr->beforeDelete(it);
      ++counter;
    }
    it++;
  }
  return counter;
}

QueryResult::Ptr DeleteQuery::execute() {
  using std::exception;
  using std::invalid_argument;
  using std::literals::string_literals::operator""s;
  using std::make_unique;
  using std::future;
  using std::vector;
  if (!this->operands.empty())
    return make_unique<ErrorMsgResult>(
        qname, this->targetTable.c_str(),
        "Invalid number of operands (? operands)."_f % operands.size());
  Database &db = Database::getInstance();
  Table::SizeType counter = 0;
  try {
    auto &table = db[this->targetTable];
    Table *tablePtr = &table;
    auto result = initCondition(table);
    if (result.second) {
      auto size = (size_t) getThreadNum();
      if (size == 1) {
	// Single Thread
        for (auto it = table.begin(); it != table.end(); ++it) {
          if (this->evalCondition(*it)) {
            table.beforeDelete(it);
            ++counter;
          }
        }
      }
      else {
        // MultiThread
	ThreadPool &pool = ThreadPool::getInstance();
	size_t range = (table.size()) / size + 1;
	Table::Iterator it = table.begin();
	vector<future<unsigned long>> results;
	results.reserve(size);
	for (size_t i = 0; i < size; ++i) {
          auto sum = pool.enqueue([this, tablePtr, it, range] {
	    return this->DeleteQueryHelper(tablePtr, it, range);
	  });
	  it += (int) range;
	  results.emplace_back(std::move(sum));
	}
	for (auto &&temp : results) {
          counter += temp.get();
	}
      }
      table.doDelete();
    }
    return make_unique<RecordCountResult>(counter);
  } catch (const TableNameNotFound &e) {
    return make_unique<ErrorMsgResult>(qname, this->targetTable,
                                       "No such table."s);
  } catch (const IllFormedQueryCondition &e) {
    return make_unique<ErrorMsgResult>(qname, this->targetTable, e.what());
  } catch (const invalid_argument &e) {
    // Cannot convert operand to string
    return make_unique<ErrorMsgResult>(qname, this->targetTable,
                                       "Unknown error '?'"_f % e.what());
  } catch (const exception &e) {
    return make_unique<ErrorMsgResult>(qname, this->targetTable,
                                       "Unkonwn error '?'."_f % e.what());
  }
}

std::string DeleteQuery::toString() {
  return "QUERY = DELETE " + this->targetTable + "\"";
}
