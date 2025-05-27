#include <iostream>
#include "data.h"
#include "account.h"
#include "mainmenu.h"
#include "calculate.h"
using namespace std;

int main()
{
    loadData();
    while (isRunning)
    {
        calculateAll();
        if (isLoggedIn == true)
        {
            mainMenu();
        }
        else
        {
            accountMenu();
        }
        saveData();
    }
}
