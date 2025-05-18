#include <iostream>
#include "account.h"
#include "data.h"
#include "ED.h"
#include "utils.h"

void displayAccountTemplate(string text)
{
    cout << "=========================================" << endl;
    cout << "    Student Grades Management System" << endl;
    cout << "=========================================" << endl;
    cout << text << endl;
    cout << "-----------------------------------------" << endl;
}

bool isPasswordStrong(string password)
{
    int capital = 0, number = 0, specialCharacter = 0;
    for (int i = 0; i < password.size(); i++)
    {
        if (isupper(password[i]) != 0)
        {
            capital++;
        }
        if (isdigit(password[i]) != 0)
        {
            number++;
        }
        if (!isalnum(password[i]) && !isspace(password[i]))
        {
            specialCharacter++;
        }
    }
    if (capital != 0 && number != 0 && specialCharacter != 0)
    {
        return true;
    }

    displayAccountTemplate("There are either no capital letter, number, or special character.\n");
    get();
    return false;
}

void accountMenu()
{
    c();
    while (isRunning && !isLoggedIn)
    {
        displayAccountTemplate("[A] - Log In\n[B] - Sign Up\n[C] - Forget Password\n[D] - Quit\n");
        char option = getChar();
        c();
        switch (tolower(option))
        {
        case 'a':
            signIn();
            break;
        case 'b':
            signUp();
            break;
        case 'c':
            forgotPassword();
            break;
        case 'd':
            cout << "Thank you for using our SMS.";
            isRunning = false;
        default:
            break;
        }
    }
}

void signIn()
{
    if (logInCredential.size() > 0)
    {
        string enteredPassword;

        displayAccountTemplate("Enter ID Number:\n");
        int enteredID = getInt();
        c();
        displayAccountTemplate("Enter Password:\n");
        cout << ">> ";
        getline(cin, enteredPassword);
        if (enteredID - BASE_ID < logInCredential.size() && enteredID - BASE_ID >= 0)
        {
            if (decrypt(logInCredential[enteredID - BASE_ID].password, enteredID) == enteredPassword)
            {
                isLoggedIn = true;
                loggedInID = logInCredential[enteredID - BASE_ID].id;
                userIndex = loggedInID - BASE_ID;
            }
            else
            {
                cout << "Wrong password" << endl;
                get();
            }
        }
        else
        {
            cout << "No ID Found" << endl;
            get();
        }
    }
    if (logInCredential.size() == 0)
    {
        cout << "Sorry, there's no registered user. Please register first." << endl;
        get();
    }
}

void signUp()
{
    string desiredUsername, desiredPassword, role, temprole, tempsection;

    while (true)
    {
        displayAccountTemplate("Create a New Account!\n\nEnter your Name: ");
        cout << ">> ";
        getline(cin, desiredUsername);
        if (desiredUsername.empty())
        {
            cout << "Please enter your name.[Press enter]" << endl;
            get();
        }
        else
        {
            break;
        } 
    }
    
    while (true)
    {
        c();
        displayAccountTemplate("Create a New Account!\n\nPassword should at least have:\n\t1 capital letter\n\t1 number\n\t1 special character\nEnter your password: ");
        cout << ">> ";
        getline(cin, desiredPassword);
        if (isPasswordStrong(desiredPassword))
        {
            break;
        }
    }

    while (true)
    {
        c();
        displayAccountTemplate("Create a New Account!\n\nEnter your role(student, teacher, or admin): ");
        cout << ">> ";
        getline(cin, temprole);
        role = "";

        for (int i = 0; i < temprole.size(); i++)
        {
            role += tolower(temprole[i]);
        }

        if (role == "student" || role == "teacher" || role == "admin")
        {
            break;
        }
    }

    if (role == "student")
    {
        c();
        displayAccountTemplate("Create a New Account\n\nEnter Program[ex. BSCS]: \n");
        cout << ">> ";
        getline(cin, tempsection);
        c();
        displayAccountTemplate("Create a New Account\n\nEnter section: ");
        cout << ">> ";
        getline(cin, tempsection);
    }


    int no = BASE_ID + logInCredential.size();

    c();
    cout << "=========================================" << endl;
    cout << "    Student Grades Management System" << endl;
    cout << "=========================================" << endl;
    cout << "Name: " << desiredUsername << endl;
    cout << "Password: " << desiredPassword << endl;
    cout << "ID Number: " << no << endl;
    if (role == "student")
    {
        cout << "Section: " << tempsection << endl;
    }
    cout << "Role: " << role << endl;
    cout << "Confirm by pressing 'y'" << endl;
    cout << "-----------------------------------------" << endl;
    cout << ">> ";
    char confirm = getChar();

    if (tolower(confirm) == 'y')
    {
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

        for (int i = 0; i < courses.size(); i++)
        {
            if (logInCredential[courses[i].enrolledStudentID[0] - BASE_ID].section == tempsection)
            {
                logInCredential.back().coursesEnrolled.push_back(courses[i].courseID);
                courses[i].enrolledStudentID.push_back(logInCredential.back().id);
            }
        }
        isLoggedIn = true;
        loggedInID = logInCredential.back().id;
        userIndex = loggedInID - BASE_ID;
        cout << "You have successfully created a new account!" << endl;
        get();
    }
    else
    {
        cout << "Cancelled creating an account.\nGoing back to menu....." << endl;
        get();
    }
}

void forgotPassword()
{
    string user, pass;
    int id;
    bool success = false;

    while (!success)
    {
        displayAccountTemplate("Forgot Password?\nEnter Username: ");
        cout << ">> ";
        getline(cin, user);
        c();
        displayAccountTemplate("Forgot Password?\nEnter ID number: ");
        id = getInt();
        c();
        int index = id - BASE_ID;

        if (index >= 0 && index < logInCredential.size() &&
            logInCredential[index].username == user)
        {
            while (true)
            {
                displayAccountTemplate("Forgot Password?\nEnter new password: ");
                cout << ">> ";
                getline(cin, pass);
                c();
                if (isPasswordStrong(pass))
                {
                    logInCredential[index].password = encrypt(pass, id);
                    cout << "Password updated successfully!" << endl;
                    get();
                    success = true;
                    break;
                }
            }
        }
        else
        {
            displayAccountTemplate("Forgot Password?\nWrong username or ID number. Try again.\n");
            get();
            break;
        }
    }
}
