#ifndef DATA_H
#define DATA_H

#include <string>
#include <vector>
using namespace std;

//general data
struct User
{
  string role;
  int id;
  int space;
  string username;
  string password;
};

struct Course
{
  int courseID;
  string courseName;
  string teacher;
  vector<int> enrolledStudentID;
  float base;
  float majorExamPercentage;
  float performanceTaskPercentage;
  float writtenTaskpercentage;
  vector<float> majorExam;
  vector<float> performanceTask;
  vector<float> writtenTask;
}

extern vector<User> logInCredential;
bool isLoggedIn;
bool isRunning = true;
int size = logInCredential.size();
int loggedInID;

//functions
void loadData();
void saveData();
#endif
