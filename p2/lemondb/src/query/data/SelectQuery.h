//
// Created by liu on 18-10-25.
//

#ifndef PROJECT_SELECTQUERY_H
#define PROJECT_SELECTQUERY_H

#include "../Query.h"

class SelectQuery : public ComplexQuery {
  static constexpr const char *qname = "SELECT";
  std::vector<Table::FieldIndex> fieldIds;

public:
  using ComplexQuery::ComplexQuery;

  void SelectQueryHelper(Table *tablePtr,
                                             Table::Iterator it,
                                             size_t range);

  QueryResult::Ptr execute() override;

  std::string toString() override;
};

#endif // PROJECT_SELECTQUERY_H
