#include "mainmenu.h"
#include <iostream>
#include <string>
#include "data.h"
using namespace std;

void diplayMainMenuTemplate()
{
    system("cls")
    cout << "    Welcome to " << logInCredential[loggedInID].username << endl;
    cout << "-------------------------------------------------------" << endl;
    cout << text;
    cout << "-------------------------------------------------------" << endl;
    cout << ">> ";
}
void mainMenu()
{
  if(logInCredential[loggedInID].role == "teacher")
  {
    teacherMenu();
  }
  else if(logInCredential[loggedInID].role == "student")
  {
    studentMenu();
  }
}

void teacherMenu()
{
  char option;
  displayMainMenuTemplate(" a. Create New Course / Subject\nb. Open Existing Class\nc. Search Student\nd. Settings\ne. Log Out");
  cin >> option;

  switch(tolower(option))
    {
      case 'a': createCourse(); break;
      case 'b': break;
      case 'c': break;
      case 'd': break;
      case 'e': break;
    }
}

void studentMenu()
{
  
}

void CreateCourse()
{
  
}
