#ifndef DATA_H
#define DATA_H

#include <string>
#include <vector>
using namespace std;

//general data
struct User
{
  string role;
  int id;
  int space;
  string username;
  string password;
};

extern vector<User> logInCredential;
bool isLoggedIn;
bool isRunning = true;
int size = logInCredential.size();

//functions
void loadData();
void saveData();
#endif
