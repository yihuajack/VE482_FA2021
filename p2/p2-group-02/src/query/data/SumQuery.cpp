//
// Created by Yihua on 2021/11/4.
//

#include "SumQuery.h"
#include "../../db/Database.h"
#include "../../multiThread/MultiThread.h"
#include "../../multiThread/ThreadPool.h"

constexpr const char *SumQuery::qname;

std::vector<Table::ValueType> SumQuery::SumQueryHelper(Table *table,
               Table::Iterator it, size_t range, const size_t size_operands) {
  std::vector<Table::ValueType> results(size_operands);
  for (size_t i = 0; i < range; ++i) {
    if (it != table->end()) {
      if (this->evalCondition(*it)) {
        for (size_t j = 0; j < size_operands; j++) {
          results[j] += (*it)[table->getFieldIndex(this->operands[j])];
        }
      }
      it++;
    }
  }
  return results;
}


QueryResult::Ptr SumQuery::execute() {
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
    // TODO is that necessary
    for (i = 0; i < size_operands; i++) {
      sumFields.push_back(0);
    }
    if (result.second) {
        auto size = (size_t) getThreadNum();
        if (size == 1) {
            // this initialization is only needed when it is single-threaded
            for (auto it = table->begin(); it != table->end(); it++) {
                if (this->evalCondition(*it)) {
                    for (i = 0; i < size_operands; i++) {
                        sumFields[i] +=
                          (*it)[table->getFieldIndex(this->operands[i])];
                    }
                }
            }
        } else{
            ThreadPool &pool = ThreadPool::getInstance();
            size_t range = (table->size()) / size + 1;
            Table::Iterator it = table->begin();
            vector<future<vector<Table::ValueType>>> results;
            results.reserve(size);
            for (i = 0; i < size; ++i) {
                auto sum = pool.enqueue(
                  [this, table, it, range, size_operands]{
                    return this->SumQueryHelper(table, it, range, size_operands);
                });
                it += (int) range;
                results.emplace_back(std::move(sum));  // sum;
                // results.emplace_back(std::move(sum));
                // counter += static_cast<unsigned long>(sum.get());
            }
            for (auto &&tmp_f : results) {
                vector<Table::ValueType> tmp = tmp_f.get();
                // TODO check why std::move is not allowed here
                for (i = 0; i < size_operands; i++) {
                    sumFields[i] += tmp[i];
                }
            }
        }
    }
    return make_unique<SuccessMsgResult>(sumFields);
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

std::string SumQuery::toString() {
  std::string output = "Answer = (";
  for (const auto &val : sumFields) {
    output += " " + to_string(val);
  }
  output += " )";
  return output;
}
