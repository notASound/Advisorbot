#include "Advisorbot.h"
#include "CSVReader.h"
#include "OrderBook.h"

#include <string>
#include <iostream>
#include <vector>

Advisorbot::Advisorbot()
{
}

void Advisorbot::init()
{
    std::string input;
    currentTime = orderBook.getEarliestTime();

    while (true)
    {

        advisorbotCommand();
        input = getUserCommand();
        processUserCommand(input);
    }
};

/* Loads CSV file */

// void Advisorbot::loadOrderBook() - will be replaced with OrderBook class
// {
//     // Returns vector of order book entries
//     orders = CSVReader::readCSV("20200601.csv");
// }

/** Initiate command of advisorbot */
void Advisorbot::advisorbotCommand()
{
    std::cout << "Please enter a command, or help for a list of commands" << std::endl;
};

/** Get user command and store it in a string userCommand */
std::string Advisorbot::getUserCommand()
{
    std::string userCommand;
    std::getline(std::cin, userCommand);

    return userCommand;
};

// std::vector<std::string> Advisorbot::tokeniseUserCommand(){};

/** Process user command */
void Advisorbot::processUserCommand(std::string input)
{
    std::cout << "User typed: " << input << std::endl;
    if (input == "help")
    {
        help();
    };

    if (input == "help prod" ||
        input == "help min" ||
        input == "help max" ||
        input == "help avg" ||
        input == "help predict" ||
        input == "help time" ||
        input == "help step" ||
        input == "help ownCommand")
    {
        helpCmd(input);
    };
    if (input == "prod")
    {
        prod();
    };
    if (input == "min")
    {
        min();
    };
    if (input == "max")
    {
        max();
    };
    if (input == "avg")
    {
        avg();
    };
    if (input == "predict")
        predict();
    {};
    if (input == "time")
    {
        time();
    };
    if (input == "step")
    {
        step();
    };
    if (input == "ownCommand")
    {
        ownCommand();
    }
};

void Advisorbot::help()
{
    std::cout << "List of available commands: help, help <cmd>, prod, min, max, avg, predict, time, step, ownCommand" << std::endl;
};

void Advisorbot::helpCmd(std::string input)
{
    if (input == "help prod")
    {
        std::cout << "List of available products" << std::endl;
    }
    if (input == "help min")
    {
        std::cout << "Find minimum bid or ask for product in current time step" << std::endl;
    }
    if (input == "help max")
    {
        std::cout << "Find maximum bid or ask for product in current time step" << std::endl;
    }
    if (input == "help avg")
    {
        std::cout << "Compute average ask or bid for the sent product over the sent number of time steps" << std::endl;
    }
    if (input == "help predict")
    {
        std::cout << "Predict max or min ask or bid for the sent product for the next time step" << std::endl;
    }
    if (input == "help time")
    {
        std::cout << "State current time in dataset, i.e. which timeframe are we looking at" << std::endl;
    }
    if (input == "help step")
    {
        std::cout << "Move to the next time step" << std::endl;
    }
};

void Advisorbot::prod()
{
    std::cout << "Available products: " << std::endl;
    std::vector<std::string> products = orderBook.getKnownProducts();
    for (auto i : products)
    {
        std::cout << i << ' ' << std::endl; // will print: "a b c"
    }
};

void Advisorbot::min()
{
    {
        for (std::string const &p : orderBook.getKnownProducts())
        {
            std::cout << "Product: " << p << std::endl;
            std::vector<OrderBookEntry> entries = orderBook.getOrders(OrderBookType::ask,
                                                                      p,
                                                                      currentTime);
            std::cout << "Asks seen: " << entries.size() << std::endl;
            std::cout << "Min ask: " << OrderBook::getLowPrice(entries) << std::endl;
        }
    };
};
void Advisorbot::max()
{
    for (std::string const &p : orderBook.getKnownProducts())
    {
        std::cout << "Product: " << p << std::endl;
        std::vector<OrderBookEntry> entries = orderBook.getOrders(OrderBookType::ask,
                                                                  p,
                                                                  currentTime);
        std::cout << "Asks seen: " << entries.size() << std::endl;
        std::cout << "Max ask: " << OrderBook::getHighPrice(entries) << std::endl;
    }
};
void Advisorbot::avg()
{
    std::cout << "avg" << std::endl;
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
    std::cout << "step" << std::endl;
    currentTime = orderBook.getNextTime(currentTime);
};

void Advisorbot::ownCommand()
{
    std::cout << "ownCommand" << std::endl;
};
