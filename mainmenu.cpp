#include "mainmenu.h"
#include <iostream>
#include <string>
#include "data.h"
using namespace std;

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
  
}

void studentMenu()
{
  
}
