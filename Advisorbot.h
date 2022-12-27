#pragma once

#include <string>
#include <vector>
#include "OrderBookEntry.h"
#include "OrderBook.h"
#include "CSVReader.h"

class Advisorbot
{

  // TASK 3 - Iterating over vectors and memory - week 7

public:
  Advisorbot();
  void init();

private:
  // void loadOrderBook();
  void advisorbotCommand();
  std::vector<std::string> getUserCommand();
  void processUserCommand(std::vector<std::string> userCommand);
  void help();                                  // OK
  void helpCmd(std::vector<std::string> input); // OK
  void prod();                                  // OK

  void min(std::vector<std::string> input); // OK
  void max(std::vector<std::string> input); // OK
  void avg(std::vector<std::string> input);
  void predict();
  void time(); // OK
  void step(); // OK
  void ownCommand();

  // Helper functions
  OrderBookType askOrBid(std::vector<std::string> input);

  OrderBook orderBook{"20200601.csv"};

  std::string currentTime;
};
