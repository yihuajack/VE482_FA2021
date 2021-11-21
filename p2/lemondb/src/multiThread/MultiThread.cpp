//
// Created by alaallen on 11/9/21.
//

#include "MultiThread.h"
static long threadNum;
void autoDectThreadNUm(){
    threadNum=std::thread::hardware_concurrency();
}

void setThreadNum(long i){
    threadNum = i;
}

long getThreadNum(){
    return threadNum;
}