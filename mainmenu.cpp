#include "mainmenu.h"
#include "calculate.h"
#include <iostream>
#include <string>
#include <iomanip>
#include <fstream>
#include <algorithm>
#include <cmath>
#include <ctime>
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
    time_t timestamp = time(NULL);
    struct tm datetime = *localtime(&timestamp);
    int totalStudent = calculateTotalstudents();
    float averageGrade = calculateAverageGrade();
    int activeClasses = logInCredential[userIndex].coursesHandled.size();
    c();
    cout << "=========================================" << endl;
    cout << "    Student Grades Management System" << endl;
    cout << "=========================================" << endl;
    cout << asctime(&datetime) << endl;
    cout << "Total Students: " << totalStudent << endl;
    cout << setprecision(2) << fixed << "Average Grade: " << averageGrade << endl;
    cout << "Active Classes: " << activeClasses << endl;
    cout << "-----------------------------------------" << endl;
    cout << "[A]. Create New Course / Subject\n[B]. Open Existing Class\n[C]. Search Student\n[D]. Settings\n[E]. Generate Report of GPA\n[F]. View Notifications\n[G]. Log Out\n[H]. Quit" << endl;
    cout << "-----------------------------------------" << endl;

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
    saveData();
  }
}
//
void studentMenu()
{
  while (isLoggedIn)
  {
    c();
    calculateAll();
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
    displayTemplate("[A]. Open Course\n[B]. View Notifications\n[C]. Log Out\n[D]. Quit");
    cout << "Enter choice: ";
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
    saveData();
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
  for (int i = 0; i < tempSection.size(); i++)
  {
    if (isdigit(tempSection[i]))
    {
      break;
    }
    else
    {
      tempSection[i] = toupper(tempSection[i]);
    }
  }
  if (tempSection.empty())
  {
    cout << "-----------------------------------------" << endl;
    cout << "Section name cannot be empty!" << endl;
    get();
    return;
  }

  cout << "Is " << tempName << " a [A]Minor or [B]Major course?: ";
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
  int index;
  for (int i = 0; i < newCourse.enrolledStudentID.size(); i++)
  {
    index = indexFind(newCourse.enrolledStudentID[i]);
    cout << "\t" << i + 1 << ". " << logInCredential[index].username << endl;
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
    int index;
    for (int i = 0; i < newCourse.enrolledStudentID.size(); i++)
    {
      index = indexFind(newCourse.enrolledStudentID[i]);
      logInCredential[index].notifications.push_back(notif);
      newCourse.studentRecords[i].finalGrade = 0.0;
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
      cout << "[G] View Messages" << endl;
      cout << "[H] Return to Main Dashboard" << endl;
      cout << "-----------------------------------------" << endl;
      cout << "Enter choice: ";
      char optionOpen = getChar();

      switch (tolower(optionOpen))
      {
      case 'a':
        c();
        displayStudents(indexOfCourse);
        get();
        break;
      case 'b':
        c();
        editStudent(indexOfCourse);
        break;
      case 'c':
        c();
        generateReportCourse(indexOfCourse);
        break;
      case 'd':
        c();
        addOrRemove(indexOfCourse);
        break;
      case 'e':
        c();
        customizeGradeSettings(indexOfCourse);
        break;
      case 'f':
        c();
        releaseGrades(indexOfCourse);
      case 'g':
        viewMessages(indexOfCourse);
        break;
      case 'h':
        cout << "Returning to main dashboard...." << endl;
        get();
        return;
      default:
        c();
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
  cout << "-----------------------------------------" << endl;

  int maxLength = 0;
  int index = 0;

  if (courses[indexOfCourse].enrolledStudentID.empty())
  {
    cout << ">> No students enrolled" << endl;
    return;
  }

  for (int i = 0; i < courses[indexOfCourse].enrolledStudentID.size(); i++)
  {
    index = indexFind(courses[indexOfCourse].enrolledStudentID[i]);
    if (logInCredential[index].username.size() > maxLength)
    {
      maxLength = logInCredential[index].username.size();
    }
  }

  cout << left << setw(5) << "#" << setw(maxLength + 2) << "Name";
  for (int i = 0; i < courses[indexOfCourse].studentRecords[0].performanceTaskName.size(); i++)
  {
    cout << setw(8) << courses[indexOfCourse].studentRecords[0].performanceTaskName[i];
  }

  cout << setw(8) << "%";

  for (int i = 0; i < courses[indexOfCourse].studentRecords[0].writtenTaskName.size(); i++)
  {
    cout << setw(8) << courses[indexOfCourse].studentRecords[0].writtenTaskName[i];
  }

  cout << setw(8) << "%";

  for (int i = 0; i < courses[indexOfCourse].studentRecords[0].majorExamName.size(); i++)
  {
    cout << setw(8) << courses[indexOfCourse].studentRecords[0].majorExamName[i];
  }

  cout << setw(8) << "%" << "Final";

  cout << endl;

  cout << left << setw(5) << "*" << setw(maxLength + 2) << "Max Score:";
  for (int i = 0; i < courses[indexOfCourse].studentRecords[0].performanceTaskOver.size(); i++)
  {
    cout << setprecision(0) << fixed << setw(8) << courses[indexOfCourse].studentRecords[0].performanceTaskOver[i];
  }

  cout << setw(8) << "";

  for (int i = 0; i < courses[indexOfCourse].studentRecords[0].writtenTaskOver.size(); i++)
  {
    cout << setprecision(0) << fixed << setw(8) << courses[indexOfCourse].studentRecords[0].writtenTaskOver[i];
  }

  cout << setw(8) << "";

  for (int i = 0; i < courses[indexOfCourse].studentRecords[0].majorExamOver.size(); i++)
  {
    cout << setprecision(0) << fixed << setw(8) << courses[indexOfCourse].studentRecords[0].majorExamOver[i];
  }
  cout << setw(8) << "" << "";

  cout << "\n"
       << endl;

  for (int i = 0; i < courses[indexOfCourse].enrolledStudentID.size(); i++)
  {
    index = indexFind(courses[indexOfCourse].enrolledStudentID[i]);
    cout << left << setw(5) << to_string(i + 1) + ". " << setw(maxLength + 2) << logInCredential[index].username;
    for (int j = 0; j < courses[indexOfCourse].studentRecords[0].performanceTask.size(); j++)
    {
      cout << setprecision(0) << fixed << setw(8) << courses[indexOfCourse].studentRecords[i].performanceTask[j];
    }
    float p = calculateScore(indexOfCourse, courses[indexOfCourse].studentRecords[i], 'a') / calculateScore(indexOfCourse, courses[indexOfCourse].studentRecords[i], 'c');
    courses[indexOfCourse].studentRecords[i].performanceTaskPercent = p;
    cout << setprecision(2) << fixed << setw(8) << p * 100;
    for (int k = 0; k < courses[indexOfCourse].studentRecords[0].writtenTask.size(); k++)
    {
      cout << setprecision(0) << fixed << setw(8) << courses[indexOfCourse].studentRecords[i].writtenTask[k];
    }
    float w = calculateScore(indexOfCourse, courses[indexOfCourse].studentRecords[i], 'b') / calculateScore(indexOfCourse, courses[indexOfCourse].studentRecords[i], 'd');
    courses[indexOfCourse].studentRecords[i].writtenTaskPercent = w;
    cout << setprecision(2) << fixed << setw(8) << w * 100;
    for (int l = 0; l < courses[indexOfCourse].studentRecords[0].majorExam.size(); l++)
    {
      cout << setprecision(0) << fixed << setw(8) << courses[indexOfCourse].studentRecords[i].majorExam[l];
    }
    float m = calculateScore(indexOfCourse, courses[indexOfCourse].studentRecords[i], 'e') / calculateScore(indexOfCourse, courses[indexOfCourse].studentRecords[i], 'f');
    courses[indexOfCourse].studentRecords[i].majorExamPercent = m;
    float final = calculateGrade(indexOfCourse, p, w, m);
    float raw = calculateGradeRaw(indexOfCourse, p, w, m);
    cout << setprecision(2) << fixed << setw(8) << m * 100 << raw << " -> " << final;
    cout << endl;
  }
  cout << "-----------------------------------------" << endl;
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
    cout << "[B] - Edit score to a student" << endl;
    cout << "[C] - Go back" << endl;
    cout << "-----------------------------------------" << endl;
    char option = getChar();

    switch (tolower(option))
    {
    case 'a':
      addScoresToEveryone(indexOfCourse);
      break;
    case 'b':
      editScore(indexOfCourse);
      break;
    case 'c':
      break;
    default:
      break;
    }

    if (tolower(option) == 'c')
    {
      break;
    }
    saveData();
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
  cout << "-----------------------------------------" << endl;
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
    displayStudents(indexOfCourse);
    cout << "Enter maximum score: ";
    over = getInt();
    if (tolower(option) == 'a')
    {
      actName = "P" + to_string(courses[indexOfCourse].studentRecords[0].performanceTask.size() + 1);
      for (int i = 0; i < courses[indexOfCourse].enrolledStudentID.size(); i++)
      {
        int index = indexFind(courses[indexOfCourse].enrolledStudentID[i]);
        cout << "Enter score for " << logInCredential[index].username << " in " << actName << ": ";
        score = getFloat();
        courses[indexOfCourse].studentRecords[i].performanceTask.push_back(score);
        courses[indexOfCourse].studentRecords[i].performanceTaskName.push_back(actName);
        courses[indexOfCourse].studentRecords[i].performanceTaskOver.push_back(over);
      }
    }
    else if (tolower(option) == 'b')
    {
      actName = "W" + to_string(courses[indexOfCourse].studentRecords[0].writtenTask.size() + 1);
      for (int i = 0; i < courses[indexOfCourse].enrolledStudentID.size(); i++)
      {
        int index = indexFind(courses[indexOfCourse].enrolledStudentID[i]);
        cout << "Enter score for " << logInCredential[index].username << " in " << actName << ": ";
        score = getFloat();
        courses[indexOfCourse].studentRecords[i].writtenTask.push_back(score);
        courses[indexOfCourse].studentRecords[i].writtenTaskName.push_back(actName);
        courses[indexOfCourse].studentRecords[i].writtenTaskOver.push_back(over);
      }
    }
    else if (tolower(option) == 'c')
    {
      actName = "M" + to_string(courses[indexOfCourse].studentRecords[0].majorExam.size() + 1);
      for (int i = 0; i < courses[indexOfCourse].enrolledStudentID.size(); i++)
      {
        int index = indexFind(courses[indexOfCourse].enrolledStudentID[i]);
        cout << "Enter score for " << logInCredential[index].username << " in " << actName << ": ";
        score = getFloat();
        courses[indexOfCourse].studentRecords[i].majorExam.push_back(score);
        courses[indexOfCourse].studentRecords[i].majorExamName.push_back(actName);
        courses[indexOfCourse].studentRecords[i].majorExamOver.push_back(over);
      }
    }

    string notif = courses[indexOfCourse].courseName + ": \"" + actName + "\" has been graded.";
    int index;
    for (int i = 0; i < courses[indexOfCourse].enrolledStudentID.size(); i++)
    {
      index = indexFind(courses[indexOfCourse].enrolledStudentID[i]);
      logInCredential[index].notifications.push_back(notif);
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
void editScore(int indexOfCourse)
{
  int chosenStudent;
  c();
  displayStudents(indexOfCourse);
  cout << "Choose student: ";
  chosenStudent = getInt();

  if (chosenStudent > courses[indexOfCourse].enrolledStudentID.size())
  {
    c();
    return;
  }

  int index = indexFind(courses[indexOfCourse].enrolledStudentID[chosenStudent - 1]);
  int chosenStudentIndex = chosenStudent - 1;
  string actName;
  float score, over;
  cout << "Enter Activity Name: ";
  getline(cin, actName);
  actName[0] = toupper(actName[0]);
  if (actName.empty() || actName.size() > 2 || (actName[0] != 'P' && actName[0] != 'W' && actName[0] != 'M'))
  {
    return;
  }
  cout << "Enter new maximum score: ";
  over = getFloat();
  cout << "Enter new score: ";
  score = getFloat();
  int pos;

  if (tolower(actName[0]) == 'p')
  {
    for (int i = 0; i < courses[indexOfCourse].studentRecords[chosenStudentIndex].performanceTaskName.size(); i++)
    {
      if (actName == courses[indexOfCourse].studentRecords[chosenStudentIndex].performanceTaskName[i])
      {
        pos = i;
      }
    }
    courses[indexOfCourse].studentRecords[chosenStudentIndex].performanceTask[pos] = score;
    courses[indexOfCourse].studentRecords[chosenStudentIndex].performanceTaskName[pos] = actName;
    courses[indexOfCourse].studentRecords[chosenStudentIndex].performanceTaskOver[pos] = over;
  }
  if (tolower(actName[0]) == 'w')
  {
    for (int i = 0; i < courses[indexOfCourse].studentRecords[chosenStudentIndex].writtenTaskName.size(); i++)
    {
      if (actName == courses[indexOfCourse].studentRecords[chosenStudentIndex].writtenTaskName[i])
      {
        pos = i;
      }
    }
    courses[indexOfCourse].studentRecords[chosenStudentIndex].writtenTask[pos] = score;
    courses[indexOfCourse].studentRecords[chosenStudentIndex].writtenTaskName[pos] = actName;
    courses[indexOfCourse].studentRecords[chosenStudentIndex].writtenTaskOver[pos] = over;
  }
  if (tolower(actName[0]) == 'm')
  {
    for (int i = 0; i < courses[indexOfCourse].studentRecords[chosenStudentIndex].majorExamName.size(); i++)
    {
      if (actName == courses[indexOfCourse].studentRecords[chosenStudentIndex].majorExamName[i])
      {
        pos = i;
      }
    }
    courses[indexOfCourse].studentRecords[chosenStudentIndex].majorExam[pos] = score;
    courses[indexOfCourse].studentRecords[chosenStudentIndex].majorExamName[pos] = actName;
    courses[indexOfCourse].studentRecords[chosenStudentIndex].majorExamOver[pos] = over;
  }
  string notif = courses[indexOfCourse].courseName + ": \"" + actName + "\" has been updated.";
  logInCredential[index].notifications.push_back(notif);
}
//
void generateReportCourse(int indexOfCourse)
{
  string courseTitle = courses[indexOfCourse].courseName;
  string courseNumber = to_string(courses[indexOfCourse].courseID);

  string filename = "reports/" + courseTitle + " - " + courseNumber + ".txt";

  displayStudents(indexOfCourse);
  cout << "Enter Y/y to generate report for this course: ";
  char option = getChar();
  if (tolower(option) == 'y')
  {
    ofstream outFile(filename);
    if (outFile.is_open())
    {
      outFile << "Course: " << courses[indexOfCourse].courseName << endl;

      int maxLength = 0;
      int index = 0;
      for (int i = 0; i < courses[indexOfCourse].enrolledStudentID.size(); i++)
      {
        index = indexFind(courses[indexOfCourse].enrolledStudentID[i]);
        if (logInCredential[index].username.size() > maxLength)
        {
          maxLength = logInCredential[index].username.size();
        }
      }

      outFile << left << setw(5) << "#" << setw(maxLength + 2) << "Name";
      for (int i = 0; i < courses[indexOfCourse].studentRecords[0].performanceTaskName.size(); i++)
      {
        outFile << setw(8) << courses[indexOfCourse].studentRecords[0].performanceTaskName[i];
      }

      outFile << setw(8) << "%";

      for (int i = 0; i < courses[indexOfCourse].studentRecords[0].writtenTaskName.size(); i++)
      {
        outFile << setw(8) << courses[indexOfCourse].studentRecords[0].writtenTaskName[i];
      }

      outFile << setw(8) << "%";

      for (int i = 0; i < courses[indexOfCourse].studentRecords[0].majorExamName.size(); i++)
      {
        outFile << setw(8) << courses[indexOfCourse].studentRecords[0].majorExamName[i];
      }

      outFile << setw(8) << "%" << "Final";

      outFile << endl;

      outFile << left << setw(5) << "*" << setw(maxLength + 2) << "Max Score:";
      for (int i = 0; i < courses[indexOfCourse].studentRecords[0].performanceTaskOver.size(); i++)
      {
        outFile << setprecision(0) << fixed << setw(8) << courses[indexOfCourse].studentRecords[0].performanceTaskOver[i];
      }

      outFile << setw(8) << "";

      for (int i = 0; i < courses[indexOfCourse].studentRecords[0].writtenTaskOver.size(); i++)
      {
        outFile << setprecision(0) << fixed << setw(8) << courses[indexOfCourse].studentRecords[0].writtenTaskOver[i];
      }

      outFile << setw(8) << "";

      for (int i = 0; i < courses[indexOfCourse].studentRecords[0].majorExamOver.size(); i++)
      {
        outFile << setprecision(0) << fixed << setw(8) << courses[indexOfCourse].studentRecords[0].majorExamOver[i];
      }
      outFile << setw(8) << "" << "";

      outFile << endl;

      for (int i = 0; i < courses[indexOfCourse].enrolledStudentID.size(); i++)
      {
        index = indexFind(courses[indexOfCourse].enrolledStudentID[i]);
        outFile << left << setw(5) << to_string(i + 1) + ". " << setw(maxLength + 2) << logInCredential[index].username;
        for (int j = 0; j < courses[indexOfCourse].studentRecords[0].performanceTask.size(); j++)
        {
          outFile << setprecision(0) << fixed << setw(8) << courses[indexOfCourse].studentRecords[i].performanceTask[j];
        }
        float p = calculateScore(indexOfCourse, courses[indexOfCourse].studentRecords[i], 'a') / calculateScore(indexOfCourse, courses[indexOfCourse].studentRecords[i], 'c');
        outFile << setprecision(2) << fixed << setw(8) << p * 100;
        for (int k = 0; k < courses[indexOfCourse].studentRecords[0].writtenTask.size(); k++)
        {
          outFile << setprecision(0) << fixed << setw(8) << courses[indexOfCourse].studentRecords[i].writtenTask[k];
        }
        float w = calculateScore(indexOfCourse, courses[indexOfCourse].studentRecords[i], 'b') / calculateScore(indexOfCourse, courses[indexOfCourse].studentRecords[i], 'd');
        outFile << setprecision(2) << fixed << setw(8) << w * 100;
        for (int l = 0; l < courses[indexOfCourse].studentRecords[0].majorExam.size(); l++)
        {
          outFile << setprecision(0) << fixed << setw(8) << courses[indexOfCourse].studentRecords[i].majorExam[l];
        }
        float m = calculateScore(indexOfCourse, courses[indexOfCourse].studentRecords[i], 'e') / calculateScore(indexOfCourse, courses[indexOfCourse].studentRecords[i], 'f');
        float final = calculateGrade(indexOfCourse, p, w, m);
        outFile << setprecision(2) << fixed << setw(8) << m * 100 << final;
        outFile << endl;
      }
      outFile.close();
      cout << "File '" << filename << "' written successfully." << endl;
      system("pause");
    }
    else
    {
      cout << "Error opening file: " << filename << endl;
    }
  }
  else
  {
    cout << "-----------------------------------------" << endl;
    cout << "Generating report cancelled" << endl;
    get();
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

    for (int i = 0; i < logInCredential.size(); i++)
    {
      if (logInCredential[i].role == "student")
      {
        outFile << "==========================================\n";
        outFile << "Student Name: " << logInCredential[i].username << endl;
        outFile << "Student ID: " << logInCredential[i].id << endl;
        outFile << "------------------------------------------\n";
        for (int j = 0; j < logInCredential[i].finalGrades.size(); j++)
        {
          outFile << setprecision(2) << fixed << logInCredential[i].finalGrades[j].courseName << ": \t\t" << logInCredential[i].finalGrades[j].grade << endl;
        }
        outFile << "\t\t\t" << endl;
        outFile << "GPA: \t\t" << logInCredential[i].GPA << endl;
        outFile << "==========================================\n>                                        <\n";
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
  if (tolower(option) == 'a')
  {
    cout << "=========================================" << endl;
    cout << "    Student Grades Management System" << endl;
    cout << "=========================================" << endl;
    cout << "Enter Student ID to add: " << endl;
    cout << "-----------------------------------------" << endl;
    studID = getInt();

    int index = indexFind(studID);
    if (index != -1)
    {
      for (int i = 0; i < courses[indexOfCourse].enrolledStudentID.size(); i++)
      {
        if (studID == courses[indexOfCourse].enrolledStudentID[i])
        {
          cout << "Student is already enrolled.\n";
          get();
          return;
        }
      }

      // Add GradeRecord
      GradeRecord newRecord;
      newRecord.studentID = studID;
      newRecord.performanceTaskName = courses[indexOfCourse].studentRecords[0].performanceTaskName;
      newRecord.performanceTaskOver = courses[indexOfCourse].studentRecords[0].performanceTaskOver;
      newRecord.writtenTaskName = courses[indexOfCourse].studentRecords[0].writtenTaskName;
      newRecord.writtenTaskOver = courses[indexOfCourse].studentRecords[0].writtenTaskOver;
      newRecord.majorExamName = courses[indexOfCourse].studentRecords[0].majorExamName;
      newRecord.majorExamOver = courses[indexOfCourse].studentRecords[0].majorExamOver;
      int size = courses[indexOfCourse].studentRecords[0].performanceTask.size();
      for (int j = 0; j < size; j++)
      {
        newRecord.performanceTask.push_back(0.0);
      }
      size = courses[indexOfCourse].studentRecords[0].writtenTask.size();
      for (int j = 0; j < size; j++)
      {
        newRecord.writtenTask.push_back(0.0);
      }
      size = courses[indexOfCourse].studentRecords[0].majorExam.size();
      for (int j = 0; j < size; j++)
      {
        newRecord.majorExam.push_back(0.0);
      }
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
      cout << "Student not found in course.\n";
      get();
    }
  }
  else if (tolower(option) == 'b')
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
  cout << "Enter choice: ";
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
    cout << "Enter name: ";
    getline(cin, name);

    for (int i = 0; i < logInCredential.size(); i++)
    {
      if (logInCredential[i].username == name)
      {
        index = indexFind(logInCredential[i].id);
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
    index = indexFind(id);
    if (index != -1)
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

  if (studentFound == true)
  {
    c();
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
  get();
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
    int index = indexFind(courses[indexOfCourse].enrolledStudentID[j]);
    if (logInCredential[index].finalGrades.empty())
    {
      Grade grades;
      grades.grade = final;
      grades.courseName = courses[indexOfCourse].courseName;

      logInCredential[index].finalGrades.push_back(grades);
    }

    for (int i = 0; i < logInCredential[index].finalGrades.size(); i++)
    {
      if (logInCredential[index].finalGrades[i].courseName == courses[indexOfCourse].courseName)
      {
        index = indexFind(courses[indexOfCourse].enrolledStudentID[j]);
        Grade grades;
        grades.grade = final;
        grades.courseName = courses[indexOfCourse].courseName;

        logInCredential[index].finalGrades[i] = grades;
      }
      if (i == logInCredential[index].finalGrades.size() - 1 && logInCredential[index].finalGrades[i].courseName != courses[indexOfCourse].courseName)
      {
        Grade grades;
        grades.grade = final;
        grades.courseName = courses[indexOfCourse].courseName;

        logInCredential[index].finalGrades.push_back(grades);
      }
    }
  }
  string notif = "Grades in " + courses[indexOfCourse].courseName + " have been posted.";
  int index;
  for (int i = 0; i < courses[indexOfCourse].enrolledStudentID.size(); i++)
  {
    index = indexFind(courses[indexOfCourse].enrolledStudentID[i]);
    logInCredential[index].notifications.push_back(notif);
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

void viewMessages(int indexOfCourse)
{
  int index;
  cout << "=========================================" << endl;
  cout << "    Student Grades Management System" << endl;
  cout << "=========================================" << endl;
  cout << "Messages" << endl;
  for (int i = 0; i < courses[indexOfCourse].enrolledStudentID.size(); i++)
  {
    index = indexFind(courses[indexOfCourse].enrolledStudentID[i]);
    cout << to_string(i + 1) + ". " << logInCredential[index].username << endl;
  }
  cout << "-----------------------------------------" << endl;
  int choice = getInt();
  c();

  index = indexFind(courses[indexOfCourse].enrolledStudentID[choice - 1]);
  cout << "=========================================" << endl;
  cout << "    Student Grades Management System" << endl;
  cout << "=========================================" << endl;
  cout << "Message: " << logInCredential[index].username << "\n" << endl;

  if (courses[indexOfCourse].messages.size() != courses[indexOfCourse].enrolledStudentID.size())
  {
    for (int i = courses[indexOfCourse].messages.size(); i < courses[indexOfCourse].enrolledStudentID.size(); i++)
    {
      Message messageSlot;
      courses[indexOfCourse].messages.push_back(messageSlot);
    }
  }

  if (courses[indexOfCourse].messages[index].name.empty())
  {
    cout << "No available messsages" << endl;
  }
  for (int i = 0; i < courses[indexOfCourse].messages[index].name.size(); i++)
  {
    cout << courses[indexOfCourse].messages[index].name[i] << ": " << courses[indexOfCourse].messages[index].messages[i] << endl;
  }
  cout << "-----------------------------------------" << endl;
  while (true)
  {
    cout << "Enter message(q to go back): ";
    string message;
    getline(cin, message);

    if (message == "q")
    {
      break;
    }
    else
    {
      Message newMessage;

      newMessage.name.push_back(logInCredential[userIndex].username);
      newMessage.messages.push_back(message);
      courses[indexOfCourse].messages.push_back(newMessage);
    }
  }
}
