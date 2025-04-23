#ifndef DATA_H
#define DATA_H

#include <string>
#include <vector>

struct User
{
  int role;
  string username;
  string password;
};

vector<User> logInCredential;

void loadData();
void saveData();
#endif
