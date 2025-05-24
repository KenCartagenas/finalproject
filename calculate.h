#ifndef CALCULATE_H
#define CALCULATE_H
#include "data.h"

float calculateScore(int indexOfCourse, GradeRecord recordsToCalculate, char component);
float calculateGrade(int indexOfCourse, float performanceGrade, float writtenGrade, float majorGrade);
float calculateGradeRaw(int indexOfCourse, float performanceGrade, float writtenGrade, float majorGrade);
void calculateGPA();
float getFinal(int indexOfCourse, int j);
int calculateTotalstudents();
float calculateAverageGrade();

#endif
