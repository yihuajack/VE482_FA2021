//
// Created by liu on 18-10-21.
//

#include <getopt.h>

#include <fstream>
#include <iostream>
#include <string>
#include <chrono>
#include "multiThread/MultiThread.h"
#include "query/QueryBuilders.h"
#include "query/QueryParser.h"

#include "utils/io.h"
struct {
    std::string listen;
    long threads = 0;
} parsedArgs;
//only used to save result of input arguments

void parseArgs(int argc, char *argv[]) {
  const option longOpts[] = {{"listen", required_argument, nullptr, 'l'},
                             {"threads", required_argument, nullptr, 't'},
                             {nullptr, no_argument, nullptr, 0}};
  const char *shortOpts = "l:t:";
  int opt, longIndex;
  while ((opt = getopt_long(argc, argv, shortOpts, longOpts, &longIndex)) !=
         -1) {
    if (opt == 'l') {
      parsedArgs.listen = optarg;
    } else if (opt == 't') {
      parsedArgs.threads = std::strtol(optarg, nullptr, 10);
    } else {
      std::cerr << "lemondb: warning: unknown argument "
                << longOpts[longIndex].name << std::endl;
    }
  }
}

std::string extractQueryString(std::istream &is) {
  std::string buf;
  do {
    int ch = is.get();
    if (ch == ';')
      return buf;
    if (ch == EOF)
      throw std::ios_base::failure("End of input");
    buf.push_back((char)ch);
  } while (true);
}

int main(int argc, char *argv[]) {
  // Assume only C++ style I/O is used in lemondb
  // Do not use printf/fprintf in <cstdio> with this line


  std::ios_base::sync_with_stdio(false);

  parseArgs(argc, argv);

  std::ifstream fin;
  if (!parsedArgs.listen.empty()) {
    fin.open(parsedArgs.listen);
    if (!fin.is_open()) {
      std::cerr << "lemondb: error: " << parsedArgs.listen
                << ": no such file or directory" << std::endl;
      exit(-1);
    }
  }
  std::istream is(fin.rdbuf());
  //if listem fin becomes the file stream and is assign to is
  //if not is is initialized as empty

#ifdef NDEBUG
  // In production mode, listen argument must be defined
  if (parsedArgs.listen.empty()) {
    std::cerr << "lemondb: error: --listen argument not found, not allowed in "
                 "production mode"
              << std::endl;
    exit(-1);
  }
#else
  // In debug mode, use stdin as input if no listen file is found
  if (parsedArgs.listen.empty()) {
    std::cerr << "lemondb: warning: --listen argument not found, use stdin "
                 "instead in debug mode"
              << std::endl;
    is.rdbuf(std::cin.rdbuf());
  }
#endif

  if (parsedArgs.threads < 0) {
    std::cerr << "lemondb: error: threads num can not be negative value "
              << parsedArgs.threads << std::endl;
    exit(-1);
  } else if (parsedArgs.threads == 0) {
    // @TODO Auto detect the thread num
    autoDectThreadNUm();

  } else {
    setThreadNum(parsedArgs.threads);

  }
    std::cerr << "lemondb: info: running in " << getThreadNum()
              << " threads" << std::endl;
  QueryParser p;

  p.registerQueryBuilder(std::make_unique<QueryBuilder(Debug)>());
  p.registerQueryBuilder(std::make_unique<QueryBuilder(ManageTable)>());
  p.registerQueryBuilder(std::make_unique<QueryBuilder(Complex)>());

  size_t counter = 0;

  auto &isPool = IO::getISPool();
//  auto fsPool = IO::getFSPool();
  isPool.push_back(&is);
  while (!isPool.empty()) {
    try {
      // A very standard REPL
      // REPL: Read-Evaluate-Print-Loop
      std::string queryStr = extractQueryString(*isPool.back());
      Query::Ptr query = p.parseQuery(queryStr);
      QueryResult::Ptr result = query->execute();
      std::cout << ++counter << "\n";
      if (result->success()) {
        if (result->display()) {
          std::cout << *result;
        } else {
#ifndef NDEBUG
          std::cout.flush();
          std::cerr << *result;
#endif
        }
      } else {
        std::cout.flush();
        std::cerr << "QUERY FAILED:\n\t" << *result;
      }
    } catch (const std::ios_base::failure &e) {
      // End of input
      if (isPool.size()==1)
        break;
      if (isPool.size()>1){
          delete isPool.back();
      }
      isPool.pop_back();
      continue;
    } catch (const std::exception &e) {
      std::cout.flush();
      std::cerr << e.what() << std::endl;
    }
  }
 return 0;
}
