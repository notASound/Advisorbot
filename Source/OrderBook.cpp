#include "OrderBook.h"
#include "CSVReader.h"
#include "OrderBookEntry.h"
#include <map>
#include <iostream>

/** construct, reading a csv data file */
OrderBook::OrderBook(std::string filename)
{
    orders = CSVReader::readCSV(filename);
};

/** return vector of all known products in the dataset */
std::vector<std::string> OrderBook::getKnownProducts()
{
    std::vector<std::string> products;

    std::map<std::string, bool> prodMap;

    for (OrderBookEntry &e : orders)
    {
        // Get list of unique products to a map
        prodMap[e.product] = true;
    }
    // Flatten the map to a vector of strings
    for (auto const &e : prodMap)
    {
        products.push_back(e.first);
    }
    // Products  should contain an unique list of products
    return products;
};

/** return vector of all unique timestamps in the dataset */
std::vector<std::string> OrderBook::getTimestamps()
{
    std::vector<std::string> timestamp;

    std::map<std::string, bool> timestampMap;

    for (OrderBookEntry &e : orders)
    {
        // Get list of timestamps to a map
        timestampMap[e.timestamp] = true;
    }
    // Flatten the map to a vector of strings
    for (auto const &e : timestampMap)
    {
        timestamp.push_back(e.first);
    }
    // Timestamp  should contain an unique list of timestamps
    return timestamp;
};
/** return vector of Orders according to the sent filters */
std::vector<OrderBookEntry> OrderBook::getOrders(OrderBookType type,
                                                 std::string product,
                                                 std::string timestamp)
{
    std::vector<OrderBookEntry> orders_sub;

    for (OrderBookEntry &e : orders)
    {
        if (e.orderType == type &&
            e.product == product &&
            e.timestamp == timestamp)
        {
            // std::cout << e.price << std::endl;
            orders_sub.push_back(e);
        }
    }

    return orders_sub;
};

double OrderBook::getHighPrice(std::vector<OrderBookEntry> &orders)
{

    double max = orders[0].price;
    for (OrderBookEntry &e : orders)
    {
        if (e.price > max)
        {
            max = e.price;
        }
    }
    return max;
};

double OrderBook::getLowPrice(std::vector<OrderBookEntry> &orders)
{

    double min = orders[0].price;
    for (OrderBookEntry &e : orders)
    {
        if (e.price < min)
        {
            min = e.price;
        }
    }
    return min;
};

double OrderBook::getAveragePrice(std::vector<OrderBookEntry> &orders)
{
    // Iterates over orders and sums up prices
    double avg;
    for (OrderBookEntry &e : orders)
    {
        avg = avg + e.price;
    }
    return avg / orders.size();
};

std::string OrderBook::getEarliestTime()
{
    return orders[0].timestamp;
};

std::string OrderBook::getNextTime(std::string timestamp)
{
    std::string next_timestamp = "";
    for (OrderBookEntry &e : orders)
    {
        if (e.timestamp > timestamp)
        {
            next_timestamp = e.timestamp;
            break;
        }
    }
    if (next_timestamp == "")
    {
        next_timestamp = orders[0].timestamp;
    }
    return next_timestamp;
};