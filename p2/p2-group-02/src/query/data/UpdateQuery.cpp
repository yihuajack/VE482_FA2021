
//
// Created by liu on 18-10-25.
//

#include <iostream>
#include "UpdateQuery.h"
#include "../../multiThread/MultiThread.h"
#include "../../multiThread/ThreadPool.h"
#include "../../multiThread/Reducer.h"
#include "../../db/Database.h"

constexpr const char *UpdateQuery::qname;
//static UpdateQuery* updateQuery;
//static int thread_n;

//static Table::ValueType fieldValueTmp;// = (operands[0]=="KEY")? 0 :std::stoi(operands[1]);
//static Table::FieldIndex fieldIdTmp;
//static Table::KeyType keyValueTmp;
int UpdateQuery::UpdateQueryHelper(Table* tableptr,Table::Iterator it, size_t range){
    int counter = 0;
    for ( size_t i=0; i<range;i++ ) {
        if (it == tableptr->end()){
            break;
        }
        if (this->evalCondition(*it)) {
            if (keyValue.empty()) {
                (*it)[fieldId] = fieldValue;
            } else {
                it->setKey(keyValue);
            }
            ++counter;
        }
        it++;
    }
    return counter;
}

QueryResult::Ptr UpdateQuery::execute() {
  using std::exception;
  using std::future;
  using std::invalid_argument;
  using std::literals::string_literals::operator""s;
  using std::make_unique;
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
      this->keyValue = this->operands[1];
      // keyValueTmp = keyValue;
    } else {
      this->fieldId = table->getFieldIndex(this->operands[0]);
      // fieldIdTmp = fieldId;
      this->fieldValue =
        (Table::ValueType) strtol(this->operands[1].c_str(), nullptr, 10);
      // fieldValueTmp = fieldValue;
    }

    auto result = initCondition(*table);
    if (result.second) {
      auto size = (size_t) getThreadNum();
      if (size == 1) {
        // Single thread
        for (auto it = table->begin(); it != table->end(); ++it) {
          if (this->evalCondition(*it)) {
            if (this->keyValue.empty()) {
              (*it)[this->fieldId] = this->fieldValue;
            } else {
              it->setKey(this->keyValue);
            }
            ++counter;
          }
        }
      } else {
        // Multi-thread
        ThreadPool &pool = ThreadPool::getInstance();
        // -1 for the main thread?
        size_t range = (table->size()) / size + 1;
        Table::Iterator it = table->begin();
        vector<future<int>> results;
        results.reserve(size);
        for (size_t i = 0; i < size; i++) {
          auto sum = pool.enqueue([this, table, it, range]{
            return this->UpdateQueryHelper(table, it, range);
          });
          it += (int)(range); // *(i+1);
          results.emplace_back(move(sum));
        }
        for (auto && tmp:results){
          counter += static_cast<unsigned long>(tmp.get());
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

std::string UpdateQuery::toString() {
  return "QUERY = UPDATE " + this->targetTable + "\"";
}
