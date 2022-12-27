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
        // Empties the vector of strings with user input after prosessing
        input = {};
    }
};

/** Initiate command of advisorbot */
void Advisorbot::advisorbotCommand()
{
    std::cout << "Please enter a command, or help for a list of commands. " << std::endl;
};

/** Get user command and store it in a string userCommand */
std::vector<std::string> Advisorbot::getUserCommand()
{
    std::string userCommand;
    std::getline(std::cin, userCommand);

    // std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

    std::cout << "User typed: " << userCommand << std::endl;

    std::vector<std::string> tokens = CSVReader::tokenise(userCommand, ' ');
    // if (tokens.size() != 3)
    // {
    //     std::cout << "Bad input!" << userCommand << std::endl;
    // };
    std::cout << "Tokens[0] = " << tokens[0] << std::endl;
    std::cout << "Tokens[1] = " << tokens[1] << std::endl;
    std::cout << "Tokens[2] = " << tokens[2] << std::endl;
    std::cout << "Tokens[3] = " << tokens[3] << std::endl;
    std::cout << "Tokens size: " << tokens.size() << std::endl;
    return tokens; // TODO! Needs to be changed to tokens (!Function has to return std::vector<std::string>)
};

/** Process user command */
void Advisorbot::processUserCommand(std::vector<std::string> input)
{
    if (input[0] == "help")
    {
        help();
    };
    if (input[1] == "prod" ||
        input[1] == "min" ||
        input[1] == "max" ||
        input[1] == "avg" ||
        input[1] == "predict" ||
        input[1] == "time" ||
        input[1] == "step" ||
        input[1] == "ownCommand")
    {
        helpCmd(input);
    };
    if (input[0] == "prod")
    {
        prod();
    };
    if (input[0] == "min")
    {
        min(input);
    };
    if (input[0] == "max")
    {
        max(input);
    };
    if (input[0] == "avg")
    {
        avg(input);
    };
    // if (input == "predict")
    //     predict();
    // {};
    if (input[0] == "time")
    {
        time();
    };
    if (input[0] == "step")
    {
        step();
    };
    // if (input == "ownCommand")
    // {
    //     ownCommand();
    // }
};

void Advisorbot::help()
{
    std::cout << "List of available commands: help, help <cmd>, prod, min, max, avg, predict, time, step, ownCommand" << std::endl;
};

void Advisorbot::helpCmd(std::vector<std::string> input)
{
    if (input[1] == "prod")
    {
        std::cout << "List of available products" << std::endl;
    }
    if (input[1] == "min")
    {
        std::cout << "Find minimum bid or ask for product in current time step" << std::endl;
    }
    if (input[1] == "max")
    {
        std::cout << "Find maximum bid or ask for product in current time step" << std::endl;
    }
    if (input[1] == "avg")
    {
        std::cout << "Compute average ask or bid for the sent product over the sent number of time steps" << std::endl;
    }
    if (input[1] == "predict")
    {
        std::cout << "Predict max or min ask or bid for the sent product for the next time step" << std::endl;
    }
    if (input[1] == "time")
    {
        std::cout << "State current time in dataset, i.e. which timeframe are we looking at" << std::endl;
    }
    if (input[1] == "step")
    {
        std::cout << "Move to the next time step" << std::endl;
    }
    if (input[1] == "ownCommand")
    {
        std::cout << "ownCommand initialised" << std::endl;
    }
};

void Advisorbot::prod()
{
    std::cout << "Available products: " << std::endl;
    std::vector<std::string> products = orderBook.getKnownProducts();
    for (auto i : products)
    {
        std::cout << i << std::endl;
    }
};

void Advisorbot::min(std::vector<std::string> input)
{
    // Check if user input is valid to use min command
    if (input.size() <= 1)
    {
        std::cout << "Not sufficient data to print min stats. Try again og type 'help min' for help." << std::endl;
    }
    else
    {
        // Get vector of strings with found products
        std::vector<std::string> products = orderBook.getKnownProducts();

        // Check if asked product is found in products and print out stats
        if (std::find(products.begin(), products.end(), input[1]) != products.end())
        {
            std::cout << "Product found: " << input[1] << std::endl;
            // Check if user stats for bid or ask
            if (input[2] == "ask") // if user wants ask stats
            {
                std::vector<OrderBookEntry> entries = orderBook.getOrders(OrderBookType::ask,
                                                                          input[1],
                                                                          currentTime);
                // std::cout << "Asks seen: " << entries.size() << std::endl;
                std::cout << "The min ask for " << input[1] << " is " << OrderBook::getLowPrice(entries) << std::endl;
            }
            else if (input[2] == "bid") // if user wants bid stats
            {
                std::vector<OrderBookEntry> entries = orderBook.getOrders(OrderBookType::bid,
                                                                          input[1],
                                                                          currentTime);
                // std::cout << "Bids seen: " << entries.size() << std::endl;
                std::cout << "The min bid for " << input[1] << " is " << OrderBook::getLowPrice(entries) << std::endl;
            }
            else // bad input
            {
                std::cout << "Product not found" << std::endl;
            }
        }
    };
};

void Advisorbot::max(std::vector<std::string> input)
{
    // Check if user input is valid to use min command
    if (input.size() <= 1)
    {
        std::cout << "Not sufficient data to print max stats. Try again og type 'help max' for help." << std::endl;
    }
    else
    {
        // Get vector of strings with found products
        std::vector<std::string> products = orderBook.getKnownProducts();

        // Check if asked product is found in products and print out stats
        if (std::find(products.begin(), products.end(), input[1]) != products.end())
        {
            std::cout << "Product found: " << input[1] << std::endl;
            // Check if user stats for bid or ask
            if (input[2] == "ask") // if user wants ask stats
            {
                std::vector<OrderBookEntry> entries = orderBook.getOrders(OrderBookType::ask,
                                                                          input[1],
                                                                          currentTime);
                // std::cout << "Asks seen: " << entries.size() << std::endl;
                std::cout << "The max ask for " << input[1] << " is " << OrderBook::getHighPrice(entries) << std::endl;
            }
            else if (input[2] == "bid") // if user wants bid stats
            {
                std::vector<OrderBookEntry> entries = orderBook.getOrders(OrderBookType::bid,
                                                                          input[1],
                                                                          currentTime);
                // std::cout << "Bids seen: " << entries.size() << std::endl;
                std::cout << "The max bid for " << input[1] << " is " << OrderBook::getHighPrice(entries) << std::endl;
            }
            else // bad input
            {
                std::cout << "Product not found" << std::endl;
            }
        }
    };
};
void Advisorbot::avg(std::vector<std::string> input)
{
    // Check if user wants ask or bid stats
    OrderBookType ask_bid = askOrBid(input);

    // String to double
    double timesteps = stod(input[3]); // how many timesteps

    // Prints out timestamps
    std::cout << "Timestamps: " << std::endl;
    std::vector<std::string> timestamps = orderBook.getTimestamps();
    std::cout << "Timestamp 0 is " << timestamps[0] << std::endl;
    for (auto i : timestamps)
    {
        std::cout << i << std::endl;
    }

    // Get orders with current timestamp and store in entries
    std::vector<OrderBookEntry> entries = orderBook.getOrders(ask_bid,
                                                              input[1],
                                                              currentTime);

    // Get index of current time in timestamps
    std::vector<std::string>::iterator itr = std::find(timestamps.begin(), timestamps.end(), currentTime);
    int index = std::distance(timestamps.begin(), itr);
    // std::cout << "Current time present at index: " << index << std::endl;

    // Adds more orders to entries based on requested amount timesteps
    for (size_t i = 0; i < timesteps - 1; i++)
    {
        std::vector<OrderBookEntry> orders = orderBook.getOrders(OrderBookType::ask,
                                                                 input[1],
                                                                 timestamps[index + 1]);

        index++; // Update index used in getOrders to get orders with next timestamp
        // std::cout << "New index is " << index << std::endl;

        entries.insert(std::end(entries), std::begin(orders), std::end(orders));
    }

    std::cout << "Entries size: " << entries.size() << std::endl;

    std::cout << "The avg " << input[2] << " for " << input[1] << " is " << OrderBook::getAveragePrice(entries) << std::endl;
};

void Advisorbot::predict()
{
    std::cout << "predict" << std::endl;
};
void Advisorbot::time()
{
    std::cout << "Current time is " << currentTime << std::endl;
};
void Advisorbot::step()
{
    std::cout << "Going to the next timestep" << std::endl;
    currentTime = orderBook.getNextTime(currentTime);
    std::cout << "New current time is " << currentTime << std::endl;
};

void Advisorbot::ownCommand()
{
    std::cout << "ownCommand" << std::endl;
};

// Helper functions
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