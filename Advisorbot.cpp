#include "Advisorbot.h"

#include <string>
#include <iostream>

Advisorbot::Advisorbot()
{
}

void Advisorbot::init()
{
    std::string input;

    /* Need so be checked out!*/
    // currentTime = orderBook.getEarliestTime();

    while (true)
    {
        advisorbotCommand();
        input = getUserCommand();
        processUserCommand(input);
    }
};

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

void Advisorbot::help(){

};

void Advisorbot::helpCmd(std::string input){

};

void Advisorbot::prod(){

};

void Advisorbot::min(){

};
void Advisorbot::max()
{
    std::cout << "max" << std::endl;
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
    std::cout << "time" << std::endl;
};
void Advisorbot::step()
{
    std::cout << "step" << std::endl;
};

void Advisorbot::ownCommand()
{
    std::cout << "ownCommand" << std::endl;
};
