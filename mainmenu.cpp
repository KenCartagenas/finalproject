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
  while(isLoggedIn)
  {
    char option;
    displayMainMenuTemplate(" [A]. Create New Course / Subject\n[B]. Open Existing Class\n[C]. Search Student\n[D]. Settings\n[E]. Log Out");
    cin >> option;

    switch(tolower(option))
    {
      case 'a': createCourse(); break;
      case 'b': openClass(); break;
      case 'c': break;
      case 'd': break;
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
    displayMainMenuTemplate("Create New Course:\nDo you want to use default base of grade (zero)?");
    cin >> option;
    if(tolower(option) == 'y')
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

    displayMainMenuTemplate("Create Course:\nEnter percent for Performance Task, Written Task, and Major Examination(e.g. 40 20 20): ");
    cin >> tempP >> tempW >> tempM;

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
    
    int indexOfCourse = courses[logInCredential[userIndex].coursesHandled[chosenCourse - 1]].courseID;
    cout << "    " << courses[indexOfCourse].courseName << endl;
    cout << "-------------------------------------------------------" << endl;
    cout << "1. Display All Students and Scores" << endl;
    cout << "2. Edit Student Scores" << endl;
    cout << "3. Generate Class Grade Report" << endl;
    cout << "4. Add or Remove Students" << endl;
    cout << "5. Customize Grade Settings" << endl;
    cout << "6. Return to Main Dashboard" << endl;
    cout << "-------------------------------------------------------" << endl;
    cout << ">> ";
    cin >> optionOpen;

    switch (optionOpen)
    {
    case 1:
      displayStudents(indexOfCourse);
      break;
    case 2:
      editStudent(indexOfCourse);
      break;
    case 3:
      break;
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

      if (courses[indexOfCourse].studentRecords[j].majorExam.size() > 0)
      {
        for (int k = 0; k < courses[indexOfCourse].studentRecords[j].majorExam.size(); k++)
        {
          cout << "\t\t* " << courses[indexOfCourse].studentRecords[j].majorExamName[k] << " - " << courses[indexOfCourse].studentRecords[j].majorExam[k] << endl;
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
    courses[indexOfCourse].studentRecords[chosenStudent - 1].performanceTaskName[toEditIndex] == actName;
    courses[indexOfCourse].studentRecords[chosenStudent - 1].performanceTask[toEditIndex] == score;
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
    courses[indexOfCourse].studentRecords[chosenStudent - 1].writtenTaskName[toEditIndex] == actName;
    courses[indexOfCourse].studentRecords[chosenStudent - 1].writtenTask[toEditIndex] == score;
  }
  if (tolower(option) == 'b')
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
    courses[indexOfCourse].studentRecords[chosenStudent - 1].majorExamName[toEditIndex] == actName;
    courses[indexOfCourse].studentRecords[chosenStudent - 1].majorExam[toEditIndex] == score;
  }
}

//cout << "-------------------------------------------------------" << endl;
//  for (int i = 0; i < courses[indexOfCourse].enrolledStudentID.size(); i++)
//  {
//    cout << i + 1 << ".) " << logInCredential[courses[indexOfCourse].enrolledStudentID[i] - BASE_ID].username << endl;
//  }
//  cout << "Choose student: " << endl;
//  cout << "-------------------------------------------------------" << endl;
//  cout << ">>" << endl;
//  cin >> chosenStudent;

