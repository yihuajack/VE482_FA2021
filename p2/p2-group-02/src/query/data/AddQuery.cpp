#include "AddQuery.h"

#include "../../db/Database.h"
#include "../../multiThread/MultiThread.h"
#include "../../multiThread/ThreadPool.h"

constexpr const char *AddQuery::qname;

long unsigned AddQuery::AddQueryHelper(Table *tablePtr, Table::Iterator it, size_t range) {
  long unsigned counter = 0;
  //auto it = begin;
  for (size_t i = 0; i < range; ++i) {
    if (it == tablePtr->end()) {
      break;
    }
    if (this->evalCondition(*it)) {
      Table::ValueType fieldValue = 0;
      for (auto field = operands.begin(); field != (operands.end() - 1); ++field) {
        fieldValue += (*it)[tablePtr->getFieldIndex(*field)];
      }
      (*it)[this->fieldId] = fieldValue;
      ++counter;
    }
    it++;
  }
  return counter;
}

QueryResult::Ptr AddQuery::execute() {
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
    auto &table = db[this->targetTable];
    Table *tablePtr = &table;
    if (operands.back() == "KEY") {
      return make_unique<ErrorMsgResult>(qname, this->targetTable, 
		                         "Bad argument \"KEY\"."s);
    }
    this->fieldId = table.getFieldIndex(this->operands.back());
    auto result = initCondition(table);
    if (result.second) {
      auto size = (size_t) getThreadNum();
      if (size == 1 ) {
        // Single thread
        for (auto it = table.begin(); it != table.end(); ++it) {
          if (this->evalCondition(*it)) {
	    Table::ValueType fieldValue = 0;
	    for (auto field = operands.begin();
                 field != (operands.end() - 1); ++field) {
              if (*field == "KEY") {
                return make_unique<ErrorMsgResult>(qname, this->targetTable,
		  	                           "Bad argument \"KEY\"."s);
	      }
              fieldValue += (*it)[table.getFieldIndex(*field)];
	    }
	    (*it)[this->fieldId] = fieldValue;
	    ++counter;
	  }
        }
      }
      else {
	// Multi-thread
        ThreadPool &pool = ThreadPool::getInstance();
        size_t range = (table.size()) / size + 1;
        Table::Iterator it = table.begin();
        vector<future<long unsigned>> results;
        results.reserve(size);
        for (size_t i = 0; i < size; ++i) {
          auto sum = pool.enqueue([this, tablePtr, it, range]{
            return this->AddQueryHelper(tablePtr, it, range);
          });
          it += (int) range;
          results.emplace_back(std::move(sum));
          //counter += static_cast<unsigned long>(sum.get());
        }
        for (auto &&tmp:results){
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

std::string AddQuery::toString() {
  return "QUERY = ADD " + this->targetTable + "\"";
}
