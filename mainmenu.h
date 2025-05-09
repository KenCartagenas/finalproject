#ifndef MAINMENU_H
#define MAINMENU_H
#include <string>
using namespace std;

void displayMainMenuTemplate(string text);
void mainMenu();
void teacherMenu();
void studentMenu();
void createCourse();
void openClass();
void displayStudents(int indexOfCourse);
void addScoresToEveryone(int indexOfCourse);
void addScoreToStudent(int indexOfCourse);
void addAdditionalScoreToStudent(int indexOfCourse);
void editScore(int indexOfCourse);

#endif
