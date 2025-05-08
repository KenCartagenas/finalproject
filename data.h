#ifndef DATA_H
#define DATA_H

#include <string>
#include <vector>
using namespace std;

//g++ finalproject/main.cpp finalproject/account.cpp finalproject/data.cpp finalproject/ED.cpp finalproject/mainmenu.cpp -o main.exe 

struct User
{
  string role;
  int id;
  int space;
  string username;
  string password;
  string section;
  int coursesEnrolled;
};

struct GradeRecord {
  int studentID;
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
  float writtenTaskpercentage;
  vector<GradeRecord> studentRecords;
};

extern vector<Course> courses;
extern vector<User> logInCredential;
extern bool isLoggedIn;
extern bool isRunning;
extern int loggedInID;
extern const int BASE_ID;

void loadData();
void saveData();
#endif
