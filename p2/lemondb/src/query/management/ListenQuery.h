//
// Created by alaallen on 11/19/21.
//

#ifndef LEMONDB_LISTENQUERY_H
#define LEMONDB_LISTENQUERY_H



#include "../Query.h"

class ListenQuery : public Query {
    static constexpr const char *qname = "LISTEN";
    const std::string fileName;
public:
    explicit ListenQuery(std::string _filename)
        : fileName(std::move(_filename)) {}
    QueryResult::Ptr execute() override;

    std::string toString() override;
};

#endif //LEMONDB_LISTENQUERY_H
