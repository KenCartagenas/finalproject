#ifndef MAINMENU_H
#define MAINMENU_H
#include <string>
#include "data.h"
using namespace std;

void mainMenu();
void teacherMenu();
void studentMenu();
void createCourse();
void openClass();
void editStudent(int indexOfCourse);
void displayStudents(int indexOfCourse);
void addScoresToEveryone(int indexOfCourse);
void addScoreToStudent(int indexOfCourse);
void editScore(int indexOfCourse);
void generateReportCourse(int indexOfCourse);
void generateReportGPA();
void addOrRemove(int indexOfCourse);
void customizeGradeSettings(int indexOfCourse);
void searchStudentSummary();
void editUserProfile();
void openCourse(const User& student);
void releaseGrades(int indexOfCourse);
void openNotif();

#endif
