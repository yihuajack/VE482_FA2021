project(lemondb)
cmake_minimum_required(VERSION 2.7)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_FLAGS
    "-g -Wall -Wextra -Werror -pedantic -Wno-unused-result -Wconversion -Wno-unused-parameter -O2"
)
set(CMAKE_BUILD_TYPE Release)
set(SOURCE_FILES
    db/Database.h
    db/Database.cpp
    db/Table.h
    db/Table.cpp
    multiThread/MultiThread.h
    multiThread/MultiThread.cpp
    multiThread/Reducer.h
    multiThread/Reducer.cpp
    multiThread/ThreadPool.h
        multiThread/LocalReducer.h
    multiThread/LocalReducer.cpp
    query/Query.h
    query/Query.cpp
    query/QueryParser.h
    query/QueryParser.cpp
    query/QueryResult.h
    query/QueryResult.cpp
    query/QueryBuilders.h
    query/QueryBuilders.cpp
    query/management/CopyTableQuery.h
    query/management/CopyTableQuery.cpp
    query/management/DropTableQuery.h
    query/management/DropTableQuery.cpp
    query/management/DumpTableQuery.h
    query/management/DumpTableQuery.cpp
    query/management/ListenQuery.h
    query/management/ListenQuery.cpp
    query/management/ListTableQuery.h
    query/management/ListTableQuery.cpp
    query/management/LoadTableQuery.h
    query/management/LoadTableQuery.cpp
    query/management/PrintTableQuery.h
    query/management/PrintTableQuery.cpp
    query/management/QuitQuery.h
    query/management/QuitQuery.cpp
    query/management/TruncateTableQuery.h
    query/management/TruncateTableQuery.cpp
    query/data/AddQuery.h
    query/data/AddQuery.cpp
    query/data/CountQuery.h
    query/data/CountQuery.cpp
    query/data/DeleteQuery.h
    query/data/DeleteQuery.cpp
    query/data/DuplicateQuery.h
    query/data/DuplicateQuery.cpp
    query/data/InsertQuery.h
    query/data/InsertQuery.cpp
    query/data/MinQuery.h
    query/data/MinQuery.cpp
    query/data/MaxQuery.h
    query/data/MaxQuery.cpp
    query/data/SelectQuery.h
    query/data/SelectQuery.cpp
    query/data/SubQuery.h
    query/data/SubQuery.cpp
    query/data/SumQuery.h
    query/data/SumQuery.cpp
    query/data/SwapQuery.h
    query/data/SwapQuery.cpp
    query/data/UpdateQuery.h
    query/data/UpdateQuery.cpp
    utils/formatter.h
    utils/io.cpp
    utils/io.h
    utils/uexception.h
)
add_executable(lemondb main.cpp ${SOURCE_FILES})
target_link_libraries(lemondb pthread)
