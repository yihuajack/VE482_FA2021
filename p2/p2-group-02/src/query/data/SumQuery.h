//
// Created by Yihua on 2021/11/4.
//

#ifndef PROJECT_SUMQUERY_H
#define PROJECT_SUMQUERY_H

#include "../Query.h"

class SumQuery : public ComplexQuery {
  static constexpr const char *qname = "SUM";
  std::vector<Table::ValueType> sumFields;

public:
  using ComplexQuery::ComplexQuery;

  QueryResult::Ptr execute() override;

  std::string toString() override;

  std::vector<Table::ValueType> SumQueryHelper(Table *table,
                       Table::Iterator it, size_t range, size_t size_operands);
};

#endif // PROJECT_SUMQUERY_H
