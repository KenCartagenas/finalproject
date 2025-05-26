#include "ED.h"
#include "data.h"
#include <string>
#include <cstdlib>
#include <time.h>
using namespace std;

string characters = "qwertyuiopasdfghjklzxcvbnmQWERTYUIOPASDFGHJKLZXCVBNM1234567890!@#$%^&*_+";

string encrypt(string password, int id, int index)
{
    srand(time(0));
    int space = (100 - password.size()) / password.size();
    int random, pos;
    string encryptedPassword;

    for (int i = 0; i < password.size(); i++)
    {
        for (int j = 0; j < space; j++)
        {
            random = rand() % characters.size();
            encryptedPassword += characters[random];
        }
        pos = (characters.find(password[i]) + (id % 10)) % characters.size();
        encryptedPassword += characters[pos];
    }
    logInCredential[index].space = space;
    return encryptedPassword;
}

string decrypt(string password, int id, int index)
{
    int space = logInCredential[index].space;
    int pos;
    string decryptedPassword;

    for (int i = space; i < password.size(); i += space + 1)
    {
        pos = characters.find(password[i]) - (id % 10);
        if (pos < 0)
        {
            pos += 72;
        }
        decryptedPassword += characters[pos];
    }
    return decryptedPassword;
}
