//
// Created by liu on 18-10-25.
//

#include "SelectQuery.h"

#include "../../db/Database.h"
#include "../../multiThread/MultiThread.h"
#include "../../multiThread/ThreadPool.h"

constexpr const char *SelectQuery::qname;
static std::mutex selectMutex;
static std::vector<std::string> strOutput;
void SelectQuery::SelectQueryHelper(Table *tablePtr,
                                    Table::Iterator it,
                                    size_t range) {
  std::vector<std::string> localSel(range);
  for (size_t i = 0; i < range; i++) {
    if (it != tablePtr->end()) {
      if (this->evalCondition(*it)) {
        std::string lineMsg;
        lineMsg += "( " + it->key();
        for (unsigned long &fieldId : this->fieldIds) {
          lineMsg += " " + to_string((*it)[fieldId]);
        }
        lineMsg.append(" )\n");
        localSel.emplace_back(lineMsg);
      }
      it++;
    }
  }
  selectMutex.lock();
  strOutput.insert(strOutput.end(), make_move_iterator(localSel.begin()),
                   make_move_iterator(localSel.end()));
  selectMutex.unlock();
}

QueryResult::Ptr SelectQuery::execute() {
  using std::exception;
  using std::future;
  using std::invalid_argument;
  using std::literals::string_literals::operator""s;
  using std::make_unique;
  using std::string;
  using std::vector;
  if (this->operands.empty())
    return make_unique<ErrorMsgResult>(
        qname, this->targetTable.c_str(),
        "Invalid number of operands (? operands)."_f % operands.size());
  Database &db = Database::getInstance();
  string msg;
  try {
    auto &table = db[this->targetTable];
    if (this->operands[0] != "KEY") {
      return make_unique<ErrorMsgResult>(qname, this->targetTable,
                                         "No KEY at first."s);
    }
    for (auto it = this->operands.begin() + 1;
         it != this->operands.end();++it) {
      this->fieldIds.emplace_back(table.getFieldIndex(*it));
    }
    auto result = initCondition(table);
    size_t i;
    //vector<string> strOutput;
    if (result.second) {
      auto size = (size_t) getThreadNum();
      if (size == 1) {
        for (auto it = table.begin(); it != table.end(); ++it) {
          if (this->evalCondition(*it)) {
            string lineMsg;
            lineMsg += "( " + it->key();
            for (unsigned long &fieldId : this->fieldIds) {
              lineMsg += " " + to_string((*it)[fieldId]);
            }
            lineMsg.append(" )\n");
            strOutput.emplace_back(lineMsg);
          }
        }
      } else {
        ThreadPool &pool = ThreadPool::getInstance();
        size_t range = (table.size()) / size + 1;
        vector<future<int>> localSel;
        Table::Iterator it = table.begin();
        //localSel.reserve(size);
        for (i = 0; i < size; ++i) {
          auto sel = pool.enqueue([this, &table, it, range] {
             this->SelectQueryHelper(&table, it, range);
             return 0;
          });
          it += (int) range;
          localSel.emplace_back(move(sel));
        }
        for (auto &&tmp_f : localSel) {
          tmp_f.get();
        }
      }
      sort(strOutput.begin(), strOutput.end());
      for (auto &lineMsg : strOutput) {
        msg += lineMsg;
      }
      if (!msg.empty()) {
        msg.pop_back();
      }
    }
    strOutput.clear();
    if (msg.empty())
      return make_unique<NullQueryResult>();
    else
      return make_unique<SuccessMsgResult>(msg);
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

std::string SelectQuery::toString() {
  return "QUERY = SELECT " + this->targetTable + "\"";
}
