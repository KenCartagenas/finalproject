#include <iostream>
#include "account.h"
#include "data.h"

void displayAccountTemplate(string text)
{
    cout << "    Welcome to Student Grades Management System" << endl;
    cout << "-------------------------------------------------------" << endl;
    cout << text;
    cout << "-------------------------------------------------------" << endl;
    cout << ">> ";
}

bool isPasswordStrong(string password)
{
    int capital = 0, number = 0, specialCharacter = 0;
    for (int i = 0; i < password.size(); i++)
    {
        if(isupper(password[i]) != 0)
        {
            capital++;
        }
        if(isalnum(password[i]) != 0)
        {
            number++;
        }
        if (!isalnum(password[i]) != 0 && isspace(password[i]) != 0)
        {
            specialCharacter++;
        }
    }
    if(capital != 0 && number != 0 && specialCharacter != 0)
    {
        return true;
    }

    cout << "There are either no capital letter, number, or special character." << endl;
    return false;
}

void accountMenu()
{
    char option;
    while(isRunning && !isLoggedIn)
    {
        displayAccountTemplate("[A] - Log In\n[B] - Sign Up\n[C] - Forget Password\n[D] - Quit\n");
        cin >> option;
        switch(tolower(option))
        {
            case 'a': signIn(); isLoggedIn = true; break;
            case 'b': signUp(); break;
            case 'c': forgotPassword(); break;
            case 'd': cout << "Thank you"; isRunning = false;
            default: break;
        }
    }
}

void signIn()
{
    string password;
    int idnumber;

    while (true)
    {
        displayAccountTemplate("Enter ID Number:\n");
        cin >> idnumber;

        system("cls");

        displayAccountTemplate("Enter Password:\n");
        getline(cin, password);

        if(idnumber <= size)
        {
            if(logInCredential[idnumber].password == password)
            {
                isLoggedIn = true;
                break;
            }
        }
    }
}

void signUp()
{
    string username, password, role, temprole, a;

    displayAccountTemplate("Create a New Account!\n\nEnter your username: ");
    getline(cin, username);
    
    
    while (true)
    {
        displayAccountTemplate("Create a New Account!\n\nPassword should at least have:\n\t1 capital letter\n\t1 number\n\t1 special character\nEnter your password: ");
        getline(cin, password);
        if(isPasswordStrong(password))
        {
            break;
        }
    }
    
    while (true)
    {
        displayAccountTemplate("Create a New Account!\n\nEnter your role(student, teacher, or admin): ");
        getline(cin, temprole);
        for (int i = 0; i < temprole.size(); i++)
        {
            role += tolower(temprole[i]);
        }
        
        if(role == "student" || role == "teacher" || role == "admin")
        {
            break;
        }
    }

    logInCredential[size - 1].id = 2500000 + size;
    logInCredential[size - 1].username = username;
    logInCredential[size - 1].password = password;
    logInCredential[size - 1].role = role;

    cout << "    Welcome to Student Grades Management System" << endl;
    cout << "-------------------------------------------------------" << endl;
    cout << "Your username: " << logInCredential[size - 1].username << endl;
    cout << "Your password: " << logInCredential[size - 1].password << endl;
    cout << "Your id number: " << logInCredential[size - 1].id << endl;
    cout << "Your role: " << logInCredential[size - 1].role << endl;
    cout << "-------------------------------------------------------" << endl;
    cout << ">> ";
    getline(cin, a);
    isLoggedIn = true;
}

void forgotPassword()
{

}
