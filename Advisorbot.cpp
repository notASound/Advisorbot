#include "Advisorbot.h"
#include "CSVReader.h"
#include "OrderBook.h"

#include <string>
#include <iostream>
#include <vector>
#include <algorithm>

Advisorbot::Advisorbot() {}

void Advisorbot::init()
{
    std::vector<std::string> input;
    currentTime = orderBook.getEarliestTime();

    while (true)
    {
        advisorbotCommand();
        input = getUserCommand();
        processUserCommand(input);
        input = {}; // empties the vector of strings with user input after prosessing
    }
};

/** Init command of advisorbot */
void Advisorbot::advisorbotCommand()
{
    std::cout << "Please enter a command, or help for a list of commands. " << std::endl;
};

/** Get, store and tokenise user command */
std::vector<std::string> Advisorbot::getUserCommand()
{
    std::string userCommand;
    std::cout << "User typed: " << userCommand << std::endl;
    std::getline(std::cin, userCommand);                                     // get and store user command
    std::vector<std::string> tokens = CSVReader::tokenise(userCommand, ' '); // tokenise user command
    return tokens;
};

/** Init appropriate functions based on the users input */
void Advisorbot::processUserCommand(std::vector<std::string> input)
{
    if (input[0] == "help")
        help();
    if (input[1] == "prod" ||
        input[1] == "min" ||
        input[1] == "max" ||
        input[1] == "avg" ||
        input[1] == "time" ||
        input[1] == "step" ||
        input[1] == "info")
        helpCmd(input);
    if (input[0] == "prod")
        prod();
    if (input[0] == "min")
        min(input);
    if (input[0] == "max")
        max(input);
    if (input[0] == "avg")
        avg(input);
    if (input[0] == "time")
        time();
    if (input[0] == "step")
        step();
    if (input[0] == "info")
        info(input);
};

void Advisorbot::help()
{
    std::cout << "List of available commands: help, help <cmd>, prod, min, max, avg, predict, time, step, info" << std::endl;
};

void Advisorbot::helpCmd(std::vector<std::string> input)
{
    if (input[1] == "prod")
        std::cout << "List of available products" << std::endl;
    if (input[1] == "min")
        std::cout << "Find minimum bid or ask for product in current time step Eg min BTC/USTD bid" << std::endl;
    if (input[1] == "max")
        std::cout << "Find maximum bid or ask for product in current time ste. Eg max ETH/BTC ask" << std::endl;
    if (input[1] == "avg")
        std::cout << "Compute average ask or bid for the sent product over the sent number of time steps. Eg avg ETH/BTC ask 5" << std::endl;
    if (input[1] == "time")
        std::cout << "State current time in dataset, i.e. which timeframe are we looking at" << std::endl;
    if (input[1] == "step")
        std::cout << "Move to the next time step" << std::endl;
    if (input[1] == "info")
        std::cout << "Get all the information about typed currency in current time. Eg info ETH/BTC" << std::endl;
};

void Advisorbot::prod()
{
    std::cout << "Available products: " << std::endl;
    std::vector<std::string> products = orderBook.getKnownProducts();
    for (auto i : products) // iterate over unique products and print them out
        std::cout << i << std::endl;
};

void Advisorbot::min(std::vector<std::string> input)
{
    // Check if user input is valid to use min command
    if (input.size() <= 1)
        std::cout << "Not sufficient data to print min stats. Try again og type 'help min' for help." << std::endl;
    else
    {
        std::vector<std::string> products = orderBook.getKnownProducts(); // get a list of unique products

        if (std::find(products.begin(), products.end(), input[1]) != products.end()) // check if asked product is found
        {
            OrderBookType ask_bid = askOrBid(input); // check if user wants ask or bid stats
            if (input[2] == "ask" || input[2] == "bid")
            {
                std::vector<OrderBookEntry> entries = orderBook.getOrders(ask_bid,
                                                                          input[1],
                                                                          currentTime);
                std::cout << "The min " << input[2] << " for " << input[1] << " is " << OrderBook::getLowPrice(entries) << std::endl; // print out stats
            }
            else
                std::cout << "Wrong transaction type - use 'ask' or 'bid'" << std::endl;
        }
        else
            std::cout << "Product not found" << std::endl;
    };
};

void Advisorbot::max(std::vector<std::string> input)
{
    // Check if user input is valid to use min command
    if (input.size() <= 1)
        std::cout << "Not sufficient data to print max stats. Try again og type 'help max' for help." << std::endl;
    else
    {
        std::vector<std::string> products = orderBook.getKnownProducts(); // get a list of unique products

        if (std::find(products.begin(), products.end(), input[1]) != products.end()) // check if asked product is found
        {
            OrderBookType ask_bid = askOrBid(input); // check if user wants ask or bid stats

            if (input[2] == "ask" || input[2] == "bid")
            {
                std::vector<OrderBookEntry> entries = orderBook.getOrders(ask_bid,
                                                                          input[1],
                                                                          currentTime);
                std::cout << "The max " << input[2] << " for " << input[1] << " is " << OrderBook::getHighPrice(entries) << std::endl; // print out stats
            }
            else
                std::cout << "Wrong transaction type - use 'ask' or 'bid'" << std::endl;
        }
        else
            std::cout << "Product not found" << std::endl;
    };
};

void Advisorbot::avg(std::vector<std::string> input)
{
    OrderBookType ask_bid = askOrBid(input);                         // check if user wants ask or bids stats
    double timesteps = stod(input[3]);                               // string to double - how many timesteps user wants to iterate over
    std::vector<std::string> timestamps = orderBook.getTimestamps(); // get all timestamps from the dataset

    /** Get index of current time in timestamps */
    std::vector<std::string>::iterator itr = std::find(timestamps.begin(), timestamps.end(), currentTime);
    int index = std::distance(timestamps.begin(), itr);

    /** Get orders with current timestamp and store them in entries */
    std::vector<OrderBookEntry> entries = orderBook.getOrders(ask_bid,
                                                              input[1],
                                                              currentTime);

    /** Adds more orders to entries based on how many timesteps user requests */
    for (size_t i = 0; i < timesteps - 1; i++)
    {
        std::vector<OrderBookEntry> orders = orderBook.getOrders(OrderBookType::ask,
                                                                 input[1],
                                                                 timestamps[index + 1]);
        index++; // update index used in getOrders to get orders with next timestamp
        entries.insert(std::end(entries), std::begin(orders), std::end(orders));
    }
    std::cout << "The average " << input[1] << " " << input[2] << " price over the last " << input[3] << " timesteps is " << OrderBook::getAveragePrice(entries) << std::endl;
};

void Advisorbot::time()
{
    std::cout << "Current time is " << currentTime << std::endl;
};

void Advisorbot::step()
{
    currentTime = orderBook.getNextTime(currentTime);
    std::cout << "New current time is " << currentTime << std::endl;
};

void Advisorbot::info(std::vector<std::string> input)
{
    std::cout << "Current time is " << currentTime << std::endl;
    std::vector<OrderBookEntry> askEntries = orderBook.getOrders(OrderBookType::ask,
                                                                 input[1],
                                                                 currentTime);
    std::cout << input[1] << " - asks seen: " << askEntries.size() << std::endl;

    std::cout << "The max ask price for " << input[1] << " is " << OrderBook::getHighPrice(askEntries) << std::endl;
    std::cout << "The min ask price for " << input[1] << " is " << OrderBook::getLowPrice(askEntries) << std::endl;

    std::vector<OrderBookEntry> bidEntries = orderBook.getOrders(OrderBookType::bid,
                                                                 input[1],
                                                                 currentTime);

    std::cout << input[1] << " - bids seen: " << bidEntries.size() << std::endl;
    std::cout << "The max bid price for " << input[1] << " is " << OrderBook::getHighPrice(bidEntries) << std::endl;
    std::cout << "The min bid price for " << input[1] << " is " << OrderBook::getLowPrice(bidEntries) << std::endl;
};

/** Check if user asks for ask or bid */
OrderBookType Advisorbot::askOrBid(std::vector<std::string> input)
{
    OrderBookType orderType;
    // Bid or ask
    if (input[2] == "ask")
    {
        orderType = OrderBookType::ask;
    }
    else if (input[2] == "bid")
    {
        orderType = OrderBookType::bid;
    }
    else
    {
        orderType = OrderBookType::unknown;
    }
    return orderType;
};