#include <iostream>
#include "account.h"
#include "data.h"
#include "ED.h"
#include "utils.h"

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
    return false;
}

void accountMenu()
{
    c();
    while (isRunning && !isLoggedIn)
    {
        displayTemplate("[A] - Log In\n[B] - Sign Up\n[C] - Forget Password\n[D] - Quit");
        cout << "Enter your choice: ";
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

        displayTemplate("                Log In");
        cout << "Enter ID Number: ";
        int enteredID = getInt();
        cout << "Enter Password: ";
        getline(cin, enteredPassword);

        int index = indexFind(enteredID);
        if (index == -1)
        {
            cout << "-----------------------------------------" << endl;
            cout << "Error: No ID Found" << endl;
            get();
            return;
        }
        if (decrypt(logInCredential[index].password, enteredID, index) == enteredPassword)
        {
            isLoggedIn = true;
            loggedInID = logInCredential[index].id;
            userIndex = indexFind(loggedInID);
        }
        else
        {
            cout << "-----------------------------------------" << endl;
            cout << "Error: Wrong password" << endl;
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
    c();
    string desiredUsername, desiredPassword, role, temprole, tempsection, tempprogram;

    displayTemplate("          Create a New Account");
    cout << "Enter your Name: ";
    getline(cin, desiredUsername);
    if (desiredUsername.empty())
    {
        cout << "-----------------------------------------" << endl;
        cout << "Error: Name cannot be empty." << endl;
        get();
        return;
    }

    cout << "Password should at least have:\n\t1 capital letter\n\t1 number\n\t1 special character\nEnter your password: ";
    getline(cin, desiredPassword);
    if (!isPasswordStrong(desiredPassword))
    {
        cout << "-----------------------------------------" << endl;
        cout << "Error: There are either no capital letter, number, or special character.\n";
        get();
        return;
    }

    cout << "Enter your role(student or teacher): ";
    getline(cin, temprole);

    for (int i = 0; i < temprole.size(); i++)
    {
        role += tolower(temprole[i]);
    }

    if (role != "student" && role != "teacher")
    {
        cout << "-----------------------------------------" << endl;
        cout << "Error: Invalid role" << endl;
        get();
        return;
    }

    if (role == "student")
    {
        cout << "Enter Program[ex. BSCS]: ";
        getline(cin, tempprogram);

        cout << "Enter section: ";
        getline(cin, tempsection);
        for (int i = 0; i < tempsection.size(); i++)
        {
            if (isdigit(tempsection[i]))
            {
                break;
            }
            else
            {
                tempsection[i] = toupper(tempsection[i]);
            }
        }
    }

    int no;
    int index;
    if (logInCredential.empty())
    {
        no = BASE_ID + 1;
    }
    else
    {
        no = logInCredential.back().id + 1;
    }
    
    c();
    cout << "=========================================" << endl;
    cout << "    Student Grades Management System" << endl;
    cout << "=========================================" << endl;
    cout << "Name: " << desiredUsername << endl;
    cout << "ID Number: " << no << endl;
    if (role == "student")
    {
        cout << "Program: " << tempprogram << endl;
        cout << "Section: " << tempsection << endl;
    }
    cout << "Password: " << desiredPassword << endl;
    cout << "Role: " << role << endl;
    cout << "Confirm by pressing 'Y/y'" << endl;
    cout << "-----------------------------------------" << endl;
    cout << "Enter choice: ";
    char confirm = getChar();

    if (tolower(confirm) == 'y')
    {
        User newUser;
        newUser.id = no;
        newUser.username = desiredUsername;
        newUser.role = role;
        if (role == "student")
        {
            newUser.program = tempprogram;
            newUser.section = tempsection;
        }
        logInCredential.push_back(newUser);
        index = indexFind(no);
        logInCredential[index].password = encrypt(desiredPassword, no, index);
        for (int i = 0; i < courses.size(); i++)
        {
            index = indexFind(courses[i].enrolledStudentID[0]);
            if (logInCredential[index].section == tempsection)
            {
                GradeRecord records;
                records.studentID = no;
                records.performanceTaskName = courses[i].studentRecords[0].performanceTaskName;
                records.performanceTaskOver = courses[i].studentRecords[0].performanceTaskOver;
                records.writtenTaskName = courses[i].studentRecords[0].writtenTaskName;
                records.writtenTaskOver = courses[i].studentRecords[0].writtenTaskOver;
                records.majorExamName = courses[i].studentRecords[0].majorExamName;
                records.majorExamOver = courses[i].studentRecords[0].majorExamOver;
                int size = courses[i].studentRecords[0].performanceTask.size();
                for (int j = 0; j < size; j++)
                {
                    records.performanceTask.push_back(0.0);
                }
                size = courses[i].studentRecords[0].writtenTask.size();
                for (int j = 0; j < size; j++)
                {
                    records.writtenTask.push_back(0.0);
                }
                size = courses[i].studentRecords[0].majorExam.size();
                for (int j = 0; j < size; j++)
                {
                    records.majorExam.push_back(0.0);
                }
                courses[i].studentRecords.push_back(records);
                logInCredential.back().coursesEnrolled.push_back(courses[i].courseID);
                courses[i].enrolledStudentID.push_back(logInCredential.back().id);
                string notif = "You are now enrolled in " + courses[i].courseName;
                logInCredential.back().notifications.push_back(notif);
                if (logInCredential.back().role == "student")
                {
                    index = indexFind(courses[i].teacherID);
                    notif = logInCredential.back().username + " joined " + courses[i].courseName + " class.";
                    logInCredential[index].notifications.push_back(notif);
                }
            }
        }
        isLoggedIn = true;
        loggedInID = logInCredential.back().id;
        userIndex = indexFind(loggedInID);
        cout << "-----------------------------------------" << endl;
        cout << "You have successfully created a new account!" << endl;
        get();
    }
    else
    {
        cout << "-----------------------------------------" << endl;
        cout << "Cancelled creating an account.\nGoing back to menu....." << endl;
        get();
    }
}

void forgotPassword()
{
    string user, pass;
    int id;
    displayTemplate("       Forgot Password?");
    cout << "Enter Username: ";
    getline(cin, user);
    cout << "Enter ID number: ";
    id = getInt();
    int index = indexFind(id);

    if (index >= 0 && index < logInCredential.size() && logInCredential[index].username == user)
    {
        while (true)
        {
            cout << "Enter new password: ";
            getline(cin, pass);
            if (isPasswordStrong(pass))
            {
                logInCredential[index].password = encrypt(pass, id, index);
                cout << "Password updated successfully!" << endl;
                get();
                string notif = "You changed your password.";
                logInCredential[index].notifications.push_back(notif);
                break;
            }
            else
            {
                cout << "-----------------------------------------" << endl;
                cout << "Error: There are either no capital letter, number, or special character.\n";
                get();
                return;
            }
        }
    }
    else
    {
        cout << "-----------------------------------------" << endl;
        cout << "Wrong username or ID number. Try again." << endl;
        get();
    }
}
