#include "utils.h"
#include <iostream>
#include <limits>
#include <conio.h>

int getInt()
{
    int num;
    while (true)
    {
        std::cout << ">> ";
        std::cin >> num;
        if (std::cin.fail())
        {
            std::cin.clear();                                                   // clear the error state
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); // discard invalid input
            std::cout << "Invalid input. Please enter a number.\n";
        }
        else
        {
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); // discard leftover input
            return num;
        }
    }
}

char getChar()
{
    char c;
    while (true)
    {
        std::cout << ">> ";
        std::cin >> c;
        if (std::cin.fail())
        {
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            std::cout << "Invalid input. Try again.\n";
        }
        else
        {
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            return c;
        }
    }
}

float getFloat()
{
    float c;
    while (true)
    {
        std::cout << ">> ";
        std::cin >> c;
        if (std::cin.fail())
        {
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            std::cout << "Invalid input. Try again.\n";
        }
        else
        {
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            return c;
        }
    }
}

void c()
{
    system("cls");
}

void get()
{
    std::cout << ">> ";
    getch();
    c();
}