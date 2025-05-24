#include "utils.h"
#include "data.h"
#include <iostream>
#include <limits>
#include <conio.h>

int getInt()
{
    int num;
    while (true)
    {
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
    std::cout << "Press any key to continue\n>> ";
    getch();
    c();
}

int indexFind(int id)
{
    for (int i = 0; i < logInCredential.size(); i++)
    {
        if (id == logInCredential[i].id)
        {
            userIndex = i;
        }
    }
}

//
void displayTemplate(string text)
{
  cout << "=========================================" << endl;
  cout << "    Student Grades Management System" << endl;
  cout << "=========================================" << endl;
  cout << text << endl;
  cout << "-----------------------------------------" << endl;
}