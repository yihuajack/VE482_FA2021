//
// Created by alaallen on 10/26/21.
//

#ifndef LEMONDB_COPYTABLEQUERY_H
#define LEMONDB_COPYTABLEQUERY_H

#include "../Query.h"

class CopyTableQuery : public Query {
    static constexpr const char *qname = "COPYTABLE";
    const std::string newTableName;

public:
    explicit CopyTableQuery(std::string srcTableName, std::string newTableName)
            : Query(std::move(srcTableName)), newTableName(std::move(newTableName)) {}

    QueryResult::Ptr execute() override;

    std::string toString() override;
};




#endif //LEMONDB_COPYTABLEQUERY_H
