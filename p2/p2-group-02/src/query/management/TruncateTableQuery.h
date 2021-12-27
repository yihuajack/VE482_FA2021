//
// Created by alaallen on 10/26/21.
//

#ifndef LEMONDB_TRUNCATETABLEQUERY_H
#define LEMONDB_TRUNCATETABLEQUERY_H


#include "../Query.h"

class TruncateTableQuery : public Query {
    static constexpr const char *qname = "TRUNCATE";

public:
    explicit TruncateTableQuery(std::string table)
            : Query(std::move(table)) {}

    QueryResult::Ptr execute() override;

    std::string toString() override;
};




#endif //LEMONDB_TRUNCATETABLEQUERY_H
