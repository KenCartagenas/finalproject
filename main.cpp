#include <iostream>
#include "data.h"
#include "account.h"
#include "mainmenu.h"
using namespace std;


int main()
{
    int exit;
    //loadData();
    while(isRunning)
    {
        accountMenu();
        if(isLoggedIn == true)
        {
            cout << "wow" << endl;
            cin >> exit;
            if (exit == 1)
            {
                break;
            }
            
            //mainMenu();
        }
        //saveData();
    }
    
}
