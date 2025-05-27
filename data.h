#ifndef DATA_H
#define DATA_H

#include <string>
#include <vector>
using namespace std;

// g++ finalproject/main.cpp finalproject/account.cpp finalproject/data.cpp finalproject/ED.cpp finalproject/mainmenu.cpp -o main.exe
// g++ main.cpp account.cpp data.cpp ED.cpp mainmenu.cpp -o main.exe
// g++ main.cpp account.cpp data.cpp ED.cpp mainmenu.cpp calculate.cpp jsonhelper.cpp -o main.exe
// g++ main.cpp account.cpp data.cpp ED.cpp mainmenu.cpp calculate.cpp jsonhelper.cpp utils.cpp -o main.exe
// g++ -std=c++17 -static -static-libgcc -static-libstdc++ -o main.exe main.cpp account.cpp data.cpp ED.cpp mainmenu.cpp calculate.cpp jsonhelper.cpp utils.cpp
struct GradeRecord
{
  int studentID;
  float finalGrade;
  vector<string> majorExamName;
  vector<string> performanceTaskName;
  vector<string> writtenTaskName;
  vector<float> majorExam;
  vector<float> performanceTask;
  vector<float> writtenTask;
  vector<float> majorExamOver;
  vector<float> performanceTaskOver;
  vector<float> writtenTaskOver;
  float majorExamPercent;
  float performanceTaskPercent;
  float writtenTaskPercent;
};

struct Message
{
  vector<string> name;
  vector<string> messages;
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
  vector<Message> messages;
};

struct Grade
{
  string courseName;
  float grade;
};

struct User
{
  string role;
  int id;
  int space;
  float GPA;
  string username;
  string password;
  string section;
  string program;
  vector<Grade> finalGrades;
  vector<int> coursesEnrolled;
  vector<int> coursesHandled;
  vector<string> notifications;
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
