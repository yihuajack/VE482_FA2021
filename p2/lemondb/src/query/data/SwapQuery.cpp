//
// Created by alaallen on 11/2/21.
//
#include <iostream>
#include "SwapQuery.h"
#include "../../multiThread/MultiThread.h"
#include "../../multiThread/ThreadPool.h"
#include "../../db/Database.h"
#include "../../multiThread/LocalReducer.h"

constexpr const char *SwapQuery::qname;

int SwapQuery::SwapQueryHelper(Table* tableptr,Table::Iterator it, size_t range){
  int counter = 0;
  for ( size_t i=0; i<range;i++ ) {
    if (it == tableptr->end()){
      break;
    }
    if (this->evalCondition(*it)) {
      std::swap((*it)[this->fieldIdL],(*it)[this->fieldIdR]);
      ++counter;
    }
    it++;
  }
  return counter;
}

QueryResult::Ptr SwapQuery::execute() {
  // SWAP (field1 field2) FROM table WHERE ( cond )
  using std::exception;
  using std::future;
  using std::invalid_argument;
  using std::literals::string_literals::operator""s;
  using std::make_unique;
  using std::swap;
  using std::vector;
  if (this->operands.size() != 2)
    return make_unique<ErrorMsgResult>(
            qname, this->targetTable.c_str(),
            "Invalid number of operands (? operands)."_f % operands.size());
  Database &db = Database::getInstance();
  Table::SizeType counter = 0;
  try {
      auto &tablePtr = db[this->targetTable];
      auto table = &tablePtr;
    if (this->operands[0] == "KEY") {
      return make_unique<ErrorMsgResult>(
              qname, this->targetTable.c_str(),
              "KEY Shouldn't appear in SWAP query");
    } else {
      this->fieldIdL = table->getFieldIndex(this->operands[0]);
      this->fieldIdR = table->getFieldIndex(this->operands[1]);
    }
    auto result = initCondition(*table);
    if (result.second) { // if the condition is not always false
      auto size = (size_t) getThreadNum();
      if (size == 1 || table->size()<10) {
        // Single thread
        for (auto it : *table) {
          if (this->evalCondition(it)) {
            swap(it[this->fieldIdL],it[this->fieldIdR]);
            ++counter;
          }
        }
      } else{
        // Multi-thread
        ThreadPool &pool = ThreadPool::getInstance();
        size_t range = (table->size())/size+1;
        Table::Iterator it = table->begin();
        vector<future<int>> results;
        results.reserve(size);
        for (size_t i = 0; i < size; i++){
          auto sum = pool.enqueue([this, table, it, range](){
            return this->SwapQueryHelper(table, it, range);
          });
          it += (int)(range);
          results.emplace_back(move(sum));
        }
        for (auto && tmp : results){
          counter += static_cast<unsigned long>(tmp.get());;
        }
      }
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

std::string SwapQuery::toString() {
  return "QUERY = SWAP" + this->targetTable + "\"";
}
