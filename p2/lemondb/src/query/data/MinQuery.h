//
// Created by Yihua on 2021/11/5.
//

#ifndef PROJECT_MINQUERY_H
#define PROJECT_MINQUERY_H

#include "../Query.h"

class MinQuery : public ComplexQuery {
  static constexpr const char *qname = "MIN";
  std::vector<Table::ValueType> minFields;
  bool affected = false;

public:
  using ComplexQuery::ComplexQuery;

  QueryResult::Ptr execute() override;

  std::string toString() override;

  std::vector<Table::ValueType> MinQueryHelper(Table *table, Table::Iterator it,
                                               size_t range,
                                               size_t size_operands);
};

#endif // PROJECT_MINQUERY_H
