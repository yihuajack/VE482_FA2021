//
// Created by Yihua on 2021/11/5.
//

#include "MinQuery.h"
#include "../../db/Database.h"
#include "../../multiThread/MultiThread.h"
#include "../../multiThread/ThreadPool.h"

constexpr const char *MinQuery::qname;

std::vector<Table::ValueType> MinQuery::MinQueryHelper(Table *table,
               Table::Iterator it, size_t range, const size_t size_operands) {
  std::vector<Table::ValueType> localMin(size_operands, Table::ValueTypeMax);
  for (size_t i = 0; i < range; ++i) {
    if (it != table->end()) {
      if (this->evalCondition(*it)) {
        affected = true;
        for (size_t j = 0; j < size_operands; j++) {
          Table::ValueType minValue =
              (*it)[table->getFieldIndex(this->operands[j])];
          if (minValue < localMin[j])
            localMin[j] = minValue;
        }
      }
      it++;
    }
  }
  return localMin;
}

QueryResult::Ptr MinQuery::execute() {
  using std::exception;
  using std::future;
  using std::literals::string_literals::operator""s;
  using std::make_unique;
  using std::vector;
  if (this->operands.empty())
    return make_unique<ErrorMsgResult>(qname, this->targetTable.c_str(),
                                       "Unexpected operand (? operands)."_f %
                                           operands.size());
  Database &db = Database::getInstance();
  try {
    auto &tablePtr = db[this->targetTable];
    auto table = &tablePtr;
    auto result = initCondition(*table);
    size_t i, size_operands = this->operands.size();
    for (i = 0; i < this->operands.size(); i++) {
      minFields.push_back(Table::ValueTypeMax);
    }
    if (result.second) {
      auto size = (size_t) getThreadNum();
      if (size == 1) {
        for (auto it = table->begin(); it != table->end(); it++) {
          if (this->evalCondition(*it)) {
            affected = true;
            for (i = 0; i < size_operands; i++) {
              Table::ValueType minValue =
                  (*it)[table->getFieldIndex(this->operands[i])];
              if (minValue < minFields[i])
                minFields[i] = minValue;
            }
          }
        }
      } else {
        ThreadPool &pool = ThreadPool::getInstance();
        size_t range = (table->size()) / size + 1;
        Table::Iterator it = table->begin();
        vector<future<vector<Table::ValueType>>> localMin;
        localMin.reserve(size);
        for (i = 0; i < size; ++i) {
          auto min = pool.enqueue(
              // Effective Modern C++ 6. Lambda Expressions
              // Item 31: Avoid default capture modes.
              [this, table, it, range, size_operands]{
                return this->MinQueryHelper(table, it, range, size_operands);
              });
          it += (int) range;
          localMin.emplace_back(std::move(min));
        }
        for (auto &&tmp_f : localMin) {
          vector<Table::ValueType> tmp = tmp_f.get();
          for (i = 0; i < size_operands; i++) {
            if (tmp[i] < minFields[i])
              minFields[i] = tmp[i];
          }
        }
      }
    }
    if (affected)
      return make_unique<SuccessMsgResult>(minFields);
    else
      return make_unique<NullQueryResult>();
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

std::string MinQuery::toString() {
  std::string output = "Answer = (";
  for (const auto &val : minFields) {
    output += " " + to_string(val);
  }
  output += " )";
  return output;
}
