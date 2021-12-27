//
// Created by alaallen on 11/19/21.
//

#include "io.h"

namespace IO{
    static std::unique_ptr<std::vector<std::istream *>> isPool = nullptr;

    std::vector<std::istream *> &getISPool() {
        if (isPool == nullptr) {
            isPool = std::unique_ptr<std::vector<std::istream *>>(new std::vector<std::istream *>);
        }
        return *isPool;
    }
} // namespace IO