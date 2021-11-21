//
// Created by alaallen on 11/19/21.
//

#ifndef LEMONDB_IO_H
#define LEMONDB_IO_H


#include <memory>
#include <vector>


namespace IO {
    std::vector<std::istream *> &getISPool();

}
#endif //LEMONDB_IO_H
