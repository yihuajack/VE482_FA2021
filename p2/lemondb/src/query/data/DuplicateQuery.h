#ifndef PROJECT_DUPLICATEQUERY_H
#define PROJECT_DUPLICATEQUERY_H

#include "../Query.h"

class DuplicateQuery : public ComplexQuery {
  static constexpr const char *qname = "DUPLICATE";
  // vecKey and vecData store keys and data needed to be duplicated
  std::vector<Table::KeyType> vecKey;
  std::vector<std::vector<Table::ValueType>> vecData;

  /**
   * Compare two string to check whether one is the copy of another,
   * @param oldKey value from DuplicateQuery::vecKey
   * @param newKey value of the key of each record visited in loop
   * @return whether we need to add the key/value of current record to
   * vecKey/vecData (e.g. if @param oldKey = "Bill_Gates_copy_copy" and @param
   * newKey = "Bill_Gates", then return value should be false
   */
  bool findKeyCopy(const Table::KeyType& oldKey,
                     const Table::KeyType& newKey,
                     const std::string& copy = "_copy");
  
public:
  using ComplexQuery::ComplexQuery;

  QueryResult::Ptr execute() override;

  std::string toString() override;
};

#endif // PROJECT_DUPLICATEQUERY_H
