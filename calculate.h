#ifndef CALCULATE_H
#define CALCULATE_H
#include "data.h"

float calculateScore(int indexOfCourse, GradeRecord recordsToCalculate, char component);
float calculateGrade(int indexOfCourse, float performanceGrade, float writtenGrade, float majorGrade);
float calculateGPA();
float getFinal(int indexOfCourse, int j);

#endif
