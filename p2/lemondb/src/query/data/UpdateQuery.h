//
// Created by liu on 18-10-25.
//

#ifndef PROJECT_UPDATEQUERY_H
#define PROJECT_UPDATEQUERY_H

#include "../Query.h"

class UpdateQuery : public ComplexQuery {
  static constexpr const char *qname = "UPDATE";
  Table::ValueType
      fieldValue; // = (operands[0]=="KEY")? 0 :std::stoi(operands[1]);
  Table::FieldIndex fieldId;
  Table::KeyType keyValue;


public:
    using ComplexQuery::ComplexQuery;
    int UpdateQueryHelper(Table* tableptr, Table::Iterator begin, size_t range);


  QueryResult::Ptr execute() override;

  std::string toString() override;

};

#endif // PROJECT_UPDATEQUERY_H
