//
// Created by alaallen on 11/10/21.
//

#ifndef LEMONDB_REDUCER_H
#define LEMONDB_REDUCER_H

template<typename T>
class AddReducer {
public:
    inline T operator ()(const T a, const T b){
        return a+b;
    }
};


#endif //LEMONDB_REDUCER_H
