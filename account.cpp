#include <iostream>
#include "account.h"
#include "data.h"
#include "ED.h"

void displayAccountTemplate(string text)
{
    cout << "    Welcome to Student Grades Management System" << endl;
    cout << "-------------------------------------------------------" << endl;
    cout << text << endl;
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
        if(isdigit(password[i]) != 0)
        {
            number++;
        }
        if (!isalnum(password[i]) && !isspace(password[i]))
        {
            specialCharacter++;
        }
    }
    if(capital != 0 && number != 0 && specialCharacter != 0)
    {
        return true;
    }

    displayAccountTemplate("There are either no capital letter, number, or special character.\n");
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
            case 'a': signIn(); break;
            case 'b': signUp(); break;
            case 'c': forgotPassword(); break;
            case 'd': cout << "Thank you"; isRunning = false;
            default: break;
        }
    }
}

void signIn()
{
    string enteredPassword;
    int enteredID;

    displayAccountTemplate("Enter ID Number:\n");
    cin >> enteredID;

    cin.ignore();

    displayAccountTemplate("Enter Password:\n");
    getline(cin, enteredPassword);

    if(enteredID - BASE_ID < logInCredential.size() && enteredID - BASE_ID >= 0)
    {
        if(decrypt(logInCredential[enteredID - BASE_ID].password, enteredID) == enteredPassword)
        {
            isLoggedIn = true;
            loggedInID = logInCredential[enteredID - BASE_ID].id;
        }
        else
        {
            cout << "Wrong password" << endl;
        }
    }
    else
    {
        cout << "No ID Found";
    }
}

void signUp()
{
    string desiredUsername, desiredPassword, role, temprole, tempsection, a;

    displayAccountTemplate("Create a New Account!\n\nEnter your Name: ");
    cin.ignore();
    getline(cin, desiredUsername);
    
    
    while (true)
    {
        displayAccountTemplate("Create a New Account!\n\nPassword should at least have:\n\t1 capital letter\n\t1 number\n\t1 special character\nEnter your password: ");
        getline(cin, desiredPassword);
        if(isPasswordStrong(desiredPassword))
        {
            break;
        }
    }
    
    while (true)
    {
        displayAccountTemplate("Create a New Account!\n\nEnter your role(student, teacher, or admin): ");
        getline(cin, temprole);
        role = "";

        for (int i = 0; i < temprole.size(); i++)
        {
            role += tolower(temprole[i]);
        }
        
        if(role == "student" || role == "teacher" || role == "admin")
        {
            break;
        }
    }

    if (role == "student")
    {
        displayAccountTemplate("Create a New Account\n\nEnter section: ");
        getline(cin, tempsection);
    }
    
    int no = BASE_ID + logInCredential.size();

    User newUser;
    newUser.id = no;
    newUser.username = desiredUsername;
    newUser.role = role;
    if (role == "student")
    {
        newUser.section = tempsection;
    }
    logInCredential.push_back(newUser);
    logInCredential[no - BASE_ID].password = encrypt(desiredPassword, no);

    cout << "    Welcome to Student Grades Management System" << endl;
    cout << "-------------------------------------------------------" << endl;
    cout << "Name: " << logInCredential.back().username << endl;
    cout << "Password: " << decrypt(logInCredential.back().password, logInCredential.back().id) << endl;
    cout << "ID Number: " << logInCredential.back().id << endl;
    if (role == "student")
    {
        cout << "Section: " << logInCredential.back().section << endl;
    }
    cout << "Role: " << logInCredential.back().role << endl;
    cout << "-------------------------------------------------------" << endl;
    cout << ">> ";
    getline(cin, a);
    isLoggedIn = true;
    loggedInID = logInCredential.back().id;
    userIndex = loggedInID - BASE_ID;
}

void forgotPassword()
{
    string user, pass;
    int id;
    bool success = false;
    
    while (!success)
    {
        displayAccountTemplate("Forgot Password?\nEnter Username: ");
        getline(cin, user);

        displayAccountTemplate("Forgot Password?\nEnter ID number: ");
        cin >> id;
        cin.ignore();

        int index = id - BASE_ID;

        if (index >= 0 && index < logInCredential.size() &&
            logInCredential[index].username == user)
        {
            while (true)
            {
                displayAccountTemplate("Forgot Password?\nEnter new password: ");
                getline(cin, pass);

                if (isPasswordStrong(pass))
                {
                    logInCredential[index].password = encrypt(pass, id);
                    cout << "Password updated successfully!" << endl;
                    success = true;
                    break;
                }
            }
        }
        else
        {
            displayAccountTemplate("Forgot Password?\nWrong username or ID number. Try again.\n");
        }
    }
}
