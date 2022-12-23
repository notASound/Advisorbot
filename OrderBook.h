#pragma once
#include "OrderBookEntry.h"
#include "CSVReader.h"
#include <string>
#include <vector>

class OrderBook
{
public:
    /** construct, reading a csv data file */
    // Constructor
    OrderBook(std::string filename);
    /** return vector of all known products in the dataset */
    std::vector<std::string> getKnownProducts();
    /** return vector of Orders according to the sent filters */
    std::vector<OrderBookEntry> getOrders(OrderBook type,
                                          std::string product,
                                          std::string timestamp);

private:
    std::vector<OrderBookEntry> orders;
};