//
// Created by Yihua on 2021/11/5.
//

#ifndef PROJECT_MAXQUERY_H
#define PROJECT_MAXQUERY_H

#include "../Query.h"

class MaxQuery : public ComplexQuery {
  static constexpr const char *qname = "MAX";
  std::vector<Table::ValueType> maxFields;
  bool affected = false;

public:
  using ComplexQuery::ComplexQuery;

  QueryResult::Ptr execute() override;

  std::string toString() override;

  std::vector<Table::ValueType> MaxQueryHelper(Table *table, Table::Iterator it,
                                               size_t range,
                                               size_t size_operands);
};

#endif // PROJECT_MAXQUERY_H
