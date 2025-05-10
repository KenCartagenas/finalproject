#include "mainmenu.h"
#include "calculate.h"
#include <iostream>
#include <string>
#include <iomanip>
#include <fstream>
#include <algorithm>
#include <cmath>
#include "data.h"
using namespace std;

void displayMainMenuTemplate(string text)
{

    cout << "    Welcome " << logInCredential[userIndex].username << endl;
    cout << "-------------------------------------------------------" << endl;
    cout << text << endl;
    cout << "-------------------------------------------------------" << endl;
    cout << ">> ";
}

void mainMenu()
{
  if(logInCredential[userIndex].role == "teacher")
  {
    teacherMenu();
  }
  else if(logInCredential[userIndex].role == "student")
  {
    studentMenu();
  }
}

void teacherMenu()
{
  while(isLoggedIn)
  {
    char option;
    displayMainMenuTemplate(" [A]. Create New Course / Subject\n[B]. Open Existing Class\n[C]. Search Student\n[D]. Settings\n[E]. Log Out");
    cin >> option;

    switch(tolower(option))
    {
      case 'a': createCourse(); break;
      case 'b': openClass(); break;
      case 'c': searchStudentSummary(); break;
      case 'd': editUserProfile(); break;
      case 'e': isLoggedIn = false;
    }
  }
  
}

void studentMenu()
{
  while(isLoggedIn)
  {
    char option;
    displayMainMenuTemplate("[A]. Open Course\n[B]. Log Out");
    cin >> option;

    switch(tolower(option))
    {
      //opem course
      case 'a': break;
      //log out
      case 'b': isLoggedIn = false;
    }
  }
}

void createCourse()
{
    string tempName, tempSection;
    char option, confirm;
    float tempBase, tempP, tempW, tempM;

    cin.ignore(); 

    displayMainMenuTemplate("Create New Course:\nEnter course's name:");
    getline(cin, tempName);

    displayMainMenuTemplate("Create New Course:\nDo you want to use the default base of grade (zero)?");
    cin >> option;
    if (tolower(option) == 'y') 
    {
        tempBase = 0;
    } 
    else 
    {
        displayMainMenuTemplate("Create New Course:\nEnter base of grade: ");
        cin >> tempBase;
    }

    cin.ignore();  
    displayMainMenuTemplate("Create New Course:\nEnter students' section: ");
    getline(cin, tempSection);  

    bool validPercentages = false;
    while (!validPercentages) 
    {
        displayMainMenuTemplate("Create Course:\nEnter percent for Performance Task, Written Task, and Major Examination (e.g. 40 20 20): ");
        cin >> tempP >> tempW >> tempM;

        float totalPercent = tempP + tempW + tempM;
        if (totalPercent == 100.0f) 
        {
            validPercentages = true;
        } 
        else 
        {
            cout << "Error: Percentages must add up to 100%. You entered " << totalPercent << "%." << endl;
        }
    }

    Course newCourse;
    newCourse.courseID = coursesCount;
    newCourse.courseName = tempName;
    newCourse.teacher = logInCredential[userIndex].username;
    newCourse.teacherID = logInCredential[userIndex].id;
    newCourse.base = tempBase;
    newCourse.performanceTaskPercentage = tempP;
    newCourse.writtenTaskPercentage = tempW;
    newCourse.majorExamPercentage = tempM;

    for (int i = 0; i < logInCredential.size(); i++) 
    {
        if (logInCredential[i].section == tempSection) 
        {
            newCourse.enrolledStudentID.push_back(logInCredential[i].id);

            GradeRecord newRecord;
            newRecord.studentID = logInCredential[i].id;
            newCourse.studentRecords.push_back(newRecord);
        }
    }

    logInCredential[userIndex].coursesHandled.push_back(newCourse.courseID);
    coursesCount++;

    cout << "    Welcome " << logInCredential[userIndex].username << endl;
    cout << "-------------------------------------------------------" << endl;
    cout << "Course Name: " << newCourse.courseName << endl;
    cout << "Course ID: " << newCourse.courseID << endl;
    cout << "Teacher: " << newCourse.teacher << endl;
    cout << "Enter y to confirm: " << endl;
    cout << "-------------------------------------------------------" << endl;
    cout << ">> ";
    cin >> confirm;

    if (tolower(confirm) == 'y') 
    {
        cout << "You have successfully created a new course." << endl;
        courses.push_back(newCourse);
    } 
    else 
    {
        cout << "Failed to create new course. Returning to the menu." << endl;
    }
}
  
void openClass()
{
    if (logInCredential[userIndex].coursesHandled.size() > 0)
    {
        int chosenCourse, optionOpen;

        cout << "    Welcome " << logInCredential[userIndex].username << endl;
        cout << "-------------------------------------------------------" << endl;
        cout << "Courses Handled: " << endl;

        for (int i = 0; i < logInCredential[userIndex].coursesHandled.size(); i++)
        {
            cout << i + 1 << ".) " << courses[logInCredential[userIndex].coursesHandled[i]].courseName << endl;
        }
        cout << "-------------------------------------------------------" << endl;
        cout << ">> ";
        cin >> chosenCourse;

        if (chosenCourse < 1 || chosenCourse > logInCredential[userIndex].coursesHandled.size()) 
        {
            cout << "Invalid course selection. Returning to main menu." << endl;
            return; 
        }

        int indexOfCourse = courses[logInCredential[userIndex].coursesHandled[chosenCourse - 1]].courseID;
        cout << "    " << courses[indexOfCourse].courseName << endl;
        cout << "-------------------------------------------------------" << endl;
        cout << "[A] Display All Students and Scores" << endl;
        cout << "[B] Edit Student Scores" << endl;
        cout << "[C] Generate Class Grade Report" << endl;
        cout << "[D] Add or Remove Students" << endl;
        cout << "[E] Customize Grade Settings" << endl;
        cout << "[F] Return to Main Dashboard" << endl;
        cout << "-------------------------------------------------------" << endl;
        cout << ">> ";
        cin >> optionOpen;

        optionOpen = tolower(optionOpen);

        if (optionOpen < 'a' || optionOpen > 'f') 
        {
            cout << "Invalid option. Please try again." << endl;
            return;
        }

        switch (optionOpen)
        {
          case 'a':
              displayStudents(indexOfCourse);
              break;
          case 'b':
              editStudent(indexOfCourse);
              break;
          case 'c':
              generateReport(indexOfCourse);
              break;
          case 'd':
              addOrRemove(indexOfCourse);
              break;
          case 'e':
              customizeGradeSettings(indexOfCourse);
              break;
          case 'f':
              return;
          default:
              break;
        }
    }
    else
    {
        cout << "    Welcome " << logInCredential[userIndex].username << endl;
        cout << "-------------------------------------------------------" << endl;
        cout << "You are not currently handling any courses." << endl;
        cout << "Create courses first." << endl;
        cout << "-------------------------------------------------------" << endl;
        system("pause");
    }
}

void displayStudents(int indexOfCourse)
{
  cout << "-------------------------------------------------------" << endl;
  if (courses[indexOfCourse].enrolledStudentID.size() > 0)
  {
    for (int j = 0; j < courses[indexOfCourse].enrolledStudentID.size(); j++)
    {
      cout << j + 1 << ".) " << logInCredential[courses[indexOfCourse].enrolledStudentID[j] - BASE_ID].username << endl;
      cout << "\tPerformace Task:" << endl;
      if (courses[indexOfCourse].studentRecords[j].performanceTask.size() > 0)
      {
        for (int k = 0; k < courses[indexOfCourse].studentRecords[j].performanceTask.size(); k++)
        {
          cout << "\t\t* " << courses[indexOfCourse].studentRecords[j].performanceTaskName[k] << " - " << courses[indexOfCourse].studentRecords[j].performanceTask[k] << "/" << courses[indexOfCourse].studentRecords[j].performanceTaskName[k] << endl;
        }
        courses[indexOfCourse].studentRecords[j].performanceTaskPercent = calculateScore(indexOfCourse, courses[indexOfCourse].studentRecords[j], 'a')/calculateScore(indexOfCourse, courses[indexOfCourse].studentRecords[j], 'c');
        cout << "==============================" << endl;
        cout << setprecision(2) << fixed << "\t\t\tTotal Score: " << calculateScore(indexOfCourse, courses[indexOfCourse].studentRecords[j], 'a') << "/" << calculateScore(indexOfCourse, courses[indexOfCourse].studentRecords[j], 'c') << endl;
        cout << setprecision(0) << fixed << "\t\t\tTotal Ratio: " << courses[indexOfCourse].studentRecords[j].performanceTaskPercent * 100 << "%" << endl;
      }
      else
      {
        cout << "\t\t-------NO RECORD/s FOUND-------" << endl;
      }
      
      cout << "\tWritten Task:" << endl;

      if (courses[indexOfCourse].studentRecords[j].writtenTask.size() > 0)
      {
        for (int k = 0; k < courses[indexOfCourse].studentRecords[j].writtenTask.size(); k++)
        {
          cout << "\t\t* " << courses[indexOfCourse].studentRecords[j].writtenTaskName[k] << " - " << courses[indexOfCourse].studentRecords[j].writtenTask[k] << endl;
        }
        courses[indexOfCourse].studentRecords[j].writtenTaskPercent = calculateScore(indexOfCourse, courses[indexOfCourse].studentRecords[j], 'b')/calculateScore(indexOfCourse, courses[indexOfCourse].studentRecords[j], 'd');
        cout << "==============================" << endl;
        cout << setprecision(2) << fixed << "\t\t\tTotal Score: " << calculateScore(indexOfCourse, courses[indexOfCourse].studentRecords[j], 'b') << "/" << calculateScore(indexOfCourse, courses[indexOfCourse].studentRecords[j], 'd') << endl;
        cout << setprecision(0) << fixed << "\t\t\tTotal Ratio: " << courses[indexOfCourse].studentRecords[j].writtenTaskPercent * 100 << "%" << endl;
      }
      else
      {
        cout << "\t\t-------NO RECORD/s FOUND-------" << endl;
      }
      
      cout << "\tMajor Exam: " << endl;

      if (courses[indexOfCourse].studentRecords[j].majorExam.size() > 0)
      {
        for (int k = 0; k < courses[indexOfCourse].studentRecords[j].majorExam.size(); k++)
        {
          cout << "\t\t* " << courses[indexOfCourse].studentRecords[j].majorExamName[k] << " - " << courses[indexOfCourse].studentRecords[j].majorExam[k] << endl;
        }
        courses[indexOfCourse].studentRecords[j].writtenTaskPercent = courses[indexOfCourse].studentRecords[j].majorExam[0]/courses[indexOfCourse].studentRecords[j].majorExamOver[0];
        cout << "==============================" << endl;
        cout << "\t\t\tTotal Score: " << courses[indexOfCourse].studentRecords[j].majorExam[0] << "/" << courses[indexOfCourse].studentRecords[j].majorExamOver[0] << endl;
        cout << "\t\t\tTotal Ratio: " << courses[indexOfCourse].studentRecords[j].majorExam[0]/courses[indexOfCourse].studentRecords[j].majorExamOver[0] << endl;
      }
      else
      {
        cout << "\t\t-------NO RECORD/s FOUND-------" << endl;
      }
      cout << endl;
    }
  }
  else
  {
    cout << "-------NO STUDENT IS ENROLLED-------" << endl;
  }
  cout << "-------------------------------------------------------" << endl;
  system("pause");
}

void editStudent(int indexOfCourse)
{
  while (true)
  {
    char option;
    cout << "-------------------------------------------------------" << endl;
    cout << "[A] - Add Scores to everyone" << endl;
    cout << "[B] - Add Score to a student" << endl;
    cout << "[C] - Add additional points to a student" << endl;
    cout << "[D] - Edit score to a student" << endl;
    cout << "[E] - Go back" << endl;
    cout << "-------------------------------------------------------" << endl;
    cout << ">> ";
    cin >> option;

    switch (tolower(option))
    {
    case 'a':
      addScoresToEveryone(indexOfCourse);
      break;
    case 'b':
      addScoreToStudent(indexOfCourse);
      break;
    case 'c':
      addAdditionalScoreToStudent(indexOfCourse);
    case 'd':
      editScore(indexOfCourse);
      break;
    default:
      break;
    }

    if (tolower(option) == 'e')
    {
      break;
    }
  }
  

}

void addScoresToEveryone(int indexOfCourse)
{
  char option;
  cout << "-------------------------------------------------------" << endl;
  cout << "Add scores to? " << endl;
  cout << "[A] - Performance Task" << endl;
  cout << "[B] - Written Task" << endl;
  cout << "[C] - Major Exam" << endl;
  cout << "-------------------------------------------------------" << endl;
  cout << ">> ";
  cin >> option;
  cin.ignore();

  if (tolower(option) == 'a')
  {
    string actName;
    float score, over;

    cout << "Activity name: ";
    getline(cin, actName);
    cout << "Over: ";
    cin >> over;
    for (int i = 0; i < courses[indexOfCourse].enrolledStudentID.size(); i++)
    {
      cout << i + 1 << ".) " << logInCredential[courses[indexOfCourse].enrolledStudentID[i] - BASE_ID].username << endl;
      cout << "\tPerformace Task:" << endl;
      if (courses[indexOfCourse].studentRecords[i].performanceTask.size() > 0)
      {
        for (int j = 0; j < courses[indexOfCourse].studentRecords[i].performanceTask.size(); j++)
        {
          cout << "\t\t* " << courses[indexOfCourse].studentRecords[i].performanceTaskName[j] << " - " << courses[indexOfCourse].studentRecords[i].performanceTask[j] << endl;
        }
      }
      else
      {
        cout << "\t\t-------NO RECORD/s FOUND-------" << endl;
      }
      cout << "Add score to " << actName << ": " << endl;
      cout << "-------------------------------------------------------" << endl;
      cin >> score;
      courses[indexOfCourse].studentRecords[i].performanceTaskName.push_back(actName);
      courses[indexOfCourse].studentRecords[i].performanceTask.push_back(score);
      courses[indexOfCourse].studentRecords[i].performanceTaskOver.push_back(over);
    }
  }
  else if(tolower(option) == 'b')
  {
    string actName;
    float score, over;

    cout << "Activity name: ";
    getline(cin, actName);
    cout << "Over: ";
    cin >> over;
    for (int i = 0; i < courses[indexOfCourse].enrolledStudentID.size(); i++)
    {
      cout << i + 1 << ".) " << logInCredential[courses[indexOfCourse].enrolledStudentID[i] - BASE_ID].username << endl;
      cout << "\tPerformace Task:" << endl;
      if (courses[indexOfCourse].studentRecords[i].writtenTask.size() > 0)
      {
        for (int j = 0; j < courses[indexOfCourse].studentRecords[i].writtenTask.size(); j++)
        {
          cout << "\t\t* " << courses[indexOfCourse].studentRecords[i].writtenTaskName[j] << " - " << courses[indexOfCourse].studentRecords[i].writtenTask[j] << endl;
        }
      }
      else
      {
        cout << "\t\t-------NO RECORD/s FOUND-------" << endl;
      }
      cout << "Add score to " << actName << ": " << endl;
      cout << "-------------------------------------------------------" << endl;
      cin >> score;
      courses[indexOfCourse].studentRecords[i].writtenTaskName.push_back(actName);
      courses[indexOfCourse].studentRecords[i].writtenTask.push_back(score);
      courses[indexOfCourse].studentRecords[i].writtenTaskOver.push_back(over);
    }
  }
  else if(tolower(option) == 'c')
  {
    string actName;
    float score, over;

    cout << "Activity name: ";
    getline(cin, actName);
    cout << "Over: ";
    cin >> over;
    for (int i = 0; i < courses[indexOfCourse].enrolledStudentID.size(); i++)
    {
      cout << i + 1 << ".) " << logInCredential[courses[indexOfCourse].enrolledStudentID[i] - BASE_ID].username << endl;
      cout << "\tPerformace Task:" << endl;
      if (courses[indexOfCourse].studentRecords[i].majorExam.size() > 0)
      {
        for (int j = 0; j < courses[indexOfCourse].studentRecords[i].majorExam.size(); j++)
        {
          cout << "\t\t* " << courses[indexOfCourse].studentRecords[i].majorExamName[j] << " - " << courses[indexOfCourse].studentRecords[i].majorExam[j] << endl;
        }
      }
      else
      {
        cout << "\t\t-------NO RECORD/s FOUND-------" << endl;
      }
      cout << "Add score to " << actName << ": " << endl;
      cout << "-------------------------------------------------------" << endl;
      cin >> score;
      courses[indexOfCourse].studentRecords[i].majorExamName.push_back(actName);
      courses[indexOfCourse].studentRecords[i].majorExam.push_back(score);
      courses[indexOfCourse].studentRecords[i].majorExamOver.push_back(over);
    }
  }
}

void addScoreToStudent(int indexOfCourse)
{
  int chosenStudent;
  for (int i = 0; i < courses[indexOfCourse].enrolledStudentID.size(); i++)
  {
    cout << i + 1 << ".) " << logInCredential[courses[indexOfCourse].enrolledStudentID[i] - BASE_ID].username << endl;
  }
  cout << "Choose student: ";
  cin >> chosenStudent;

  char option;
  cout << "-------------------------------------------------------" << endl;
  cout << "Add scores to? " << endl;
  cout << "[A] - Performance Task" << endl;
  cout << "[B] - Written Task" << endl;
  cout << "[C] - Major Exam" << endl;
  cout << "-------------------------------------------------------" << endl;
  cout << ">> ";
  cin >> option;
  cin.ignore();

  string actName;
  float score, over;
  if (tolower(option) == 'a')
  {
    cout << "Activity name: ";
    getline(cin, actName);
    cout << "Over: ";
    cin >> over;
      cout << chosenStudent << ".) " << logInCredential[courses[indexOfCourse].enrolledStudentID[chosenStudent - 1] - BASE_ID].username << endl;
      cout << "\tPerformace Task:" << endl;
      if (courses[indexOfCourse].studentRecords[chosenStudent - 1].performanceTask.size() > 0)
      {
        for (int j = 0; j < courses[indexOfCourse].studentRecords[chosenStudent - 1].performanceTask.size(); j++)
        {
          cout << "\t\t* " << courses[indexOfCourse].studentRecords[chosenStudent - 1].performanceTaskName[j] << " - " << courses[indexOfCourse].studentRecords[chosenStudent - 1].performanceTask[j] << endl;
        }
      }
      else
      {
        cout << "\t\t-------NO RECORD/s FOUND-------" << endl;
      }
      cout << "Add score to " << actName << ": " << endl;
      cout << "-------------------------------------------------------" << endl;
      cin >> score;
      courses[indexOfCourse].studentRecords[chosenStudent - 1].performanceTaskName.push_back(actName);
      courses[indexOfCourse].studentRecords[chosenStudent - 1].performanceTask.push_back(score);
      courses[indexOfCourse].studentRecords[chosenStudent - 1].performanceTaskOver.push_back(over);
  }
  if (tolower(option) == 'b')
  {
    cout << "Activity name: ";
    getline(cin, actName);
    cout << "Over: ";
    cin >> over;
      cout << chosenStudent << ".) " << logInCredential[courses[indexOfCourse].enrolledStudentID[chosenStudent - 1] - BASE_ID].username << endl;
      cout << "\tPerformace Task:" << endl;
      if (courses[indexOfCourse].studentRecords[chosenStudent - 1].writtenTask.size() > 0)
      {
        for (int j = 0; j < courses[indexOfCourse].studentRecords[chosenStudent - 1].writtenTask.size(); j++)
        {
          cout << "\t\t* " << courses[indexOfCourse].studentRecords[chosenStudent - 1].writtenTaskName[j] << " - " << courses[indexOfCourse].studentRecords[chosenStudent - 1].writtenTask[j] << endl;
        }
      }
      else
      {
        cout << "\t\t-------NO RECORD/s FOUND-------" << endl;
      }
      cout << "Add score to " << actName << ": " << endl;
      cout << "-------------------------------------------------------" << endl;
      cin >> score;
      courses[indexOfCourse].studentRecords[chosenStudent - 1].writtenTaskName.push_back(actName);
      courses[indexOfCourse].studentRecords[chosenStudent - 1].writtenTask.push_back(score);
      courses[indexOfCourse].studentRecords[chosenStudent - 1].writtenTaskOver.push_back(over);
  }
  if (tolower(option) == 'c')
  {
    cout << "Activity name: ";
    getline(cin, actName);
    cout << "Over: ";
    cin >> over;
      cout << chosenStudent << ".) " << logInCredential[courses[indexOfCourse].enrolledStudentID[chosenStudent - 1] - BASE_ID].username << endl;
      cout << "\tPerformace Task:" << endl;
      if (courses[indexOfCourse].studentRecords[chosenStudent - 1].majorExam.size() > 0)
      {
        for (int j = 0; j < courses[indexOfCourse].studentRecords[chosenStudent - 1].majorExam.size(); j++)
        {
          cout << "\t\t* " << courses[indexOfCourse].studentRecords[chosenStudent - 1].majorExamName[j] << " - " << courses[indexOfCourse].studentRecords[chosenStudent - 1].majorExam[j] << endl;
        }
      }
      else
      {
        cout << "\t\t-------NO RECORD/s FOUND-------" << endl;
      }
      cout << "Add score to " << actName << ": " << endl;
      cout << "-------------------------------------------------------" << endl;
      cin >> score;
      courses[indexOfCourse].studentRecords[chosenStudent - 1].majorExamName.push_back(actName);
      courses[indexOfCourse].studentRecords[chosenStudent - 1].majorExam.push_back(score);
      courses[indexOfCourse].studentRecords[chosenStudent - 1].majorExamOver.push_back(over);
  }
}

void addAdditionalScoreToStudent(int indexOfCourse)
{
  int chosenStudent;
  for (int i = 0; i < courses[indexOfCourse].enrolledStudentID.size(); i++)
  {
    cout << i + 1 << ".) " << logInCredential[courses[indexOfCourse].enrolledStudentID[i] - BASE_ID].username << endl;
  }
  cout << "Choose student: ";
  cin >> chosenStudent;

  char option;
  cout << "-------------------------------------------------------" << endl;
  cout << "Add additional points to? " << endl;
  cout << "[A] - Performance Task" << endl;
  cout << "[B] - Written Task" << endl;
  cout << "[C] - Major Exam" << endl;
  cout << "-------------------------------------------------------" << endl;
  cout << ">> ";
  cin >> option;
  cin.ignore();

  string actName;
  float score;
  if (tolower(option) == 'a')
  {
    cout << "Activity name: ";
    getline(cin, actName);
      cout << chosenStudent << ".) " << logInCredential[courses[indexOfCourse].enrolledStudentID[chosenStudent - 1] - BASE_ID].username << endl;
      cout << "\tPerformace Task:" << endl;
      if (courses[indexOfCourse].studentRecords[chosenStudent - 1].performanceTask.size() > 0)
      {
        for (int j = 0; j < courses[indexOfCourse].studentRecords[chosenStudent - 1].performanceTask.size(); j++)
        {
          cout << "\t\t* " << courses[indexOfCourse].studentRecords[chosenStudent - 1].performanceTaskName[j] << " - " << courses[indexOfCourse].studentRecords[chosenStudent - 1].performanceTask[j] << endl;
        }
      }
      else
      {
        cout << "\t\t-------NO RECORD/s FOUND-------" << endl;
      }
      cout << "Add score to " << actName << ": " << endl;
      cout << "-------------------------------------------------------" << endl;
      cin >> score;
      courses[indexOfCourse].studentRecords[chosenStudent - 1].performanceTaskName.push_back(actName);
      courses[indexOfCourse].studentRecords[chosenStudent - 1].performanceTask.push_back(score);
      courses[indexOfCourse].studentRecords[chosenStudent - 1].performanceTaskOver.push_back(score);
  }
  if (tolower(option) == 'b')
  {
    cout << "Activity name: ";
    getline(cin, actName);
      cout << chosenStudent << ".) " << logInCredential[courses[indexOfCourse].enrolledStudentID[chosenStudent - 1] - BASE_ID].username << endl;
      cout << "\tPerformace Task:" << endl;
      if (courses[indexOfCourse].studentRecords[chosenStudent - 1].writtenTask.size() > 0)
      {
        for (int j = 0; j < courses[indexOfCourse].studentRecords[chosenStudent - 1].writtenTask.size(); j++)
        {
          cout << "\t\t* " << courses[indexOfCourse].studentRecords[chosenStudent - 1].writtenTaskName[j] << " - " << courses[indexOfCourse].studentRecords[chosenStudent - 1].writtenTask[j] << endl;
        }
      }
      else
      {
        cout << "\t\t-------NO RECORD/s FOUND-------" << endl;
      }
      cout << "Add score to " << actName << ": " << endl;
      cout << "-------------------------------------------------------" << endl;
      cin >> score;
      courses[indexOfCourse].studentRecords[chosenStudent - 1].writtenTaskName.push_back(actName);
      courses[indexOfCourse].studentRecords[chosenStudent - 1].writtenTask.push_back(score);
      courses[indexOfCourse].studentRecords[chosenStudent - 1].writtenTaskOver.push_back(score);
  }
  if (tolower(option) == 'c')
  {
    cout << "Activity name: ";
    getline(cin, actName);
      cout << chosenStudent << ".) " << logInCredential[courses[indexOfCourse].enrolledStudentID[chosenStudent - 1] - BASE_ID].username << endl;
      cout << "\tPerformace Task:" << endl;
      if (courses[indexOfCourse].studentRecords[chosenStudent - 1].majorExam.size() > 0)
      {
        for (int j = 0; j < courses[indexOfCourse].studentRecords[chosenStudent - 1].majorExam.size(); j++)
        {
          cout << "\t\t* " << courses[indexOfCourse].studentRecords[chosenStudent - 1].majorExamName[j] << " - " << courses[indexOfCourse].studentRecords[chosenStudent - 1].majorExam[j] << endl;
        }
      }
      else
      {
        cout << "\t\t-------NO RECORD/s FOUND-------" << endl;
      }
      cout << "Add score to " << actName << ": " << endl;
      cout << "-------------------------------------------------------" << endl;
      cin >> score;
      courses[indexOfCourse].studentRecords[chosenStudent - 1].majorExamName.push_back(actName);
      courses[indexOfCourse].studentRecords[chosenStudent - 1].majorExam.push_back(score);
      courses[indexOfCourse].studentRecords[chosenStudent - 1].majorExamOver.push_back(score);
  }
}

void editScore(int indexOfCourse)
{
  int chosenStudent;
  for (int i = 0; i < courses[indexOfCourse].enrolledStudentID.size(); i++)
  {
    cout << i + 1 << ".) " << logInCredential[courses[indexOfCourse].enrolledStudentID[i] - BASE_ID].username << endl;
  }
  cout << "Choose student: ";
  cin >> chosenStudent;

  char option;
  cout << "-------------------------------------------------------" << endl;
  cout << "Add additional points to? " << endl;
  cout << "[A] - Performance Task" << endl;
  cout << "[B] - Written Task" << endl;
  cout << "[C] - Major Exam" << endl;
  cout << "-------------------------------------------------------" << endl;
  cout << ">> ";
  cin >> option;
  cin.ignore();

  string actName;
  float score;
  if (tolower(option) == 'a')
  {
    cout << "Activity name: ";
    getline(cin, actName);
    cout << chosenStudent << ".) " << logInCredential[courses[indexOfCourse].enrolledStudentID[chosenStudent - 1] - BASE_ID].username << endl;
    cout << "\tPerformace Task:" << endl;
    if (courses[indexOfCourse].studentRecords[chosenStudent - 1].performanceTask.size() > 0)
    {
      for (int j = 0; j < courses[indexOfCourse].studentRecords[chosenStudent - 1].performanceTask.size(); j++)
      {
        cout << "\t\t* " << courses[indexOfCourse].studentRecords[chosenStudent - 1].performanceTaskName[j] << " - " << courses[indexOfCourse].studentRecords[chosenStudent - 1].performanceTask[j] << endl;
      }
    }
    else
    {
      cout << "\t\t-------NO RECORD/s FOUND-------" << endl;
    }
    int toEditIndex;
    cout << "Enter number of task to edit: " << endl;
    cout << "-------------------------------------------------------" << endl;
    cout << ">> ";
    cin >> toEditIndex;
    toEditIndex--;

    cout << "Enter new name: ";
    getline(cin, actName);
    cout << "Enter new score: ";
    cin >> score;
    courses[indexOfCourse].studentRecords[chosenStudent - 1].performanceTaskName[toEditIndex] = actName;
    courses[indexOfCourse].studentRecords[chosenStudent - 1].performanceTask[toEditIndex] = score;
  }
  if (tolower(option) == 'b')
  {
    cout << "Activity name: ";
    getline(cin, actName);
    cout << chosenStudent << ".) " << logInCredential[courses[indexOfCourse].enrolledStudentID[chosenStudent - 1] - BASE_ID].username << endl;
    cout << "\tPerformace Task:" << endl;
    if (courses[indexOfCourse].studentRecords[chosenStudent - 1].writtenTask.size() > 0)
    {
      for (int j = 0; j < courses[indexOfCourse].studentRecords[chosenStudent - 1].writtenTask.size(); j++)
      {
        cout << "\t\t* " << courses[indexOfCourse].studentRecords[chosenStudent - 1].writtenTaskName[j] << " - " << courses[indexOfCourse].studentRecords[chosenStudent - 1].writtenTask[j] << endl;
      }
    }
    else
    {
      cout << "\t\t-------NO RECORD/s FOUND-------" << endl;
    }
    int toEditIndex;
    cout << "Enter number of task to edit: " << endl;
    cout << "-------------------------------------------------------" << endl;
    cout << ">> ";
    cin >> toEditIndex;
    toEditIndex--;

    cout << "Enter new name: ";
    getline(cin, actName);
    cout << "Enter new score: ";
    cin >> score;
    courses[indexOfCourse].studentRecords[chosenStudent - 1].writtenTaskName[toEditIndex] = actName;
    courses[indexOfCourse].studentRecords[chosenStudent - 1].writtenTask[toEditIndex] = score;
  }
  if (tolower(option) == 'c')
  {
    cout << "Activity name: ";
    getline(cin, actName);
    cout << chosenStudent << ".) " << logInCredential[courses[indexOfCourse].enrolledStudentID[chosenStudent - 1] - BASE_ID].username << endl;
    cout << "\tPerformace Task:" << endl;
    if (courses[indexOfCourse].studentRecords[chosenStudent - 1].writtenTask.size() > 0)
    {
      for (int j = 0; j < courses[indexOfCourse].studentRecords[chosenStudent - 1].majorExam.size(); j++)
      {
        cout << "\t\t* " << courses[indexOfCourse].studentRecords[chosenStudent - 1].majorExamName[j] << " - " << courses[indexOfCourse].studentRecords[chosenStudent - 1].majorExam[j] << endl;
      }
    }
    else
    {
      cout << "\t\t-------NO RECORD/s FOUND-------" << endl;
    }
    int toEditIndex;
    cout << "Enter number of task to edit: " << endl;
    cout << "-------------------------------------------------------" << endl;
    cout << ">> ";
    cin >> toEditIndex;
    toEditIndex--;

    cout << "Enter new name: ";
    getline(cin, actName);
    cout << "Enter new score: ";
    cin >> score;
    courses[indexOfCourse].studentRecords[chosenStudent - 1].majorExamName[toEditIndex] = actName;
    courses[indexOfCourse].studentRecords[chosenStudent - 1].majorExam[toEditIndex] = score;
  }
}

void generateReport(int indexOfCourse)
{
  string courseTitle = courses[indexOfCourse].courseName;
  string courseNumber = to_string(courses[indexOfCourse].courseID);

  string filename = "reports/" + courseTitle + " - " + courseNumber +".txt";

  ofstream outFile(filename);
  if (outFile.is_open()) 
  {
      outFile << "-------------------------------------------------------" << endl;
    if (courses[indexOfCourse].enrolledStudentID.size() > 0)
    {
      for (int j = 0; j < courses[indexOfCourse].enrolledStudentID.size(); j++)
      {
        outFile << j + 1 << ".) " << logInCredential[courses[indexOfCourse].enrolledStudentID[j] - BASE_ID].username << endl;
        outFile << "\tPerformace Task:" << endl;
        if (courses[indexOfCourse].studentRecords[j].performanceTask.size() > 0)
        {
          for (int k = 0; k < courses[indexOfCourse].studentRecords[j].performanceTask.size(); k++)
          {
            outFile << "\t\t* " << courses[indexOfCourse].studentRecords[j].performanceTaskName[k] << " - " << courses[indexOfCourse].studentRecords[j].performanceTask[k] << "/" << courses[indexOfCourse].studentRecords[j].performanceTaskOver[k] << endl;
          }
          courses[indexOfCourse].studentRecords[j].performanceTaskPercent = calculateScore(indexOfCourse, courses[indexOfCourse].studentRecords[j], 'a')/calculateScore(indexOfCourse, courses[indexOfCourse].studentRecords[j], 'c');
          outFile << "==============================" << endl;
          outFile << setprecision(2) << fixed << "\t\t\tTotal Score: " << calculateScore(indexOfCourse, courses[indexOfCourse].studentRecords[j], 'a') << "/" << calculateScore(indexOfCourse, courses[indexOfCourse].studentRecords[j], 'c') << endl;
          outFile << setprecision(0) << fixed << "\t\t\tTotal Ratio: " << courses[indexOfCourse].studentRecords[j].performanceTaskPercent * 100 << "%" << endl;
        }
        else
        {
          outFile << "\t\t-------NO RECORD/s FOUND-------" << endl;
        }
        
        outFile << "\tWritten Task:" << endl;

        if (courses[indexOfCourse].studentRecords[j].writtenTask.size() > 0)
        {
          for (int k = 0; k < courses[indexOfCourse].studentRecords[j].writtenTask.size(); k++)
          {
            outFile << "\t\t* " << courses[indexOfCourse].studentRecords[j].writtenTaskName[k] << " - " << courses[indexOfCourse].studentRecords[j].writtenTask[k] << "/" << courses[indexOfCourse].studentRecords[j].writtenTaskOver[k] << endl;
          }
          courses[indexOfCourse].studentRecords[j].writtenTaskPercent = calculateScore(indexOfCourse, courses[indexOfCourse].studentRecords[j], 'b')/calculateScore(indexOfCourse, courses[indexOfCourse].studentRecords[j], 'd');
          cout << "==============================" << endl;
          cout << setprecision(2) << fixed << "\t\t\tTotal Score: " << calculateScore(indexOfCourse, courses[indexOfCourse].studentRecords[j], 'b') << "/" << calculateScore(indexOfCourse, courses[indexOfCourse].studentRecords[j], 'd') << endl;
          cout << setprecision(0) << fixed << "\t\t\tTotal Ratio: " << courses[indexOfCourse].studentRecords[j].writtenTaskPercent * 100 << "%" << endl;
        }
        else
        {
          outFile << "\t\t-------NO RECORD/s FOUND-------" << endl;
        }
        
        outFile << "\tMajor Exam: " << endl;

        if (courses[indexOfCourse].studentRecords[j].majorExam.size() > 0)
        {
          for (int k = 0; k < courses[indexOfCourse].studentRecords[j].majorExam.size(); k++)
          {
            outFile << "\t\t* " << courses[indexOfCourse].studentRecords[j].majorExamName[k] << " - " << courses[indexOfCourse].studentRecords[j].majorExam[k] << endl;
          }
          courses[indexOfCourse].studentRecords[j].majorExamPercent = courses[indexOfCourse].studentRecords[j].majorExam[0]/courses[indexOfCourse].studentRecords[j].majorExamOver[0];
          outFile << "==============================" << endl;
          outFile << "\t\t\tTotal Score: " << courses[indexOfCourse].studentRecords[j].majorExam[0] << "/" << courses[indexOfCourse].studentRecords[j].majorExamOver[0] << endl;
          outFile << "\t\t\tTotal Ratio: " << courses[indexOfCourse].studentRecords[j].majorExam[0]/courses[indexOfCourse].studentRecords[j].majorExamOver[0] << endl;
        }
        else
        {
          outFile << "\t\t-------NO RECORD/s FOUND-------" << endl;
        }
        outFile << endl;
      }
    }
    else
    {
      outFile << "-------NO STUDENT IS ENROLLED-------" << endl;
    }
    outFile << "-------------------------------------------------------" << endl;
    outFile.close();
    cout << "File '" << filename << "' written successfully." << endl;
    system("pause");
  }
  else 
  {
        cout << "Error opening file: " << filename << endl;
  }
}

void addOrRemove(int indexOfCourse)
{
    char option;
    int studID;

    cout << "[a] Add Student\n[b] Remove Student\nChoice: ";
    cin >> option;

    // Reference to the enrolled student vector for easier access
    vector<int>& enrolled = courses[indexOfCourse].enrolledStudentID;

    if (option == 'a')
    {
        cout << "Enter Student ID to add: ";
        cin >> studID;

        // Avoid duplicate enrollment
        if (find(enrolled.begin(), enrolled.end(), studID) == enrolled.end()) {
            enrolled.push_back(studID);
            cout << "Student " << studID << " added successfully.\n";
        } else {
            cout << "Student is already enrolled.\n";
        }
    }
    else if (option == 'b')
    {
        cout << "Enter Student ID to remove: ";
        cin >> studID;

        // Find the student ID in the vector
        auto it = find(enrolled.begin(), enrolled.end(), studID);
        if (it != enrolled.end()) {
            enrolled.erase(it);  // Remove the student
            cout << "Student " << studID << " removed successfully.\n";
        } else {
            cout << "Student not found in course.\n";
        }
    }
    else
    {
        cout << "Invalid option.\n";
    }
}

void customizeGradeSettings(int indexOfCourse)
{
    float base, major, perf, written;

    cout << "Customize Grade Settings for: " << courses[indexOfCourse].courseName << endl;
    cout << "-----------------------------------------" << endl;

    cout << "Enter base grade (e.g., 50): ";
    cin >> base;

    cout << "Enter percentage for Major Exams: ";
    cin >> major;

    cout << "Enter percentage for Performance Tasks: ";
    cin >> perf;

    cout << "Enter percentage for Written Tasks: ";
    cin >> written;

    float total = major + perf + written;

    if (fabsf(total - 100.0f) > 0.01f)
    {
        cout << "Error: Percentages must add up to 100. You entered " << total << "%." << endl;
        return;
    }

    courses[indexOfCourse].base = base;
    courses[indexOfCourse].majorExamPercentage = major;
    courses[indexOfCourse].performanceTaskPercentage = perf;
    courses[indexOfCourse].writtenTaskPercentage = written;

    cout << "Grade settings updated successfully!\n";
}

void searchStudentSummary() 
{
    cin.ignore();
    string input;
    cout << "Enter Student ID or Name to search: ";
    getline(cin, input);

    bool studentFound = false;

    for (const User& user : logInCredential) {
        stringstream ss;
        ss << user.id;
        string idStr = ss.str();

        if (input == user.username || input == idStr) {
            studentFound = true;

            cout << "-------------------------------------------------------" << endl;
            cout << "Student Name : " << user.username << endl;
            cout << "ID           : " << user.id << endl;
            cout << "Section      : " << user.section << endl;
            cout << "GPA          : " << fixed << setprecision(2) << user.GPA << endl;
            cout << "-------------------------------------------------------" << endl;
            cout << "Courses and Final Grades:" << endl;

            for (int courseIndex : user.coursesEnrolled) {
                if (courseIndex < 0 || courseIndex >= courses.size()) continue;

                Course& course = courses[courseIndex];
                cout << "📘 " << course.courseName << " (Teacher: " << course.teacher << ")" << endl;

                for (const GradeRecord& record : course.studentRecords) {
                    if (record.studentID == user.id) {
                        cout << "   ➤ Final Grade: " << fixed << setprecision(2) << record.finalGrade << endl;
                        break;
                    }
                }

                cout << "-------------------------------------------------------" << endl;
            }

            break; // stop after finding the student
        }
    }

    if (!studentFound) {
        cout << "❌ Student not found in the system." << endl;
    }
}

void editUserProfile() 
{
    string newName;
    cout << "-------------------------------------------------------" << endl;
    cout << "Edit Profile - Change Name" << endl;
    cout << "Current Name: " << logInCredential[userIndex].username << endl;
    cout << "Enter new name: ";
    cin.ignore();
    getline(cin, newName);

    if (newName.empty()) {
        cout << "❌ Name cannot be empty. Profile not updated." << endl;
        return;
    }

    logInCredential[userIndex].username = newName;

    // Update name in courses (if user is a teacher)
    for (Course& course : courses) {
        if (course.teacherID == logInCredential[userIndex].id) {
            course.teacher = newName;
        }
    }

    cout << "✅ Name successfully updated to \"" << newName << "\"." << endl;
}
