#ifndef DATA_H
#define DATA_H

#include <string>
#include <vector>
using namespace std;

//general data
struct User
{
  int role;
  int id;
  string username;
  string password;
};

vector<User> logInCredential;

//functions
void loadData(vector<User> &logInCredential);
void saveData(vector<User> &logInCredential);
#endif
