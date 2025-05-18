#include <iostream>
#include "data.h"
#include "account.h"
#include "mainmenu.h"
using namespace std;

int main()
{
    loadData();
    while (isRunning)
    {
        
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
