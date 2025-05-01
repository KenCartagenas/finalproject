#ifndef ED_H
#define ED_H
#include <string>

string characters = "qwertyuiopasdfghjklzxcvbnmQWERTYUIOPASDFGHJKLZXCVBNM1234567890!@#$%^&*_+";
string encrypt(string password, int id);
string decrypt(string password, int id);
#endif