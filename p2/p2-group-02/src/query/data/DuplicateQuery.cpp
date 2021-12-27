#include "DuplicateQuery.h"

#include "../../db/Database.h"

constexpr const char *DuplicateQuery::qname;

QueryResult::Ptr DuplicateQuery::execute() {
  using std::exception;
  using std::invalid_argument;
  using std::literals::string_literals::operator""s;
  using std::make_unique;
  using std::vector;
  if (!this->operands.empty())
    return make_unique<ErrorMsgResult>(qname, this->targetTable.c_str(),
        "Invalid number of operands (? operands)."_f % operands.size());
  Database &db = Database::getInstance();
  long count = 0;
  vector<long> indices;
  size_t affected = 0;
  try {
    auto &table = db[this->targetTable];
    auto result = initCondition(table);
    if (result.second) {
      for (auto it : table) {
        if (this->evalCondition(it)) {
          indices.emplace_back(count);
        }
        count++;
      }
      for (auto index : indices) {
        table.dupByIndex(index, affected);
      }
    }
    return make_unique<RecordCountResult>(affected);
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

std::string DuplicateQuery::toString() {
  return "QUERY = DUPLICATE " + this->targetTable + "\"";
}
