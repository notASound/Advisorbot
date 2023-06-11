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
    std::getline(std::cin, userCommand); // get and store user command
    std::cout << "User typed: " << userCommand << std::endl;
    std::vector<std::string> tokens = CSVReader::tokenise(userCommand, ' '); // tokenise user command
    return tokens;
};

/** Init appropriate functions based on the users input */
void Advisorbot::processUserCommand(std::vector<std::string> input)
{
    if (input[0] == "help" && input.size() == 1)
        help();
    if (input[1] == "prod" ||
        input[1] == "min" ||
        input[1] == "max" ||
        input[1] == "avg" ||
        input[1] == "predict" ||
        input[1] == "time" ||
        input[1] == "step" ||
        input[1] == "info" && input.size() == 2)
        helpCmd(input);
    if (input[0] == "prod" && input.size() == 1)
        prod();
    if (input[0] == "min")
        min(input);
    if (input[0] == "max")
        max(input);
    if (input[0] == "avg")
        avg(input);
    if (input[0] == "predict")
        predict(input);
    if (input[0] == "time" && input.size() == 1)
        time();
    if (input[0] == "step" && input.size() == 1)
        step();
    if (input[0] == "info")
        info(input);
};

/** Print out available functions */
void Advisorbot::help()
{
    std::cout << "List of available commands: help, help <cmd>, prod, min, max, avg, predict, time, step, info" << std::endl;
};

/** Print out help for certain functions */
void Advisorbot::helpCmd(std::vector<std::string> input)
{
    if (input[1] == "prod")
        std::cout << "List of available products" << std::endl;
    if (input[1] == "min")
        std::cout << "Find minimum bid or ask for product in current timestep. Example: min BTC/USDT bid" << std::endl;
    if (input[1] == "max")
        std::cout << "Find maximum bid or ask for product in current timestep. Example: max ETH/BTC ask" << std::endl;
    if (input[1] == "avg")
        std::cout << "Compute average ask or bid for the sent product over the sent number of timesteps. Example: avg ETH/BTC ask 5" << std::endl;
    if (input[1] == "predict")
        std::cout << "Predict max or min ask or bid for the sent product for the next timestep. Example: predict ETH/BTC ask max 3" << std::endl;
    if (input[1] == "time")
        std::cout << "State current time in dataset, i.e. which timeframe is processed" << std::endl;
    if (input[1] == "step")
        std::cout << "Move to the next time step" << std::endl;
    if (input[1] == "info")
        std::cout << "Get all the information about typed currency in current time. Example: info ETH/BTC" << std::endl;
}

/** Print out available products */
void Advisorbot::prod()
{
    std::cout << "Available products: " << std::endl;
    std::vector<std::string> products = orderBook.getKnownProducts();
    for (auto i : products) // iterate over unique products and print them out
        std::cout << i << std::endl;
};

/** Print out min stats */
void Advisorbot::min(std::vector<std::string> input)
{
    // Check if user input is valid to use min command
    if (input.size() != 3) // bad input
        std::cout << "Wrong command. Try again og type 'help min' for help." << std::endl;
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

/** Print out max stats */
void Advisorbot::max(std::vector<std::string> input)
{
    // Check if user input is valid to use min command
    if (input.size() != 3) // bad input
        std::cout << "Wrong command. Try again og type 'help max' for help." << std::endl;
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

/** Print out average stats */
void Advisorbot::avg(std::vector<std::string> input)
{
    if (input.size() != 4) // bad input
        std::cout << "Wrong command. Try again og type 'help avg' for help." << std::endl;
    else
    {
        OrderBookType ask_bid = askOrBid(input);                         // check if user wants ask or bids stats
        int timesteps = stod(input[3]);                                  // convert string to int - how many timesteps user wants to iterate over
        std::vector<std::string> timestamps = orderBook.getTimestamps(); // get all timestamps from the dataset

        /** Get index of current time in timestamps */
        std::vector<std::string>::iterator itr = std::find(timestamps.begin(), timestamps.end(), currentTime);
        int index = std::distance(timestamps.begin(), itr);

        /** Get orders with current timestamp and store them in entries */
        std::vector<OrderBookEntry> entries = orderBook.getOrders(ask_bid,
                                                                  input[1],
                                                                  currentTime);

        /** Adds more orders to entries based on how many timesteps user requests */
        for (size_t i = 0; i <= timesteps - 1; i++) // timesteps - 1 because we already have orders from current timestep
        {
            std::vector<OrderBookEntry> orders = orderBook.getOrders(OrderBookType::ask,
                                                                     input[1],
                                                                     timestamps[index - 1]); // index - 1 because we already have orders from current timestep
            index--;                                                                         // update index used in getOrders to get orders from previous timestep with every loop
            entries.insert(std::end(entries), std::begin(orders), std::end(orders));
        }

        double averagePrice = OrderBook::getAveragePrice(entries);

        std::cout
            << "The average " << input[1] << " " << input[2] << " price over the last " << input[3] << " timesteps is " << averagePrice << std::endl;
    }
};

/** Print out min/max bid/ask price prediction based on user requested amount timesteps */
void Advisorbot::predict(std::vector<std::string> input)
{

    if (input.size() != 5) // bad input
        std::cout << "Wrong command. Try again og type 'help predict' for help." << std::endl;
    else
    {
        OrderBookType ask_bid = askOrBid(input);                         // check if user wants ask or bids stats
        int timesteps = stod(input[4]);                                  // convert string to int - how many timesteps user wants to iterate over
        std::vector<std::string> timestamps = orderBook.getTimestamps(); // get all timestamps from the dataset
        std::vector<double> predictPrice;                                // vector that stores average from requested amount timesteps

        /** Get index of current time in timestamps */
        std::vector<std::string>::iterator itr = std::find(timestamps.begin(), timestamps.end(), currentTime);
        int index = std::distance(timestamps.begin(), itr);

        /** Get orders with current timestamp and store them in entries */
        std::vector<OrderBookEntry> entries = orderBook.getOrders(ask_bid,
                                                                  input[1],
                                                                  currentTime);
        /** Calculate and push back min/max prices into predictPrice for current timestep */
        if (input[3] == "min")
            predictPrice.push_back(OrderBook::getLowPrice(entries));
        else if (input[3] == "max")
            predictPrice.push_back(OrderBook::getHighPrice(entries));

        /** Adds more orders to entries based on how many timesteps user requests */
        for (size_t i = 0; i <= timesteps - 1; i++) // timesteps - 1 because we already have orders from current timestep
        {
            std::vector<OrderBookEntry> orders = orderBook.getOrders(ask_bid,
                                                                     input[1],
                                                                     timestamps[index - 1]); // index - 1 because we already have orders from current timestep
            index--;                                                                         // update index used in getOrders to get orders from previous timestep with every loop

            /** Calculate and push back min/max prices into the predictPrice for requested amount timesteps */
            if (input[3] == "min")
                predictPrice.push_back(OrderBook::getLowPrice(entries));
            else if (input[3] == "max")
                predictPrice.push_back(OrderBook::getHighPrice(entries));
        }
        std::cout << "Predicted price for " << input[1] << " " << input[3] << " " << input[2] << " based on the last " << input[4] << " timesteps is " << Advisorbot::predictPrice(predictPrice) << std::endl;
    }
};

/** Prints out current time */
void Advisorbot::time()
{
    std::cout << "Current time is " << currentTime << std::endl;
};

/** Go to the next timestep */
void Advisorbot::step()
{
    currentTime = orderBook.getNextTime(currentTime);
    std::cout << "Going to the next timestep. New current time is " << currentTime << std::endl;
};

/** Own command - print out stats about certain currency*/
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

/** Calculate predicted price based on moving average method */
double Advisorbot::predictPrice(std::vector<double> average)
{
    const int size = average.size();
    average[size];
    int l_size = 2;
    double sum;
    double movingAverage;

    for (int i = 0; i <= (size - l_size); i++)
    {
        sum = 0;
        for (int j = i; j < i + l_size; j++)
        {
            sum += average[j];
        }
        movingAverage = sum / l_size;
    }
    return movingAverage;
}