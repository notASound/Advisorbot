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
  // void loadOrderBook();
  void advisorbotCommand();
  std::string getUserCommand();
  // std::vector<std::string> tokeniseUserCommand(); - week 8 - Read a line from the user
  void processUserCommand(std::string userCommand);
  void help();                     // OK
  void helpCmd(std::string input); // OK
  void prod();                     // OK

  void min();
  void max();
  void avg();
  void predict();
  void time(); // OK
  void step(); // OK
  void ownCommand();

  OrderBook orderBook{"20200601.csv"};

  std::string currentTime;

  // OrderBook orderBook{"20200601.csv"};
};
