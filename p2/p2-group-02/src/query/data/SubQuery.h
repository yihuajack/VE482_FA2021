#ifndef PROJECT_SUBQUERY_H
#define PROJECT_SUBQUERY_H

#include "../Query.h"

class SubQuery : public ComplexQuery {
  static constexpr const char *qname = "SUB";
  Table::FieldIndex srcFieldId;
  Table::FieldIndex desFieldId;

public:
  using ComplexQuery::ComplexQuery;

  unsigned long SubQueryHelper(Table *tablePtr, Table::Iterator begin, size_t range);

  QueryResult::Ptr execute() override;

  std::string toString() override;
};

#endif // PROJECT_SUBQUERY_H

