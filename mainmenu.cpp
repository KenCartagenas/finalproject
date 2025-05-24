#include "mainmenu.h"
#include "calculate.h"
#include <iostream>
#include <string>
#include <iomanip>
#include <fstream>
#include <algorithm>
#include <cmath>
#include "data.h"
#include "utils.h"
using namespace std;
//
void mainMenu()
{
  c();
  if (logInCredential[userIndex].role == "teacher")
  {
    teacherMenu();
  }
  else if (logInCredential[userIndex].role == "student")
  {
    studentMenu();
  }
}
//
void teacherMenu()
{
  while (isLoggedIn)
  {
    c();
    displayTemplate("[A]. Create New Course / Subject\n[B]. Open Existing Class\n[C]. Search Student\n[D]. Settings\n[E]. Generate Report of GPA\n[F]. View Notifications\n[G]. Log Out\n[H]. Quit");
    cout << "Enter your choice: ";
    char option = getChar();
    char confirm;

    switch (tolower(option))
    {
    case 'a':
      createCourse();
      break;
    case 'b':
      openClass();
      break;
    case 'c':
      searchStudentSummary();
      break;
    case 'd':
      editUserProfile();
      break;
    case 'e':
      calculateGPA();
      generateReportGPA();
      break;
    case 'f':
      openNotif();
      break;
    case 'g':
      cout << "Are you sure you want to Log Out?\nPress Y/y to confirm:" << endl;
      confirm = getChar();
      if (confirm == 'y')
      {
        isLoggedIn = false;
        loggedInID = 0;
        userIndex = 0;
      }
      break;
    case 'h':
      cout << "Thank you for using our SMS.";
      isRunning = false;
      return;
    }
  }
}
//
void studentMenu()
{
  while (isLoggedIn)
  {
    c();
    User &student = logInCredential[userIndex];

    // === DASHBOARD HEADER ===
    cout << "\n========= STUDENT DASHBOARD =========\n";
    cout << "Name    : " << student.username << endl;
    cout << "ID      : " << student.id << endl;
    cout << "Program : " << student.program << endl;
    cout << "Section : " << student.section << endl;

    // === GRADES DISPLAY AND GPA CALCULATION ===
    cout << "\nGrades:\n";

    float gpaSum = 0.0;
    int gpaCount = 0;

    if (student.finalGrades.empty())
    {
      cout << "  No final grades available yet.\n";
    }
    else
    {
      for (const auto &grade : student.finalGrades)
      {
        cout << setprecision(2) << fixed << "  - " << grade.courseName << ": " << grade.grade << endl;
        gpaSum += grade.grade;
        gpaCount++;
      }
    }

    // Calculate and display GPA
    cout << fixed << setprecision(2);
    if (gpaCount > 0)
    {
      student.GPA = gpaSum / gpaCount;
      cout << "GPA     : " << student.GPA << endl;
    }
    else
    {
      student.GPA = -1; // You may use -1 to mark undefined GPA
      cout << "GPA     : N/A" << endl;
    }

    // === MENU OPTIONS ===
    char option;
    displayMainMenuTemplate("[A]. Open Course\n[B]. View Notifications\n[C]. Log Out\n[D]. Quit");
    option = getChar();
    c();
    switch (tolower(option))
    {
    case 'a':
      if (student.coursesEnrolled.empty())
      {
        cout << "You're not enrolled in any courses yet.\n";
        get();
      }
      else
      {
        openCourse(student);
      }
      break;
    case 'b':
      openNotif();
      break;
    case 'c':
      cout << "Logging out...\n";
      isLoggedIn = false;
      loggedInID = 0;
      userIndex = 0;
      break;
    case 'd':
      cout << "Thank you for using our SMS.";
      isRunning = false;
      return;
    default:
      cout << "Invalid option. Please select A or B.\n";
      break;
    }
  }
}
//
void createCourse()
{
  coursesCount = courses.size();
  string tempName, tempSection;
  float tempBase, tempP, tempW, tempM;

  c();
  displayTemplate("        Create New Course");
  cout << "Enter course's name: ";
  getline(cin, tempName);
  if (tempName.empty())
  {
    cout << "-----------------------------------------" << endl;
    cout << "Course name cannot be empty!" << endl;
    get();
    return;
  }

  cout << "Enter students' section: ";
  getline(cin, tempSection);
  if (tempSection.empty())
  {
    cout << "-----------------------------------------" << endl;
    cout << "Section name cannot be empty!" << endl;
    get();
    return;
  }

  cout << "Is " << tempName << " a [B]Minor or [B]Major course?: ";
  char option = getChar();
  if (tolower(option) == 'a')
  {
    tempP = 45.0f;
    tempW = 25.0f;
    tempM = 30.0f;
  }
  else if (tolower(option) == 'b')
  {
    tempP = 50.0f;
    tempW = 20.0f;
    tempM = 30.0f;
  }
  else
  {
    cout << "-----------------------------------------" << endl;
    cout << "Invalid choice" << endl;
    get();
    return;
  }

  cout << "Enter base grade: ";
  tempBase = getInt();

  c();
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

  cout << "    Welcome " << logInCredential[userIndex].username << endl;
  cout << "-------------------------------------------------------" << endl;
  cout << "Course Name: " << newCourse.courseName << endl;
  cout << "Course ID: " << newCourse.courseID << endl;
  cout << "Teacher: " << newCourse.teacher << endl;
  cout << "Students: " << endl;
  for (int i = 0; i < newCourse.enrolledStudentID.size(); i++)
  {
    cout << "\t" << i + 1 << ". " << logInCredential[newCourse.enrolledStudentID[i] - BASE_ID].username << endl;
  }
  cout << "Enter Y/y to confirm: " << endl;
  cout << "-------------------------------------------------------" << endl;
  cout << "Enter choice: ";
  char confirm = getChar();

  if (tolower(confirm) == 'y')
  {
    cout << "You have successfully created a new course." << endl;
    courses.push_back(newCourse);
    logInCredential[userIndex].coursesHandled.push_back(newCourse.courseID);
    for (int i = 0; i < logInCredential.size(); i++)
    {
      if (logInCredential[i].section == tempSection)
      {
        logInCredential[i].coursesEnrolled.push_back(coursesCount);
      }
    }
    string notif = "You have been added to " + newCourse.courseName + " by " + logInCredential[userIndex].username;
    for (int i = 0; i < newCourse.enrolledStudentID.size(); i++)
    {
      logInCredential[newCourse.enrolledStudentID[i] - BASE_ID].notifications.push_back(notif);
    }
    get();
  }
  else
  {
    cout << "-----------------------------------------" << endl;
    cout << "Failed to create new course. Returning to the menu." << endl;
    get();
  }
}
//
void openClass()
{
  c();
  if (logInCredential[userIndex].coursesHandled.size() > 0)
  {
    cout << "=========================================" << endl;
    cout << "    Student Grades Management System" << endl;
    cout << "=========================================" << endl;
    cout << "            Courses Handled" << endl;
    cout << "-----------------------------------------" << endl;

    for (int i = 0; i < logInCredential[userIndex].coursesHandled.size(); i++)
    {
      cout << i + 1 << ". " << courses[logInCredential[userIndex].coursesHandled[i]].courseName << endl;
    }
    cout << "-----------------------------------------" << endl;
    cout << "Enter choice: ";
    int chosenCourse = getInt();

    if (chosenCourse < 1 || chosenCourse > logInCredential[userIndex].coursesHandled.size())
    {
      cout << "-----------------------------------------" << endl;
      cout << "Invalid course selection. Returning to main menu." << endl;
      get();
      return;
    }
    int indexOfCourse = courses[logInCredential[userIndex].coursesHandled[chosenCourse - 1]].courseID;

    while (true)
    {
      c();
      cout << "=========================================" << endl;
      cout << "    Student Grades Management System" << endl;
      cout << "=========================================" << endl;
      cout << courses[indexOfCourse].courseName << endl;
      cout << "-----------------------------------------" << endl;
      cout << "[A] Display All Students and Scores" << endl;
      cout << "[B] Edit Student Scores" << endl;
      cout << "[C] Generate Class Grade Report" << endl;
      cout << "[D] Add or Remove Students" << endl;
      cout << "[E] Customize Grade Settings" << endl;
      cout << "[F] Release Grades" << endl;
      cout << "[G] Return to Main Dashboard" << endl;
      cout << "-----------------------------------------" << endl;
      cout << "Enter choice: ";
      char optionOpen = getChar();
      c();

      switch (tolower(optionOpen))
      {
      case 'a':
        displayStudents(indexOfCourse);
        break;
      case 'b':
        editStudent(indexOfCourse);
        break;
      case 'c':
        generateReportCourse(indexOfCourse);
        break;
      case 'd':
        addOrRemove(indexOfCourse);
        break;
      case 'e':
        customizeGradeSettings(indexOfCourse);
        break;
      case 'f':
        releaseGrades(indexOfCourse);
      case 'g':
        cout << "Returning to main dashboard...." << endl;
        get();
        return;
      default:
        break;
      }
    }
  }
  else
  {
    cout << "=========================================" << endl;
    cout << "    Student Grades Management System" << endl;
    cout << "=========================================" << endl;
    cout << "You are not currently handling any courses." << endl;
    cout << "Create courses first." << endl;
    cout << "-----------------------------------------" << endl;
    get();
  }
}
//
void displayStudents(int indexOfCourse)
{
  c();
  cout << "=========================================" << endl;
  cout << "    Student Grades Management System" << endl;
  cout << "=========================================" << endl;
  cout << "Course: " << courses[indexOfCourse].courseName << endl;

  if (courses[indexOfCourse].enrolledStudentID.size() > 0)
  {
    for (int j = 0; j < courses[indexOfCourse].enrolledStudentID.size(); j++)
    {
      cout << "*****************************************" << endl;
      cout << j + 1 << ". " << logInCredential[courses[indexOfCourse].enrolledStudentID[j] - BASE_ID].username << endl;
      cout << "*****************************************" << endl;
      cout << "Performace Task:" << endl;
      if (courses[indexOfCourse].studentRecords[j].performanceTask.size() > 0)
      {
        for (int k = 0; k < courses[indexOfCourse].studentRecords[j].performanceTask.size(); k++)
        {
          cout << "\t" << courses[indexOfCourse].studentRecords[j].performanceTaskName[k] << ": " << courses[indexOfCourse].studentRecords[j].performanceTask[k] << "/" << courses[indexOfCourse].studentRecords[j].performanceTaskOver[k] << endl;
        }
        courses[indexOfCourse].studentRecords[j].performanceTaskPercent = calculateScore(indexOfCourse, courses[indexOfCourse].studentRecords[j], 'a') / calculateScore(indexOfCourse, courses[indexOfCourse].studentRecords[j], 'c');
        cout << "----------------------------" << endl;
        cout << setprecision(2) << fixed << "Total: " << calculateScore(indexOfCourse, courses[indexOfCourse].studentRecords[j], 'a') << "/" << calculateScore(indexOfCourse, courses[indexOfCourse].studentRecords[j], 'c') << " = " << courses[indexOfCourse].studentRecords[j].performanceTaskPercent * 100 << "%" << endl;
      }
      else
      {
        cout << "\t>> No records found." << endl;
      }

      cout << "\nWritten Task:" << endl;

      if (courses[indexOfCourse].studentRecords[j].writtenTask.size() > 0)
      {
        for (int k = 0; k < courses[indexOfCourse].studentRecords[j].writtenTask.size(); k++)
        {
          cout << "\t" << courses[indexOfCourse].studentRecords[j].writtenTaskName[k] << ": " << courses[indexOfCourse].studentRecords[j].writtenTask[k] << "/" << courses[indexOfCourse].studentRecords[j].writtenTaskOver[k] << endl;
        }
        courses[indexOfCourse].studentRecords[j].writtenTaskPercent = calculateScore(indexOfCourse, courses[indexOfCourse].studentRecords[j], 'b') / calculateScore(indexOfCourse, courses[indexOfCourse].studentRecords[j], 'd');
        cout << "----------------------------" << endl;
        cout << setprecision(2) << fixed << "Total: " << calculateScore(indexOfCourse, courses[indexOfCourse].studentRecords[j], 'b') << "/" << calculateScore(indexOfCourse, courses[indexOfCourse].studentRecords[j], 'd') << " = " << courses[indexOfCourse].studentRecords[j].writtenTaskPercent * 100 << "%" << endl;
      }
      else
      {
        cout << "\t>> No records found." << endl;
      }

      cout << "\nMajor Exam: " << endl;

      if (courses[indexOfCourse].studentRecords[j].majorExam.size() > 0)
      {
        for (int k = 0; k < courses[indexOfCourse].studentRecords[j].majorExam.size(); k++)
        {
          cout << courses[indexOfCourse].studentRecords[j].majorExamName[k] << " - " << courses[indexOfCourse].studentRecords[j].majorExam[k] << endl;
        }
        courses[indexOfCourse].studentRecords[j].majorExamPercent = calculateScore(indexOfCourse, courses[indexOfCourse].studentRecords[j], 'e') / calculateScore(indexOfCourse, courses[indexOfCourse].studentRecords[j], 'f');
        cout << "----------------------------" << endl;
        cout << setprecision(2) << fixed << "Total " << calculateScore(indexOfCourse, courses[indexOfCourse].studentRecords[j], 'e') << "/" << calculateScore(indexOfCourse, courses[indexOfCourse].studentRecords[j], 'f') << " = " << courses[indexOfCourse].studentRecords[j].majorExamPercent * 100 << "%" << endl;
        cout << "*****************************************" << endl;
        float final = calculateGrade(indexOfCourse, courses[indexOfCourse].studentRecords[j].performanceTaskPercent, courses[indexOfCourse].studentRecords[j].writtenTaskPercent, courses[indexOfCourse].studentRecords[j].majorExamPercent);
        float rawFinal = calculateGradeRaw(indexOfCourse, courses[indexOfCourse].studentRecords[j].performanceTaskPercent, courses[indexOfCourse].studentRecords[j].writtenTaskPercent, courses[indexOfCourse].studentRecords[j].majorExamPercent);
        cout << setprecision(2) << fixed << "Final Grade: " << final << " | " << rawFinal;
      }
      else
      {
        cout << "\t>> No records found." << endl;
      }
      cout << endl;
    }
  }
  else
  {
    cout << ">> No student is enrolled in this class." << endl;
  }
  cout << "-----------------------------------------" << endl;
  get();
}
//
void editStudent(int indexOfCourse)
{
  while (true)
  {
    c();
    cout << "=========================================" << endl;
    cout << "    Student Grades Management System" << endl;
    cout << "=========================================" << endl;
    cout << "Course: " << courses[indexOfCourse].courseName << endl;
    cout << "[A] - Add Scores to everyone" << endl;
    cout << "[B] - Add Score to a student" << endl;
    cout << "[C] - Edit score to a student" << endl;
    cout << "[D] - Go back" << endl;
    cout << "-----------------------------------------" << endl;
    char option = getChar();

    switch (tolower(option))
    {
    case 'a':
      addScoresToEveryone(indexOfCourse);
      break;
    case 'b':
      addScoreToStudent(indexOfCourse);
      break;
    case 'c':
      editScore(indexOfCourse);
    case 'd':
      break;
    default:
      break;
    }

    if (tolower(option) == 'd')
    {
      break;
    }
  }
}
//
void addScoresToEveryone(int indexOfCourse)
{
  c();
  cout << "=========================================" << endl;
  cout << "    Student Grades Management System" << endl;
  cout << "=========================================" << endl;
  cout << "Course: " << courses[indexOfCourse].courseName << endl;
  cout << "Add scores to? " << endl;
  cout << "[A] - Performance Task" << endl;
  cout << "[B] - Written Task" << endl;
  cout << "[C] - Major Exam" << endl;
  cout << "-----------------------------------------" << endl;
  char option = getChar();
  string actName;
  float score, over;
  if (courses[indexOfCourse].enrolledStudentID.size() > 0)
  {
    c();
    if (tolower(option) == 'a')
    {
      c();
      cout << "=========================================" << endl;
      cout << "    Student Grades Management System" << endl;
      cout << "=========================================" << endl;
      cout << "Course: " << courses[indexOfCourse].courseName << endl;
      cout << "Activity name: " << endl;
      cout << "-----------------------------------------" << endl;
      cout << ">> ";
      getline(cin, actName);
      c();

      cout << "=========================================" << endl;
      cout << "    Student Grades Management System" << endl;
      cout << "=========================================" << endl;
      cout << "Course: " << courses[indexOfCourse].courseName << endl;
      cout << "Over: " << endl;
      cout << "-----------------------------------------" << endl;
      over = getFloat();
      c();

      for (int i = 0; i < courses[indexOfCourse].enrolledStudentID.size(); i++)
      {
        c();
        cout << "=========================================" << endl;
        cout << "    Student Grades Management System" << endl;
        cout << "=========================================" << endl;
        cout << "Course: " << courses[indexOfCourse].courseName << endl;
        cout << i + 1 << ".) " << logInCredential[courses[indexOfCourse].enrolledStudentID[i] - BASE_ID].username << endl;
        cout << "\tPerformace Task:" << endl;
        if (courses[indexOfCourse].studentRecords[i].performanceTask.size() > 0)
        {
          for (int k = 0; k < courses[indexOfCourse].studentRecords[i].performanceTask.size(); k++)
          {
            cout << "\t" << courses[indexOfCourse].studentRecords[i].performanceTaskName[k] << ": " << courses[indexOfCourse].studentRecords[i].performanceTask[k] << "/" << courses[indexOfCourse].studentRecords[i].performanceTaskOver[k] << endl;
          }
          cout << "-----------------------------------------" << endl;
        }
        else
        {
          cout << "\t>> No records found." << endl;
          cout << "-----------------------------------------" << endl;
        }
        cout << "Add score to " << actName << ": " << endl;
        cout << "-----------------------------------------" << endl;
        score = getFloat();
        courses[indexOfCourse].studentRecords[i].performanceTaskName.push_back(actName);
        courses[indexOfCourse].studentRecords[i].performanceTask.push_back(score);
        courses[indexOfCourse].studentRecords[i].performanceTaskOver.push_back(over);
      }
    }
    else if (tolower(option) == 'b')
    {
      c();
      cout << "=========================================" << endl;
      cout << "    Student Grades Management System" << endl;
      cout << "=========================================" << endl;
      cout << "Course: " << courses[indexOfCourse].courseName << endl;
      cout << "Activity name: " << endl;
      cout << "-----------------------------------------" << endl;
      cout << ">> ";
      getline(cin, actName);
      c();

      cout << "=========================================" << endl;
      cout << "    Student Grades Management System" << endl;
      cout << "=========================================" << endl;
      cout << "Course: " << courses[indexOfCourse].courseName << endl;
      cout << "Over: " << endl;
      cout << "-----------------------------------------" << endl;
      over = getFloat();
      c();

      for (int i = 0; i < courses[indexOfCourse].enrolledStudentID.size(); i++)
      {
        c();
        cout << "=========================================" << endl;
        cout << "    Student Grades Management System" << endl;
        cout << "=========================================" << endl;
        cout << "Course: " << courses[indexOfCourse].courseName << endl;
        cout << i + 1 << ".) " << logInCredential[courses[indexOfCourse].enrolledStudentID[i] - BASE_ID].username << endl;
        cout << "\tWritten Task:" << endl;

        if (courses[indexOfCourse].studentRecords[i].writtenTask.size() > 0)
        {
          for (int k = 0; k < courses[indexOfCourse].studentRecords[i].writtenTask.size(); k++)
          {
            cout << "\t" << courses[indexOfCourse].studentRecords[i].writtenTaskName[k]
                 << ": " << courses[indexOfCourse].studentRecords[i].writtenTask[k]
                 << "/" << courses[indexOfCourse].studentRecords[i].writtenTaskOver[k] << endl;
          }
          cout << "-----------------------------------------" << endl;
        }
        else
        {
          cout << "\t>> No records found." << endl;
          cout << "-----------------------------------------" << endl;
        }

        cout << "Add score to " << actName << ": " << endl;
        cout << "-----------------------------------------" << endl;
        score = getFloat();
        courses[indexOfCourse].studentRecords[i].writtenTaskName.push_back(actName);
        courses[indexOfCourse].studentRecords[i].writtenTask.push_back(score);
        courses[indexOfCourse].studentRecords[i].writtenTaskOver.push_back(over);
      }
    }
    else if (tolower(option) == 'c')
    {
      c();
      cout << "=========================================" << endl;
      cout << "    Student Grades Management System" << endl;
      cout << "=========================================" << endl;
      cout << "Course: " << courses[indexOfCourse].courseName << endl;
      cout << "Activity name: " << endl;
      cout << "-----------------------------------------" << endl;
      cout << ">> ";
      getline(cin, actName);
      c();

      cout << "=========================================" << endl;
      cout << "    Student Grades Management System" << endl;
      cout << "=========================================" << endl;
      cout << "Course: " << courses[indexOfCourse].courseName << endl;
      cout << "Over: " << endl;
      cout << "-----------------------------------------" << endl;
      over = getFloat();
      c();

      for (int i = 0; i < courses[indexOfCourse].enrolledStudentID.size(); ++i)
      {
        c();
        cout << "=========================================" << endl;
        cout << "    Student Grades Management System" << endl;
        cout << "=========================================" << endl;
        cout << "Course: " << courses[indexOfCourse].courseName << endl;
        cout << i + 1 << ".) "
             << logInCredential[courses[indexOfCourse].enrolledStudentID[i] - BASE_ID].username
             << endl;

        cout << "\tMajor Exam:" << endl;

        if (!courses[indexOfCourse].studentRecords[i].majorExam.empty())
        {
          for (int j = 0; j < courses[indexOfCourse].studentRecords[i].majorExam.size(); ++j)
          {
            cout << "\t"
                 << courses[indexOfCourse].studentRecords[i].majorExamName[j]
                 << ": "
                 << courses[indexOfCourse].studentRecords[i].majorExam[j]
                 << "/"
                 << courses[indexOfCourse].studentRecords[i].majorExamOver[j]
                 << endl;
          }
          cout << "-----------------------------------------" << endl;
        }
        else
        {
          cout << "\t>> No records found." << endl;
          cout << "-----------------------------------------" << endl;
        }

        cout << "Add score to " << actName << ": " << endl;
        cout << "-----------------------------------------" << endl;
        score = getFloat();

        courses[indexOfCourse].studentRecords[i].majorExamName.push_back(actName);
        courses[indexOfCourse].studentRecords[i].majorExam.push_back(score);
        courses[indexOfCourse].studentRecords[i].majorExamOver.push_back(over);
      }
    }
    string notif = courses[indexOfCourse].courseName + ": \"" + actName + "\" has been graded.";
    for (int i = 0; i < courses[indexOfCourse].enrolledStudentID.size(); i++)
    {
      logInCredential[courses[indexOfCourse].enrolledStudentID[i] - BASE_ID].notifications.push_back(notif);
    }
  }
  else
  {
    cout << ">> No student is enrolled in this class." << endl;
    cout << "-----------------------------------------" << endl;
    get();
  }
}
//
void addScoreToStudent(int indexOfCourse)
{
  int chosenStudent;
  c();
  cout << "=========================================" << endl;
  cout << "    Student Grades Management System" << endl;
  cout << "=========================================" << endl;
  cout << "Course: " << courses[indexOfCourse].courseName << endl;
  for (int i = 0; i < courses[indexOfCourse].enrolledStudentID.size(); i++)
  {
    cout << i + 1 << ". " << logInCredential[courses[indexOfCourse].enrolledStudentID[i] - BASE_ID].username << endl;
  }
  cout << "Choose student: " << endl;
  cout << "-----------------------------------------" << endl;
  chosenStudent = getInt();

  if (chosenStudent > courses[indexOfCourse].enrolledStudentID.size())
  {
    c();
    return;
  }

  c();
  char option;
  cout << "=========================================" << endl;
  cout << "    Student Grades Management System" << endl;
  cout << "=========================================" << endl;
  cout << "Course: " << courses[indexOfCourse].courseName << endl;
  cout << "Add scores to? " << endl;
  cout << "[A] - Performance Task" << endl;
  cout << "[B] - Written Task" << endl;
  cout << "[C] - Major Exam" << endl;
  cout << "-----------------------------------------" << endl;
  option = getChar();

  c();
  string actName;
  float score, over;
  cout << "=========================================" << endl;
  cout << "    Student Grades Management System" << endl;
  cout << "=========================================" << endl;
  cout << "Course: " << courses[indexOfCourse].courseName << endl;
  cout << "Activity name: " << endl;
  cout << "-----------------------------------------" << endl;
  cout << ">> ";
  getline(cin, actName);
  c();

  cout << "=========================================" << endl;
  cout << "    Student Grades Management System" << endl;
  cout << "=========================================" << endl;
  cout << "Course: " << courses[indexOfCourse].courseName << endl;
  cout << "Over: " << endl;
  cout << "-----------------------------------------" << endl;
  over = getFloat();
  c();
  if (tolower(option) == 'a')
  {
    c();
    cout << "=========================================" << endl;
    cout << "    Student Grades Management System" << endl;
    cout << "=========================================" << endl;
    cout << "Course: " << courses[indexOfCourse].courseName << endl;
    cout << chosenStudent << ".) " << logInCredential[courses[indexOfCourse].enrolledStudentID[chosenStudent - 1] - BASE_ID].username << endl;
    cout << "\tPerformace Task:" << endl;
    if (courses[indexOfCourse].studentRecords[chosenStudent - 1].performanceTask.size() > 0)
    {
      for (int k = 0; k < courses[indexOfCourse].studentRecords[chosenStudent - 1].performanceTask.size(); k++)
      {
        cout << "\t" << courses[indexOfCourse].studentRecords[chosenStudent - 1].performanceTaskName[k] << ": " << courses[indexOfCourse].studentRecords[chosenStudent - 1].performanceTask[k] << "/" << courses[indexOfCourse].studentRecords[chosenStudent - 1].performanceTaskOver[k] << endl;
      }
      cout << "-----------------------------------------" << endl;
    }
    else
    {
      cout << "\t>> No records found." << endl;
      cout << "-----------------------------------------" << endl;
    }
    cout << "Add score to " << actName << ": " << endl;
    cout << "-----------------------------------------" << endl;
    score = getFloat();
    courses[indexOfCourse].studentRecords[chosenStudent - 1].performanceTaskName.push_back(actName);
    courses[indexOfCourse].studentRecords[chosenStudent - 1].performanceTask.push_back(score);
    courses[indexOfCourse].studentRecords[chosenStudent - 1].performanceTaskOver.push_back(over);
  }
  if (tolower(option) == 'b')
  {
    c();
    cout << "=========================================" << endl;
    cout << "    Student Grades Management System" << endl;
    cout << "=========================================" << endl;
    cout << "Course: " << courses[indexOfCourse].courseName << endl;
    cout << chosenStudent << ". " << logInCredential[courses[indexOfCourse].enrolledStudentID[chosenStudent - 1] - BASE_ID].username << endl;
    cout << "\tWritten Task:" << endl;

    if (courses[indexOfCourse].studentRecords[chosenStudent - 1].writtenTask.size() > 0)
    {
      for (int k = 0; k < courses[indexOfCourse].studentRecords[chosenStudent - 1].writtenTask.size(); k++)
      {
        cout << "\t" << courses[indexOfCourse].studentRecords[chosenStudent - 1].writtenTaskName[k]
             << ": " << courses[indexOfCourse].studentRecords[chosenStudent - 1].writtenTask[k]
             << "/" << courses[indexOfCourse].studentRecords[chosenStudent - 1].writtenTaskOver[k] << endl;
      }
      cout << "-----------------------------------------" << endl;
    }
    else
    {
      cout << "\t>> No records found." << endl;
      cout << "-----------------------------------------" << endl;
    }

    cout << "Add score to " << actName << ": " << endl;
    cout << "-----------------------------------------" << endl;
    score = getFloat();
    courses[indexOfCourse].studentRecords[chosenStudent - 1].writtenTaskName.push_back(actName);
    courses[indexOfCourse].studentRecords[chosenStudent - 1].writtenTask.push_back(score);
    courses[indexOfCourse].studentRecords[chosenStudent - 1].writtenTaskOver.push_back(over);
  }
  if (tolower(option) == 'c')
  {
    c();
    cout << "=========================================" << endl;
    cout << "    Student Grades Management System" << endl;
    cout << "=========================================" << endl;
    cout << "Course: " << courses[indexOfCourse].courseName << endl;
    cout << chosenStudent << ". "
         << logInCredential[courses[indexOfCourse].enrolledStudentID[chosenStudent - 1] - BASE_ID].username
         << endl;

    cout << "\tMajor Exam:" << endl;

    if (!courses[indexOfCourse].studentRecords[chosenStudent - 1].majorExam.empty())
    {
      for (int j = 0; j < courses[indexOfCourse].studentRecords[chosenStudent - 1].majorExam.size(); ++j)
      {
        cout << "\t"
             << courses[indexOfCourse].studentRecords[chosenStudent - 1].majorExamName[j]
             << ": "
             << courses[indexOfCourse].studentRecords[chosenStudent - 1].majorExam[j]
             << "/"
             << courses[indexOfCourse].studentRecords[chosenStudent - 1].majorExamOver[j]
             << endl;
      }
      cout << "-----------------------------------------" << endl;
    }
    else
    {
      cout << "\t>> No records found." << endl;
      cout << "-----------------------------------------" << endl;
    }

    cout << "Add score to " << actName << ": " << endl;
    cout << "-----------------------------------------" << endl;
    score = getFloat();

    courses[indexOfCourse].studentRecords[chosenStudent - 1].majorExamName.push_back(actName);
    courses[indexOfCourse].studentRecords[chosenStudent - 1].majorExam.push_back(score);
    courses[indexOfCourse].studentRecords[chosenStudent - 1].majorExamOver.push_back(over);
  }
  string notif = courses[indexOfCourse].courseName + ": \"" + actName + "\" has been graded.";
  logInCredential[courses[indexOfCourse].enrolledStudentID[chosenStudent - 1] - BASE_ID].notifications.push_back(notif);
}
//
void editScore(int indexOfCourse)
{
  int chosenStudent;
  c();
  cout << "=========================================" << endl;
  cout << "    Student Grades Management System" << endl;
  cout << "=========================================" << endl;
  cout << "Course: " << courses[indexOfCourse].courseName << endl;
  for (int i = 0; i < courses[indexOfCourse].enrolledStudentID.size(); i++)
  {
    cout << i + 1 << ". " << logInCredential[courses[indexOfCourse].enrolledStudentID[i] - BASE_ID].username << endl;
  }
  cout << "Choose student: " << endl;
  ;
  cout << "-----------------------------------------" << endl;
  chosenStudent = getInt();

  if (chosenStudent > courses[indexOfCourse].enrolledStudentID.size())
  {
    c();
    return;
  }

  c();
  char option;
  cout << "=========================================" << endl;
  cout << "    Student Grades Management System" << endl;
  cout << "=========================================" << endl;
  cout << "Course: " << courses[indexOfCourse].courseName << endl;
  cout << "Edit scores to? " << endl;
  cout << "[A] - Performance Task" << endl;
  cout << "[B] - Written Task" << endl;
  cout << "[C] - Major Exam" << endl;
  cout << "-----------------------------------------" << endl;
  option = getChar();

  c();
  string actName;
  float score;
  if (tolower(option) == 'a')
  {
    c();
    cout << "=========================================" << endl;
    cout << "    Student Grades Management System" << endl;
    cout << "=========================================" << endl;
    cout << "Course: " << courses[indexOfCourse].courseName << endl;
    cout << chosenStudent << ". " << logInCredential[courses[indexOfCourse].enrolledStudentID[chosenStudent - 1] - BASE_ID].username << endl;
    cout << "\tPerformace Task:" << endl;
    if (courses[indexOfCourse].studentRecords[chosenStudent - 1].performanceTask.size() > 0)
    {
      for (int k = 0; k < courses[indexOfCourse].studentRecords[chosenStudent - 1].performanceTask.size(); k++)
      {
        cout << "\t" << k + 1 << ". " << courses[indexOfCourse].studentRecords[chosenStudent - 1].performanceTaskName[k] << ": " << courses[indexOfCourse].studentRecords[chosenStudent - 1].performanceTask[k] << "/" << courses[indexOfCourse].studentRecords[chosenStudent - 1].performanceTaskOver[k] << endl;
      }
      cout << "-----------------------------------------" << endl;
    }
    else
    {
      cout << "\t>> No records found." << endl;
      cout << "-----------------------------------------" << endl;
    }
    int toEditIndex;
    cout << "Enter number of task to edit: " << endl;
    cout << "-----------------------------------------" << endl;
    toEditIndex = getInt();
    toEditIndex--;

    c();
    cout << "=========================================" << endl;
    cout << "    Student Grades Management System" << endl;
    cout << "=========================================" << endl;
    cout << "Course: " << courses[indexOfCourse].courseName << endl;
    cout << "Enter new name: " << endl;
    cout << "-----------------------------------------" << endl;
    cout << ">> ";
    getline(cin, actName);

    c();
    cout << "=========================================" << endl;
    cout << "    Student Grades Management System" << endl;
    cout << "=========================================" << endl;
    cout << "Course: " << courses[indexOfCourse].courseName << endl;
    cout << "Enter new score: " << endl;
    cout << "-----------------------------------------" << endl;
    score = getFloat();
    courses[indexOfCourse].studentRecords[chosenStudent - 1].performanceTaskName[toEditIndex] = actName;
    courses[indexOfCourse].studentRecords[chosenStudent - 1].performanceTask[toEditIndex] = score;
  }
  if (tolower(option) == 'b')
  {
    c();
    cout << "=========================================" << endl;
    cout << "    Student Grades Management System" << endl;
    cout << "=========================================" << endl;
    cout << "Course: " << courses[indexOfCourse].courseName << endl;
    cout << chosenStudent << ". " << logInCredential[courses[indexOfCourse].enrolledStudentID[chosenStudent - 1] - BASE_ID].username << endl;
    cout << "\tWritten Task:" << endl;

    if (courses[indexOfCourse].studentRecords[chosenStudent - 1].writtenTask.size() > 0)
    {
      for (int k = 0; k < courses[indexOfCourse].studentRecords[chosenStudent - 1].writtenTask.size(); k++)
      {
        cout << "\t" << k + 1 << ". "
             << courses[indexOfCourse].studentRecords[chosenStudent - 1].writtenTaskName[k]
             << ": "
             << courses[indexOfCourse].studentRecords[chosenStudent - 1].writtenTask[k]
             << "/"
             << courses[indexOfCourse].studentRecords[chosenStudent - 1].writtenTaskOver[k]
             << endl;
      }
      cout << "-----------------------------------------" << endl;
    }
    else
    {
      cout << "\t>> No records found." << endl;
      cout << "-----------------------------------------" << endl;
    }

    int toEditIndex;
    cout << "Enter number of task to edit: " << endl;
    cout << "-----------------------------------------" << endl;
    toEditIndex = getInt();
    toEditIndex--;

    c();
    cout << "=========================================" << endl;
    cout << "    Student Grades Management System" << endl;
    cout << "=========================================" << endl;
    cout << "Course: " << courses[indexOfCourse].courseName << endl;
    cout << "Enter new name: " << endl;
    cout << "-----------------------------------------" << endl;
    cout << ">> ";
    getline(cin, actName);

    c();
    cout << "=========================================" << endl;
    cout << "    Student Grades Management System" << endl;
    cout << "=========================================" << endl;
    cout << "Course: " << courses[indexOfCourse].courseName << endl;
    cout << "Enter new score: " << endl;
    cout << "-----------------------------------------" << endl;
    score = getFloat();

    courses[indexOfCourse].studentRecords[chosenStudent - 1].writtenTaskName[toEditIndex] = actName;
    courses[indexOfCourse].studentRecords[chosenStudent - 1].writtenTask[toEditIndex] = score;
  }
  if (tolower(option) == 'c')
  {
    c();
    cout << "=========================================" << endl;
    cout << "    Student Grades Management System" << endl;
    cout << "=========================================" << endl;
    cout << "Course: " << courses[indexOfCourse].courseName << endl;
    cout << chosenStudent << ". " << logInCredential[courses[indexOfCourse].enrolledStudentID[chosenStudent - 1] - BASE_ID].username << endl;
    cout << "\tMajor Exam:" << endl;

    if (courses[indexOfCourse].studentRecords[chosenStudent - 1].majorExam.size() > 0)
    {
      for (int k = 0; k < courses[indexOfCourse].studentRecords[chosenStudent - 1].majorExam.size(); k++)
      {
        cout << "\t" << k + 1 << ". "
             << courses[indexOfCourse].studentRecords[chosenStudent - 1].majorExamName[k]
             << ": "
             << courses[indexOfCourse].studentRecords[chosenStudent - 1].majorExam[k]
             << "/"
             << courses[indexOfCourse].studentRecords[chosenStudent - 1].majorExamOver[k]
             << endl;
      }
      cout << "-----------------------------------------" << endl;
    }
    else
    {
      cout << "\t>> No records found." << endl;
      cout << "-----------------------------------------" << endl;
    }

    int toEditIndex;
    cout << "Enter number of task to edit: " << endl;
    cout << "-----------------------------------------" << endl;
    toEditIndex = getInt();
    toEditIndex--;

    c();
    cout << "=========================================" << endl;
    cout << "    Student Grades Management System" << endl;
    cout << "=========================================" << endl;
    cout << "Course: " << courses[indexOfCourse].courseName << endl;
    cout << "Enter new name: " << endl;
    cout << "-----------------------------------------" << endl;
    cout << ">> ";
    getline(cin, actName);

    c();
    cout << "=========================================" << endl;
    cout << "    Student Grades Management System" << endl;
    cout << "=========================================" << endl;
    cout << "Course: " << courses[indexOfCourse].courseName << endl;
    cout << "Enter new score: " << endl;
    cout << "-----------------------------------------" << endl;
    score = getFloat();

    courses[indexOfCourse].studentRecords[chosenStudent - 1].majorExamName[toEditIndex] = actName;
    courses[indexOfCourse].studentRecords[chosenStudent - 1].majorExam[toEditIndex] = score;
  }
  string notif = courses[indexOfCourse].courseName + ": \"" + actName + "\" has been updated.";
  logInCredential[courses[indexOfCourse].enrolledStudentID[chosenStudent - 1] - BASE_ID].notifications.push_back(notif);
}
//
void generateReportCourse(int indexOfCourse)
{
  string courseTitle = courses[indexOfCourse].courseName;
  string courseNumber = to_string(courses[indexOfCourse].courseID);

  string filename = "reports/" + courseTitle + " - " + courseNumber + ".txt";

  ofstream outFile(filename);
  if (outFile.is_open())
  {
    outFile << "=========================================" << endl;
    outFile << "    Student Grades Management System" << endl;
    outFile << "=========================================" << endl;
    outFile << "Course: " << courses[indexOfCourse].courseName << endl;

    if (courses[indexOfCourse].enrolledStudentID.size() > 0)
    {
      for (int j = 0; j < courses[indexOfCourse].enrolledStudentID.size(); j++)
      {
        outFile << "*****************************************" << endl;
        outFile << j + 1 << ". " << logInCredential[courses[indexOfCourse].enrolledStudentID[j] - BASE_ID].username << endl;
        outFile << "*****************************************" << endl;
        outFile << "Performace Task:" << endl;
        if (courses[indexOfCourse].studentRecords[j].performanceTask.size() > 0)
        {
          for (int k = 0; k < courses[indexOfCourse].studentRecords[j].performanceTask.size(); k++)
          {
            outFile << "\t" << courses[indexOfCourse].studentRecords[j].performanceTaskName[k] << ": " << courses[indexOfCourse].studentRecords[j].performanceTask[k] << "/" << courses[indexOfCourse].studentRecords[j].performanceTaskOver[k] << endl;
          }
          courses[indexOfCourse].studentRecords[j].performanceTaskPercent = calculateScore(indexOfCourse, courses[indexOfCourse].studentRecords[j], 'a') / calculateScore(indexOfCourse, courses[indexOfCourse].studentRecords[j], 'c');
          outFile << "----------------------------" << endl;
          outFile << setprecision(2) << fixed << "Total: " << calculateScore(indexOfCourse, courses[indexOfCourse].studentRecords[j], 'a') << "/" << calculateScore(indexOfCourse, courses[indexOfCourse].studentRecords[j], 'c') << " = " << courses[indexOfCourse].studentRecords[j].performanceTaskPercent * 100 << "%" << endl;
        }
        else
        {
          outFile << "\t>> No records found." << endl;
        }

        outFile << "\nWritten Task:" << endl;

        if (courses[indexOfCourse].studentRecords[j].writtenTask.size() > 0)
        {
          for (int k = 0; k < courses[indexOfCourse].studentRecords[j].writtenTask.size(); k++)
          {
            outFile << "\t" << courses[indexOfCourse].studentRecords[j].writtenTaskName[k] << ": " << courses[indexOfCourse].studentRecords[j].writtenTask[k] << "/" << courses[indexOfCourse].studentRecords[j].writtenTaskOver[k] << endl;
          }
          courses[indexOfCourse].studentRecords[j].writtenTaskPercent = calculateScore(indexOfCourse, courses[indexOfCourse].studentRecords[j], 'b') / calculateScore(indexOfCourse, courses[indexOfCourse].studentRecords[j], 'd');
          outFile << "----------------------------" << endl;
          outFile << setprecision(2) << fixed << "Total: " << calculateScore(indexOfCourse, courses[indexOfCourse].studentRecords[j], 'b') << "/" << calculateScore(indexOfCourse, courses[indexOfCourse].studentRecords[j], 'd') << " = " << courses[indexOfCourse].studentRecords[j].writtenTaskPercent * 100 << "%" << endl;
        }
        else
        {
          outFile << "\t>> No records found." << endl;
        }

        outFile << "\nMajor Exam: " << endl;

        if (courses[indexOfCourse].studentRecords[j].majorExam.size() > 0)
        {
          for (int k = 0; k < courses[indexOfCourse].studentRecords[j].majorExam.size(); k++)
          {
            outFile << courses[indexOfCourse].studentRecords[j].majorExamName[k] << " - " << courses[indexOfCourse].studentRecords[j].majorExam[k] << endl;
          }
          courses[indexOfCourse].studentRecords[j].majorExamPercent = courses[indexOfCourse].studentRecords[j].majorExam[0] / courses[indexOfCourse].studentRecords[j].majorExamOver[0];
          outFile << "----------------------------" << endl;
          outFile << setprecision(2) << fixed << "Total " << courses[indexOfCourse].studentRecords[j].majorExam[0] << "/" << courses[indexOfCourse].studentRecords[j].majorExamOver[0] << " = " << courses[indexOfCourse].studentRecords[j].majorExamPercent * 100 << "%" << endl;
          outFile << "*****************************************" << endl;
          float final = calculateGrade(indexOfCourse, courses[indexOfCourse].studentRecords[j].performanceTaskPercent, courses[indexOfCourse].studentRecords[j].writtenTaskPercent, courses[indexOfCourse].studentRecords[j].majorExamPercent);
          outFile << setprecision(2) << fixed << "Final Grade: " << final;
        }
        else
        {
          outFile << "\t>> No records found." << endl;
        }
        outFile << endl;
      }
    }
    else
    {
      outFile << ">> No student is enrolled in this class." << endl;
    }
    outFile << "-----------------------------------------" << endl;
    outFile.close();
    cout << "File '" << filename << "' written successfully." << endl;
    system("pause");
  }
  else
  {
    cout << "Error opening file: " << filename << endl;
  }
}
//
void generateReportGPA()
{
  calculateGPA();
  string filename = "GPAreports/report.txt";
  ofstream outFile(filename);
  int studentCount;

  for (int i = 0; i < logInCredential.size(); i++)
  {
    if (logInCredential[i].role == "student")
    {
      studentCount++;
    }
  }

  if (outFile.is_open())
  {
    outFile << "-----------------------------------------" << endl;
    for (int i = 0; i < logInCredential.size(); i++)
    {
      if (logInCredential[i].role == "student")
      {
        outFile << logInCredential[i].id << ": " << endl;
        for (int j = 0; j < logInCredential[i].finalGrades.size(); j++)
        {
          outFile << setprecision(2) << fixed << "\t" << logInCredential[i].finalGrades[j].courseName << ": " << logInCredential[i].finalGrades[j].grade << endl;
        }

        outFile << "\nGPA: " << logInCredential[i].GPA << endl;
        outFile << "==========================================\n"
                << endl;
      }
    }
    outFile.close();
    cout << "File '" << filename << "' written successfully.\nPlease download the file immediately after generating the report." << endl;
  }
  else
  {
    cout << "Failed to open/create file" << endl;
  }

  get();
}
//
void addOrRemove(int indexOfCourse)
{
  c();
  char option;
  int studID;
  int courseID = courses[indexOfCourse].courseID;

  cout << "=========================================" << endl;
  cout << "    Student Grades Management System" << endl;
  cout << "=========================================" << endl;
  cout << "[A] Add Student\n[B] Remove Student\n";
  cout << "-----------------------------------------" << endl;
  option = getChar();

  vector<int> &enrolled = courses[indexOfCourse].enrolledStudentID;

  c();
  if (option == 'a')
  {
    cout << "=========================================" << endl;
    cout << "    Student Grades Management System" << endl;
    cout << "=========================================" << endl;
    cout << "Enter Student ID to add: " << endl;
    cout << "-----------------------------------------" << endl;
    studID = getInt();

    if (find(enrolled.begin(), enrolled.end(), studID) == enrolled.end())
    {
      // Add GradeRecord
      GradeRecord newRecord;
      newRecord.studentID = studID;
      courses[indexOfCourse].studentRecords.push_back(newRecord);

      // Add course to student's enrolled list
      for (User &user : logInCredential)
      {
        if (user.id == studID)
        {
          user.coursesEnrolled.push_back(courseID);
          break;
        }
      }
      enrolled.push_back(studID);
      cout << "Student " << studID << " added successfully.\n";
      string notif = "You haven been added to " + courses[indexOfCourse].courseName;
      logInCredential[studID - BASE_ID].notifications.push_back(notif);
      get();
    }

    else
    {
      cout << "Student is already enrolled.\n";
      get();
    }
  }
  else if (option == 'b')
  {
    cout << "=========================================" << endl;
    cout << "    Student Grades Management System" << endl;
    cout << "=========================================" << endl;
    cout << "Enter Student ID to remove: " << endl;
    cout << "-----------------------------------------" << endl;
    studID = getInt();

    // 1. Remove student from enrolledStudentID
    auto it = find(enrolled.begin(), enrolled.end(), studID);
    if (it != enrolled.end())
    {
      enrolled.erase(it);

      // 2. Remove grade record
      auto &records = courses[indexOfCourse].studentRecords;
      records.erase(remove_if(records.begin(), records.end(),
                              [studID](const GradeRecord &record)
                              {
                                return record.studentID == studID;
                              }),
                    records.end());

      // 3. Remove this course from student's coursesEnrolled
      for (User &user : logInCredential)
      {
        if (user.id == studID)
        {
          auto &enrolledCourses = user.coursesEnrolled;
          enrolledCourses.erase(remove(enrolledCourses.begin(), enrolledCourses.end(), courseID), enrolledCourses.end());
          break;
        }
      }

      cout << "Student " << studID << " removed successfully from the course and all related records.\n";
      string notif = "You haven been removed to " + courses[indexOfCourse].courseName;
      logInCredential[studID - BASE_ID].notifications.push_back(notif);
      get();
    }
    else
    {
      cout << "Student not found in course.\n";
      get();
    }
  }
  else
  {
    cout << "Invalid option.\n";
    get();
  }
}
//
void customizeGradeSettings(int indexOfCourse)
{
  float base, major, perf, written;
  c();
  cout << "=========================================" << endl;
  cout << "    Student Grades Management System" << endl;
  cout << "=========================================" << endl;
  cout << "Customize Grade Settings for: " << courses[indexOfCourse].courseName << endl;
  cout << "-----------------------------------------" << endl;
  cout << "Enter base grade (e.g., 50): ";
  base = getFloat();
  cout << "Enter percentage for Major Exams: ";
  major = getFloat();
  cout << "Enter percentage for Performance Tasks: ";
  perf = getFloat();
  cout << "Enter percentage for Written Tasks: ";
  written = getFloat();

  float total = major + perf + written;

  if (fabsf(total - 100.0f) > 0.01f)
  {
    cout << "Error: Percentages must add up to 100. You entered " << total << "%." << endl;
    get();
    return;
  }

  courses[indexOfCourse].base = base;
  courses[indexOfCourse].majorExamPercentage = major;
  courses[indexOfCourse].performanceTaskPercentage = perf;
  courses[indexOfCourse].writtenTaskPercentage = written;

  cout << "Grade settings updated successfully!\n";
  get();
}
//
void searchStudentSummary()
{
  c();
  cout << "=========================================" << endl;
  cout << "    Student Grades Management System" << endl;
  cout << "=========================================" << endl;
  cout << "Hello, " << logInCredential[userIndex].username << "\n"
       << endl;
  cout << "[A] Search by Name: \n[B] Search by ID: " << endl;
  cout << "-----------------------------------------" << endl;
  char option = getChar();
  bool studentFound = false;
  int index;

  c();
  if (tolower(option) == 'a')
  {
    string name;
    cout << "=========================================" << endl;
    cout << "    Student Grades Management System" << endl;
    cout << "=========================================" << endl;
    cout << "Search By Name[Enter Name]: " << endl;
    cout << "-----------------------------------------" << endl;
    cout << ">> ";
    getline(cin, name);

    for (int i = 0; i < logInCredential.size(); i++)
    {
      if (logInCredential[i].username == name)
      {
        index = logInCredential[i].id - BASE_ID;
        studentFound = true;
        break;
      }
    }
  }
  else if (tolower(option) == 'b')
  {
    cout << "=========================================" << endl;
    cout << "    Student Grades Management System" << endl;
    cout << "=========================================" << endl;
    cout << "Search By ID[Enter ID]: " << endl;
    cout << "-----------------------------------------" << endl;
    int id = getInt();
    index = id - BASE_ID;
    if (id < logInCredential.size() && id >= 0)
    {
      if (logInCredential[index].role == "student")
      {
        studentFound = true;
      }
    }
  }
  else
  {
    cout << "Please enter a valid option" << endl;
    get();
    return;
  }

  c();

  if (studentFound = true)
  {
    User user = logInCredential[index];
    cout << "-------------------------------------------------------" << endl;
    cout << "Student Name : " << user.username << endl;
    cout << "ID           : " << user.id << endl;
    cout << "Section      : " << user.section << endl;
    cout << "Program      : " << user.program << endl;
    if (user.GPA > 0 && user.GPA <= 5.0)
    {
      cout << "GPA          : " << fixed << setprecision(2) << user.GPA << endl;
    }
    else
    {
      cout << "GPA          : Not Available" << endl;
    }
    cout << "-------------------------------------------------------" << endl;
    cout << "Courses and Final Grades:" << endl;

    for (int courseIndex : user.coursesEnrolled)
    {
      if (courseIndex < 0 || courseIndex >= courses.size())
        continue;

      Course &course = courses[courseIndex];
      cout << " " << course.courseName << " (Teacher: " << course.teacher << ")" << endl;

      for (const GradeRecord &record : course.studentRecords)
      {
        if (record.studentID == user.id)
        {
          cout << "   => Final Grade: " << fixed << setprecision(2) << record.finalGrade << endl;
          break;
        }
      }
      cout << "-------------------------------------------------------" << endl;
    }
  }
  if (!studentFound)
  {
    cout << "Student not found in the system." << endl;
  }
  system("pause");
}
//
void editUserProfile()
{
  string newName;
  cout << "-------------------------------------------------------" << endl;
  cout << "Edit Profile - Change Name" << endl;
  cout << "Current Name: " << logInCredential[userIndex].username << endl;
  cout << "Enter new name: ";
  getline(cin, newName);

  if (newName.empty())
  {
    cout << "Name cannot be empty. Profile not updated." << endl;
    return;
  }

  logInCredential[userIndex].username = newName;

  for (Course &course : courses)
  {
    if (course.teacherID == logInCredential[userIndex].id)
    {
      course.teacher = newName;
    }
  }

  cout << "Name successfully updated to \"" << newName << "\"." << endl;
}
//
void viewActivities(const GradeRecord &record)
{
  c();
  cout << "=========================================" << endl;
  cout << "    Student Grades Management System" << endl;
  cout << "=========================================" << endl;
  cout << "\n--- Activities ---\n";

  cout << "Major Exams:\n";
  if (!record.majorExam.empty())
  {
    for (size_t i = 0; i < record.majorExam.size(); ++i)
    {
      cout << "- " << record.majorExamName[i] << ": " << record.majorExam[i] << "/" << record.majorExamOver[i] << endl;
    }
  }
  else
  {
    cout << ">> No records found" << endl;
  }

  cout << "Performance Tasks:\n";
  if (!record.performanceTask.empty())
  {
    for (size_t i = 0; i < record.performanceTask.size(); ++i)
    {
      cout << "- " << record.performanceTaskName[i] << ": " << record.performanceTask[i] << "/" << record.performanceTaskOver[i] << endl;
    }
  }
  else
  {
    cout << ">> No records found" << endl;
  }

  cout << "Written Tasks:\n";
  if (!record.writtenTask.empty())
  {
    for (size_t i = 0; i < record.writtenTask.size(); ++i)
    {
      cout << "- " << record.writtenTaskName[i] << ": " << record.writtenTask[i] << "/" << record.writtenTaskOver[i] << endl;
    }
  }
  else
  {
    cout << ">> No records found" << endl;
  }
  cout << "-----------------------------------------" << endl;
  get();
}

void calculateTargetScore(const GradeRecord &record, const Course &course)
{
  c();
  float currentGrade = record.finalGrade;
  float desiredGrade;
  cout << "=========================================" << endl;
  cout << "    Student Grades Management System" << endl;
  cout << "=========================================" << endl;
  cout << "\nEnter your desired final grade: " << endl;
  cout << "-----------------------------------------" << endl;
  desiredGrade = getFloat();

  float examSum = 0, perfSum = 0, writtenSum = 0;
  float examTotal = 0, perfTotal = 0, writtenTotal = 0;

  for (size_t i = 0; i < record.majorExam.size(); ++i)
  {
    examSum += record.majorExam[i];
    examTotal += record.majorExamOver[i];
  }

  for (size_t i = 0; i < record.performanceTask.size(); ++i)
  {
    perfSum += record.performanceTask[i];
    perfTotal += record.performanceTaskOver[i];
  }

  for (size_t i = 0; i < record.writtenTask.size(); ++i)
  {
    writtenSum += record.writtenTask[i];
    writtenTotal += record.writtenTaskOver[i];
  }

  float currentScore = 0.0;
  int componentCount = 0;

  if (examTotal > 0)
  {
    currentScore += (examSum / examTotal) * course.majorExamPercentage;
    componentCount++;
  }
  if (perfTotal > 0)
  {
    currentScore += (perfSum / perfTotal) * course.performanceTaskPercentage;
    componentCount++;
  }
  if (writtenTotal > 0)
  {
    currentScore += (writtenSum / writtenTotal) * course.writtenTaskPercentage;
    componentCount++;
  }

  if (componentCount == 0)
  {
    cout << "No recorded scores yet. Cannot calculate target.\n";
    return;
  }

  float needed = desiredGrade - currentScore * 100;

  cout << fixed << setprecision(2);
  cout << "\nCurrent Grade: " << currentScore * 100 << "%" << endl;
  cout << "To reach " << desiredGrade << ", you need " << needed << "% more." << endl;
  cout << "(Assuming future activities can still boost your grade.)\n";
}
//
void openCourse(const User &student)
{
  c();
  cout << "=========================================" << endl;
  cout << "    Student Grades Management System" << endl;
  cout << "=========================================" << endl;
  vector<int> myCourses = student.coursesEnrolled;
  if (myCourses.empty())
  {
    cout << "You're not enrolled in any courses.\n";
    cout << "-----------------------------------------" << endl;
    int ok = getInt();
    get();
    return;
  }

  cout << "\nYour Courses:\n";
  for (size_t i = 0; i < myCourses.size(); ++i)
  {
    int courseID = myCourses[i];
    for (const auto &course : courses)
    {
      if (course.courseID == courseID)
      {
        cout << i + 1 << ". " << course.courseName << endl;
      }
    }
  }

  int choice;
  cout << "-----------------------------------------" << endl;
  choice = getInt();
  if (choice < 1 || choice > (int)myCourses.size())
  {
    cout << "Invalid choice.\n";
    get();
    return;
  }

  int selectedCourseID = myCourses[choice - 1];
  Course *selectedCourse = nullptr;
  GradeRecord *selectedRecord = nullptr;

  for (auto &course : courses)
  {
    if (course.courseID == selectedCourseID)
    {
      selectedCourse = &course;
      for (auto &record : course.studentRecords)
      {
        if (record.studentID == student.id)
        {
          selectedRecord = &record;
          break;
        }
      }
      break;
    }
  }

  if (!selectedCourse || !selectedRecord)
  {
    cout << "Course data not found.\n";
    get();
    return;
  }

  while (true)
  {
    c();
    cout << "=========================================" << endl;
    cout << "    Student Grades Management System" << endl;
    cout << "=========================================" << endl;
    cout << "Course: " << selectedCourse->courseName << endl;
    cout << "1. View Activities\n";
    cout << "2. Calculate Target Score\n";
    cout << "3. Back to Dashboard\n";
    cout << "-----------------------------------------" << endl;
    int opt = getInt();

    if (opt == 1)
    {
      viewActivities(*selectedRecord);
    }
    else if (opt == 2)
    {
      calculateTargetScore(*selectedRecord, *selectedCourse);
    }
    else if (opt == 3)
    {
      break;
    }
    else
    {
      cout << "Invalid input. Try again.\n";
    }
  }
  c();
}
//
void releaseGrades(int indexOfCourse)
{
  c();
  cout << "=========================================" << endl;
  cout << "    Student Grades Management System" << endl;
  cout << "=========================================" << endl;
  cout << "Do you want to release grades in " << courses[indexOfCourse].courseName << endl;
  cout << "-----------------------------------------" << endl;
  char confirm = getChar();
  if (confirm != 'y')
  {
    return;
  }

  for (int j = 0; j < courses[indexOfCourse].enrolledStudentID.size(); j++)
  {
    float final = getFinal(indexOfCourse, j);
    if (logInCredential[courses[indexOfCourse].enrolledStudentID[j] - BASE_ID].finalGrades.empty())
    {
      Grade grades;
      grades.grade = final;
      grades.courseName = courses[indexOfCourse].courseName;

      logInCredential[courses[indexOfCourse].enrolledStudentID[j] - BASE_ID].finalGrades.push_back(grades);
    }

    for (int i = 0; i < logInCredential[courses[indexOfCourse].enrolledStudentID[j] - BASE_ID].finalGrades.size(); i++)
    {
      if (logInCredential[courses[indexOfCourse].enrolledStudentID[j] - BASE_ID].finalGrades[i].courseName == courses[indexOfCourse].courseName)
      {
        Grade grades;
        grades.grade = final;
        grades.courseName = courses[indexOfCourse].courseName;

        logInCredential[courses[indexOfCourse].enrolledStudentID[j] - BASE_ID].finalGrades[i] = grades;
      }
      if (i == logInCredential[courses[indexOfCourse].enrolledStudentID[j] - BASE_ID].finalGrades.size() - 1 && logInCredential[courses[indexOfCourse].enrolledStudentID[j] - BASE_ID].finalGrades[i].courseName != courses[indexOfCourse].courseName)
      {
        Grade grades;
        grades.grade = final;
        grades.courseName = courses[indexOfCourse].courseName;

        logInCredential[courses[indexOfCourse].enrolledStudentID[j] - BASE_ID].finalGrades.push_back(grades);
      }
    }
  }
  string notif = "Grades in " + courses[indexOfCourse].courseName + " have been posted.";
  for (int i = 0; i < courses[indexOfCourse].enrolledStudentID.size(); i++)
  {
    logInCredential[courses[indexOfCourse].enrolledStudentID[i] - BASE_ID].notifications.push_back(notif);
  }

  cout << "Grades are now posted." << endl;
  get();
}
//
void openNotif()
{
  c();
  cout << "=========================================" << endl;
  cout << "    Student Grades Management System" << endl;
  cout << "=========================================" << endl;
  cout << "Notifications: " << endl;
  for (int i = 0; i < logInCredential[userIndex].notifications.size(); i++)
  {
    cout << "> " << logInCredential[userIndex].notifications[i] << endl;
  }
  cout << "-----------------------------------------" << endl;
  get();
}
