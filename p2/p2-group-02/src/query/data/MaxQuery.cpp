//
// Created by Yihua on 2021/11/5.
//

#include "MaxQuery.h"
#include "../../db/Database.h"
#include "../../multiThread/MultiThread.h"
#include "../../multiThread/ThreadPool.h"

constexpr const char *MaxQuery::qname;

std::vector<Table::ValueType> MaxQuery::MaxQueryHelper(Table *table,
               Table::Iterator it, size_t range, const size_t size_operands) {
  std::vector<Table::ValueType> localMax(size_operands, Table::ValueTypeMin);
  for (size_t i = 0; i < range; ++i) {
    if (it != table->end()) {
      if (this->evalCondition(*it)) {
        affected = true;
        for (size_t j = 0; j < size_operands; j++) {
          Table::ValueType maxValue =
              (*it)[table->getFieldIndex(this->operands[j])];
          if (maxValue > localMax[j])
            localMax[j] = maxValue;
        }
      }
      it++;
    }
  }
  return localMax;
}

QueryResult::Ptr MaxQuery::execute() {
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
      maxFields.push_back(Table::ValueTypeMin);
    }
    if (result.second) {
      auto size = (size_t) getThreadNum();
      if (size == 1) {
        for (auto it = table->begin(); it != table->end(); it++) {
          if (this->evalCondition(*it)) {
            affected = true;
            for (i = 0; i < size_operands; i++) {
              Table::ValueType maxValue =
                  (*it)[table->getFieldIndex(this->operands[i])];
              if (maxValue > maxFields[i])
                maxFields[i] = maxValue;
            }
          }
        }
      } else {
        ThreadPool &pool = ThreadPool::getInstance();
        size_t range = (table->size()) / size + 1;
        Table::Iterator it = table->begin();
        vector<future<vector<Table::ValueType>>> localMax;
        localMax.reserve(size);
        for (i = 0; i < size; i++) {
          auto max = pool.enqueue(
              [this, table, it, range, size_operands]{
                return this->MaxQueryHelper(table, it, range, size_operands);
          });
          it += (int) range;
          localMax.emplace_back(move(max));
        }
        for (auto &&tmp_f : localMax) {
          vector<Table::ValueType> tmp = tmp_f.get();
          for (i = 0; i < size_operands; i++) {
            if (tmp[i] > maxFields[i])
              maxFields[i] = tmp[i];
          }
        }
      }
    }
    if (affected)
      return make_unique<SuccessMsgResult>(maxFields);
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

std::string MaxQuery::toString() {
  std::string output = "Answer = (";
  for (const auto &val : maxFields) {
    output += " " + to_string(val);
  }
  output += " )";
  return output;
}
