#include <iostream>
#include "data.h"
#include "account.h"
#include "mainmenu.h"
using namespace std;


int main()
{
    //loadData();
    while(isRunning)
    {
        accountMenu();
        if(isLoggedIn == true)
        {
            mainMenu();
        }
        //saveData();
    }
    
}
