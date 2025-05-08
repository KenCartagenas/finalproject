#ifndef DATA_H
#define DATA_H

#include <string>
#include <vector>
using namespace std;

//g++ finalproject/main.cpp finalproject/account.cpp finalproject/data.cpp finalproject/ED.cpp finalproject/mainmenu.cpp -o main.exe 

struct GradeRecord {
  int studentID;
  vector<string> majorExamName;
  vector<string> performanceTaskName;
  vector<string> writtenTaskName;
  vector<float> majorExam;
  vector<float> performanceTask;
  vector<float> writtenTask;
};

struct Course
{
  int courseID;
  string courseName;
  string teacher;
  int teacherID;
  vector<int> enrolledStudentID;
  float base;
  float majorExamPercentage;
  float performanceTaskPercentage;
  float writtenTaskPercentage;
  vector<GradeRecord> studentRecords;
};

struct User
{
  string role;
  int id;
  int space;
  string username;
  string password;
  string section;
  vector<int> coursesEnrolled;
  vector<int> coursesHandled;
};

extern vector<Course> courses;
extern vector<User> logInCredential;
extern bool isLoggedIn;
extern bool isRunning;
extern int loggedInID;
extern const int BASE_ID;
extern int coursesCount;
extern int userIndex;

void loadData();
void saveData();
#endif
