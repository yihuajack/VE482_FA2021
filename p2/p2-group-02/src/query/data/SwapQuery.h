//
// Created by alaallen on 11/2/21.
//

#ifndef LEMONDB_SWAPQUERY_H
#define LEMONDB_SWAPQUERY_H

#include "../Query.h"

class SwapQuery: public ComplexQuery {
    static constexpr const char *qname = "SWAP";
    Table::FieldIndex fieldIdL;
    Table::FieldIndex fieldIdR;

public:
    using ComplexQuery::ComplexQuery;
    int SwapQueryHelper(Table* tableptr,Table::Iterator begin, size_t range);
    QueryResult::Ptr execute() override;

    std::string toString() override;
};


#endif //LEMONDB_SWAPQUERY_H
