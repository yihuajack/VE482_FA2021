#ifndef PROJECT_ADDQUERY_H
#define PROJECT_ADDQUERY_H

#include "../Query.h"

class AddQuery : public ComplexQuery {
  static constexpr const char *qname = "ADD";
  Table::FieldIndex fieldId; // Field index of destination field

public:
  using ComplexQuery::ComplexQuery;

  long unsigned AddQueryHelper(Table *tablePtr, Table::Iterator begin, size_t range);

  QueryResult::Ptr execute() override;

  std::string toString() override;
};

#endif // PROJECT_ADDQUERY_H

