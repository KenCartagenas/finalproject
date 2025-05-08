#include "mainmenu.h"
#include <iostream>
#include <string>
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
  while(true)
  {
    char option;
    displayMainMenuTemplate(" a. Create New Course / Subject\nb. Open Existing Class\nc. Search Student\nd. Settings\ne. Log Out");
    cin >> option;

    switch(tolower(option))
    {
      case 'a': createCourse(); break;
      case 'b': openClass(); break;
      case 'c': break;
      case 'd': break;
      case 'e': break;
    }
  }
  
}

void studentMenu()
{
  
}

void createCourse()
{
    string tempName, tempSection;
    char option, confirm;
    float tempBase, tempP, tempW, tempM;

    displayMainMenuTemplate("Create New Course:\nEnter course's name:");
    getline(cin, tempName);
    displayMainMenuTemplate("Create New Course:\nDo you want to use default base of grade (zero)?");
    if(tolower(option) == 'y')
    {
      tempBase = 0;
    }
    else
    {
      displayMainMenuTemplate("Create New Course:\nEnter base of grade: ");
      cin >> tempBase;
    }
    displayMainMenuTemplate("Create New Course:\nEnter students' section: ");
    getline(cin, tempSection);

    displayMainMenuTemplate("Create Course:\nEnter percent for Performance Task, Written Task, and Major Examination(e.g. 40 20 20): ");
    cin >> tempP >> tempW >> tempM;

    Course newCourse;
    newCourse.courseID = courseCount;
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
      }
    }

    logInCredential[userIndex].coursesHandled.push_back(newCourse.courseID);

    cout << "    Welcome " << logInCredential[userIndex].username << endl;
    cout << "-------------------------------------------------------" << endl;
    cout << "Course Name: " << newCourse.courseName << endl;
    cout << "Course ID: " << newCourse.courseID << endl;
    cout << "Teacher: " << newCourse.teacher << endl;
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
      cout << "Failed to create new course." << endl;
    }
}

void openClass()
{
  int chosenCourse, optionOpen, chosenStudent;

  cout << "    Welcome " << logInCredential[userIndex].username << endl;
  cout << "-------------------------------------------------------" << endl;
  cout << "Courses ID and Courses Name: " << endl;
  for (int i = 0; i < logInCredential[userIndex].coursesHandled; i++)
  {
    cout << logInCredential[userIndex].coursesHandled[i] << ".) " << courses[logInCredential[userIndex].coursesHandled[i]].name << endl;
  }
  cout << "-------------------------------------------------------" << endl;
  cout << ">> ";
  cin >> chosenCourse;
  
  int indexOfCourse = chosenCourse;
  cout << "    " << courses[indexOfCourse].name << endl;
  cout << "-------------------------------------------------------" << endl;
  cout << "1. Display All Students and Scores" << endl;
  cout << "2. Edit Student Scores" << endl;
  cout << "3. Calculate Averages or Grades" << endl;
  cout << "4. Generate Class Grade Report" << endl;
  cout << "5. Add or Remove Students" << endl;
  cout << "6. Customize Grade Settings" << endl;
  cout << "7. Return to Main Dashboard" << endl;
  cout << "-------------------------------------------------------" << endl;
  cout << ">> ";
  cin >> optionOpen;

  switch (optionOpen)
  {
  case 1:
    cout << "-------------------------------------------------------" << endl;
    if (courses[indexOfCourse].enrolledStudentID.size() > 0)
    {
      for (int j = 0; j < courses[indexOfCourse].enrolledStudentID.size(); j++)
      {
        cout << j + 1 << ".) " << logInCredential[courses[indexOfCourse].enrolledStudentID[j]].username << endl;
        cout << "\tPerformace Task:" << endl;
        if (course[indexOfCourse].studentRecords[j].performanceTask.size() > 0)
        {
          for (int k = 0; k < courses[indexOfCourse].studentRecords[j].performanceTask.size(); k++)
          {
            cout << "\t\t* " << courses[indexOfCourse].studentRecords[j].performanceTaskName[k] << " - " << courses[indexOfCourse].studentRecords[j].performanceTask[k] << endl;
          }
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
        }
        else
        {
          cout << "\t\t-------NO RECORD/s FOUND-------" << endl;
        }
        
        cout << "\tMajor Exam: " << endl;

        if (courses[indexOfCourse].studentRecords[j].majorTask.size() > 0)
        {
          for (int k = 0; k < courses[indexOfCourse].studentRecords[j].majorExamination.size(); k++)
          {
            cout << "\t\t* " << courses[indexOfCourse].studentRecords[j].majorExaminationName[k] << " - " << courses[indexOfCourse].studentRecords[j].majorTask[k] << endl;
          }
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
    break;
  case 2:
    cout << "-------------------------------------------------------" << endl;
    for (int i = 0; i < courses[indexOfCourse].enrolledStudentID.size(); i++)
    {
      cout << cout << j + 1 << ".) " << logInCredential[courses[indexOfCourse].enrolledStudentID[j]].username << endl;
    }
    cout << "Choose student: " << endl;
    cout << "-------------------------------------------------------" << endl;
    cout << ">>" << endl;
    cin >> chosenStudent;

    
    break;
  default:
    break;
  }
}
