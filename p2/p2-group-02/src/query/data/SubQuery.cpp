#include "SubQuery.h"

#include "../../db/Database.h"
#include "../../multiThread/MultiThread.h"
#include "../../multiThread/ThreadPool.h"

constexpr const char *SubQuery::qname;

unsigned long SubQuery::SubQueryHelper(Table *tablePtr, Table::Iterator it, size_t range) {
  unsigned long counter = 0;
  for (size_t i = 0; i < range; ++i) {
    if (it != tablePtr->end()) {
      if (this->evalCondition(*it)) {
        Table::ValueType fieldValue = (*it)[this->srcFieldId];
        for (auto field = (operands.begin() + 1);
             field != (operands.end() - 1); ++field) {
          fieldValue -= (*it)[tablePtr->getFieldIndex(*field)];
        }
        (*it)[this->desFieldId] = fieldValue;
        ++counter;
      }
      it++;
    }
  }
  return counter;
}

QueryResult::Ptr SubQuery::execute() {
  using std::exception;
  using std::future;
  using std::literals::string_literals::operator""s;
  using std::make_unique;
  using std::vector;
  if (this->operands.size() < 2)
    return make_unique<ErrorMsgResult>(
        qname, this->targetTable.c_str(),
	"Invalid number of operands (? operands)."_f % operands.size());
  Database &db = Database::getInstance();
  Table::SizeType counter = 0;
  try {
      auto &tablePtr = db[this->targetTable];
      auto table = &tablePtr;
    if (operands.front() == "KEY" || operands.back() == "KEY") {
      return make_unique<ErrorMsgResult>(qname, this->targetTable,
		                         R"(Bad argument "KEY".)");
    }
    this->srcFieldId = table->getFieldIndex(this->operands.front());
    this->desFieldId = table->getFieldIndex(this->operands.back());
    auto result = initCondition(*table);
    if (result.second) {
      auto size = (size_t) getThreadNum();
      if (size == 1) {
	// Single thread
        for (auto it = table->begin(); it != table->end(); ++it) {
          if (this->evalCondition(*it)) {
            Table::ValueType fieldValue = (*it)[this->srcFieldId];
            for (auto field = (operands.begin() + 1);
                 field != (operands.end() - 1); ++field) {
              if (*field == "KEY") {
                return make_unique<ErrorMsgResult>(qname, this->targetTable, 
	  		                           R"(Bad argument "KEY".)");
	      }
	      fieldValue -= (*it)[table->getFieldIndex(*field)];
	    }
	    (*it)[this->desFieldId] = fieldValue;
	    ++counter;
	  }
        }
      }
      else {
        // Multi-thread
        ThreadPool &pool = ThreadPool::getInstance();
        vector<future<unsigned long>> results;
        results.reserve(size);
        size_t range = (table->size()) / size + 1;
        Table::Iterator it = table->begin();
        for (size_t i = 0; i < size; ++i) {
          auto sum = pool.enqueue([this, table, it, range]{
            return this->SubQueryHelper(table, it, range);
          });
          it += (int) range;
          results.emplace_back(move(sum));
        }
        for (auto &&tmp : results) {
          counter += tmp.get();
        }
      }
    }
    return make_unique<RecordCountResult>(counter);
  } catch (const TableNameNotFound &e) {
    return make_unique<ErrorMsgResult>(qname, this->targetTable, 
		                       "No such table."s);
  } catch (const IllFormedQueryCondition &e) {
    return make_unique<ErrorMsgResult>(qname, this->targetTable, e.what());
  } catch (const exception &e) {
    return make_unique<ErrorMsgResult>(qname, this->targetTable,
		                       "Unkonwn error '?'."_f % e.what());
  }
}

std::string SubQuery::toString() {
  return "QUERY = SUB " + this->targetTable + "\"";
}
