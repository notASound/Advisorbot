#pragma once

#include <string>
#include <vector>
#include "OrderBookEntry.h"

class Advisorbot
{

public:
  Advisorbot();
  void init();

private:
  // Functions to implement in cpp file
  void loadOrderBook();
  void advisorbotCommand();
  std::string getUserCommand();
  void processUserCommand(std::string userCommand);
  void help();
  void helpCmd(std::string input);
  void prod();
  void min();
  void max();
  void avg();
  void predict();
  void time();
  void step();
  void ownCommand();

  std::vector<OrderBookEntry> orders;

  // std::string currentTime;

  // OrderBook orderBook{"20200601.csv"};
};
