#include "CountQuery.h"

#include "../../db/Database.h"
#include "../../multiThread/MultiThread.h"
#include "../../multiThread/ThreadPool.h"

constexpr const char *CountQuery::qname;

unsigned long CountQuery::CountQueryHelper(Table *tablePtr, Table::Iterator it,
                                 size_t range) {
  unsigned long counter = 0;
  for (size_t i = 0; i < range; ++i) {
    if (it != tablePtr->end()) {
      if (this->evalCondition(*it)) {
        ++counter;
      }
      it++;
    }
  }
  return counter;
}

QueryResult::Ptr CountQuery::execute() {
  using std::exception;
  using std::literals::string_literals::operator""s;
  using std::make_unique;
  using std::future;
  using std::vector;
  if (!this->operands.empty())
    return make_unique<ErrorMsgResult>(qname, this->targetTable.c_str(),
		    		       "Unexpected operand (? operands)."_f %
				           operands.size());
  Database &db = Database::getInstance();
  Table::SizeType counter = 0;
  try {
    auto &table = db[this->targetTable];
    Table *tablePtr = &table;
    auto result = initCondition(table);
    if (result.second) {
      auto size = (size_t) getThreadNum();
      if (size == 1) {
        // Single thread
        for (auto it : table) {
          if (this->evalCondition(it)) {
            ++counter;
	  }
        }
      }
      else {
        // Multi-thread
        ThreadPool &pool = ThreadPool::getInstance();
	size_t range = (table.size()) / size + 1;
	Table::Iterator it = table.begin();
	vector<future<unsigned long>> results;
	results.reserve(size);
	for (size_t i = 0; i < size; ++i) {
	  auto sum = pool.enqueue([this, tablePtr, it, range] {
            return this->CountQueryHelper(tablePtr, it, range);
	  });
	  it += (int) range;
	  results.emplace_back(std::move(sum));
	}
	for (auto &&tmpRes:results) {
        counter += tmpRes.get();
    }
      }
    }
    return make_unique<SuccessMsgResult>(counter);
  } catch (const TableNameNotFound &e) {
    return make_unique<ErrorMsgResult>(qname, this->targetTable,
		                       "No such table."s);
  } catch (const IllFormedQueryCondition &e) {
    return make_unique<ErrorMsgResult>(qname, this->targetTable, e.what());
  } catch (const exception &e) {
    return make_unique<ErrorMsgResult>(qname, this->targetTable,
		                       "Unknown error '?'."_f % e.what());
  }
}

std::string CountQuery::toString() {
  return "QUERY = COUNT " + this->targetTable + "\"";
}
