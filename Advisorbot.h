#pragma once

#include <string>
#include <vector>
#include "OrderBookEntry.h"
#include "OrderBook.h"
#include "CSVReader.h"

class Advisorbot
{
public:
  Advisorbot();
  void init();

private:
  void advisorbotCommand();
  std::vector<std::string> getUserCommand();
  void processUserCommand(std::vector<std::string> userCommand);
  void help();                                  // OK
  void helpCmd(std::vector<std::string> input); // OK
  void prod();                                  // OK
  void min(std::vector<std::string> input);     // OK
  void max(std::vector<std::string> input);     // OK
  void avg(std::vector<std::string> input);     // OK
  void time();                                  // OK
  void step();                                  // OK
  void info(std::vector<std::string> input);    // OK

  // Helper functions
  OrderBookType askOrBid(std::vector<std::string> input);
  OrderBook orderBook{"20200601.csv"};
  std::string currentTime;
};
