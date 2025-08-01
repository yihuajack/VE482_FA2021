//
// Created by liu on 18-10-25.
//

#ifndef PROJECT_DELETEQUERY_H
#define PROJECT_DELETEQUERY_H

#include "../Query.h"

class DeleteQuery : public ComplexQuery {
  static constexpr const char *qname = "DELETE";

public:
  using ComplexQuery::ComplexQuery;

  unsigned long DeleteQueryHelper(Table *tablePtr, Table::Iterator it,
		  size_t range);

  QueryResult::Ptr execute() override;

  std::string toString() override;
};

#endif // PROJECT_DELETEQUERY_H
